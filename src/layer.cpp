/**
 * Copyright (C) 2012 by INdT
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @author Rodrigo Goncalves de Oliveira <rodrigo.goncalves@openbossa.org>
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

#include "layer.h"

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#if QT_VERSION >= 0x050000
#include <QtQml/QQmlProperty>
#endif

//! Class constructor
Layer::Layer(QuasiDeclarativeItem *parent)
    : QuasiPaintedItem(parent)
    , m_direction((Quasi::LayerDirection)-1) // Backward
    , m_factor(1.0)
    , m_type(Quasi::InfiniteType)
    , m_columnOffset(0)
    , m_imageWidth(0)
    , m_imageHeight(0)
{
#if QT_VERSION >= 0x050000
    setZ(Quasi::InteractionLayerOrdering_01);
    // this activates the item layered mode
    QQmlProperty(this, "layer.enabled").write(true);
#else
    setZValue(Quasi::InteractionLayerOrdering_01);
#endif
}

//! Class destructor
Layer::~Layer()
{
}

//! Stores the source path for the image
/*!
 * \param source the image path
 */
void Layer::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;

        imageReader.setFileName(m_source);
        m_imageWidth = imageReader.size().width();
        m_imageHeight = imageReader.size().height();
    }
}

//! Gets the image source path
/*!
 * \return the source path for the image
 */
QString Layer::source() const
{
    return m_source;
}

void Layer::setDirection(const Quasi::LayerDirection &direction)
{
    if (direction != m_direction) {
        if (direction == Quasi::BackwardDirection)
            m_direction = (Quasi::LayerDirection)-1; // insane black magic
        else
            m_direction = direction;

        if (m_offsets.count() != 0)
            m_columnOffset = (m_columnOffset + 2) % m_offsets.size();

        emit directionChanged();
    }
}

//! Stores the layer update factor
/*!
 * \param factor the factor value
 */
void Layer::setFactor(qreal factor)
{
    if (m_factor != factor)
        m_factor = factor;
}

//! Gets the layer update factor
/*!
 * \return layer update factor
 */
qreal Layer::factor() const
{
    return m_factor;
}

//! Stores the layer z order
/*!
 * \param order the layer z order
 */
void Layer::setOrder(Quasi::Ordering order)
{
#if QT_VERSION >= 0x050000
    if (z() != order)
        setZ(order);
#else
    if (zValue() != order)
        setZValue(order);
#endif
}

//! Gets the layer z order
/*!
 * \return layer z order
 */
Quasi::Ordering Layer::order() const
{
#if QT_VERSION >= 0x050000
    return (Quasi::Ordering)z();
#else
    return (Quasi::Ordering)zValue();
#endif
}

void Layer::setLayerType(const Quasi::LayerType &type)
{
    if (type != m_type) {
        m_type = type;

        emit layerTypeChanged();
    }
}

// XXX
void Layer::initialize()
{
    generateOffsets();
    drawPixmap();
}

void Layer::generateOffsets()
{
    // TODO mirror?
    // TODO generate Y offsets too?
    bool completed = false;
    int start = 0;
    int step = boundingRect().width();
    int max = m_imageWidth * (m_type == Quasi::MirroredType ? 2 : 1);// TODO
    int count = 0;
    int maxCount = step * 2;
    bool first = true;
    Offsets::OffsetsList firstPoint;

    while (!completed) {
        Offsets::OffsetsList offsetsList;

        int size;
        int end = 0;

        while (count < maxCount) {
            end = (start + step) % max;

            //bool mirror = start >= m_imageWidth;
            if (end - start > 0) {
                size = step;
                count += size;

                bool brake = (start < m_imageWidth) && (start + size >= m_imageWidth);
                if (brake) {
                    offsetsList.append(Offsets(start, m_imageWidth - start));
                    offsetsList.append(Offsets(0, size - (m_imageWidth - start), true));
                } else
                    offsetsList.append(Offsets(start, size, start >= m_imageWidth));

                if (count != maxCount)
                    start = end;
            } else {
                int oldStart = start;
                size = max - start;
                count += size;

                offsetsList.append(Offsets(start, size, m_type == Quasi::MirroredType));

                size = step - size;
                start = 0;
                count += size;

                if (size != 0) {
                    offsetsList.append(Offsets(0, size));
                }

                if (count <= maxCount / 2)
                    start = size;
                else
                    start = oldStart;
            }
        }

        count = 0;

        if (offsetsList == firstPoint)
            completed = true;
        else
            m_offsets.append(offsetsList);

        if (first) {
            firstPoint = offsetsList;
            first = false;
        }
    }
}

QPixmap Layer::generatePartialPixmap(int startPoint, int size)
{

    if (!imageReader.canRead())
        imageReader.setFileName(m_source); // XXX me doesn't like this

    imageReader.setClipRect(QRect(startPoint, 0,
                                  size,
                                  boundingRect().height() * 1));

    return QPixmap::fromImageReader(&imageReader);
}

void Layer::drawPixmap()
{
    if ((boundingRect().width() == 0) || (boundingRect().height() == 0))
        return;

    if (m_currentImage)
        delete m_currentImage;

    // XXX if image width < boundingRect.width?

    m_currentImage = new QImage(boundingRect().width() * 2, boundingRect().height(), QImage::Format_ARGB32_Premultiplied);///
    m_currentImage->fill(Qt::transparent);

    QPainter p(m_currentImage);
        int xPoint = 0;
        for (int i = 0; i < m_offsets[m_columnOffset].size(); i++) {
            Offsets offset = m_offsets[m_columnOffset].at(i);

            QPixmap pix;
            if (offset.mirror()) {
                int realPoint;
                if (offset.point() == 0)
                    realPoint = m_imageWidth - offset.point() - offset.size();
                else
                    realPoint = m_imageWidth - (offset.point() - m_imageWidth) - offset.size();

                pix = generatePartialPixmap(realPoint, offset.size());
                p.drawPixmap(xPoint, 0, pix.width(), pix.height(),
                             pix.transformed(QTransform().scale(-1, 1), Qt::FastTransformation));
            } else {
                pix = generatePartialPixmap(offset.point(), offset.size());
                p.drawPixmap(xPoint, 0, pix);
            }

            xPoint += pix.width();
        }
    p.end();

    if (m_direction == Quasi::ForwardDirection)
        m_columnOffset = (m_columnOffset - 1 < 0) ? m_offsets.size() - 1 : m_columnOffset - 1;
    else
        m_columnOffset = (m_columnOffset + 1) % m_offsets.size();
}
