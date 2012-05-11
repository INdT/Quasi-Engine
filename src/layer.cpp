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
    , m_percentLoading(0.1)
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
void Layer::setFactor(const qreal &factor)
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
void Layer::setOrder(const Quasi::Ordering &order)
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

void Layer::setPercentLoading(const qreal &percent)
{
    if (m_percentLoading != percent)
        m_percentLoading = qBound(0.0, percent, 1.0);
}

qreal Layer::percentLoading() const
{
    return m_percentLoading;
}

// XXX
void Layer::initialize()
{
    //generateFirstImage(QPoint(10546, 0)); // X > x + w
    //generateFirstImage(QPoint(0, 800)); // Y > y + h XXX
    generateFirstImage(QPoint(10000, 0)); // X > x + w XXX
    //generateFirstImage(QPoint(150, 700)); // Y > y + h
    //generateFirstImage(QPoint(10000, 150)); // X > x + w
    //generateFirstImage(QPoint(10, 40)); // X < 0
    //generateFirstImage(QPoint(40, 40)); // X < 0
    //generateFirstImage(QPoint(160, 30)); // Y < 0
    //generateFirstImage(QPoint(80, 10)); // Y < 0
    //generateFirstImage(QPoint(10546, 800)); // x = w && y = h
    //generateFirstImage(QPoint(10000, 700)); // X > x + w && Y > y + h
    //generateFirstImage(); // 0, 0
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

///
void Layer::generateFirstImage(const QPoint &pos)
{

    if (!imageReader.canRead())
        imageReader.setFileName(m_source); // XXX me doesn't like this

    int readerWidth = imageReader.size().width();
    int readerHeight = imageReader.size().height();

    int hDelta = boundingRect().width() * m_percentLoading;
    int vDelta = boundingRect().height() * m_percentLoading;

    // if want to use the same governor
    /*if (hDelta > vDelta)
        vDelta = hDelta;
    else
        hDelta = vDelta;*/

    // for viewport
    int realX = pos.x(); // 10546
    int realY = pos.y(); // 800
    //int realX = boundingRect().width() / 2;
    //int realY = boundingRect().height() / 2;
    int realW = boundingRect().width();
    int realH = boundingRect().height();
    // for extra border
    int newX = realX - hDelta;
    int newY = realY - vDelta;
    int newW = realW + (2 * hDelta);
    int newH = realH + (2 * vDelta);

    bool bigW = false;
    bool bigH = false;
    if (newX + newW > readerWidth) {
        newW = readerWidth - newX;
        bigW = true;
    }
    if (newY + newH > readerHeight) {
        bigH = true;
        newH = readerHeight - newY;
    }
    // if infinite
    /*if (realX <= 0)
        newX = 0;
    if (realY <= 0)
        newY = 0;*/

    /*
    qDebug() << QString("realX: %1 realY: %2 realW: %3 realH: %4")
                        .arg(realX).arg(realY).arg(realW).arg(realH);///
    qDebug() << QString("newX: %1 newY: %2 newW: %3 newH: %4")
                        .arg(newX).arg(newY).arg(newW).arg(newH);///
    qDebug() << QString("hDelta: %1 vDelta: %2").arg(hDelta).arg(vDelta);///
    */

    imageReader.setClipRect(QRect(newX, newY, newW, newH));

    QPixmap pix(realW + (2 * hDelta), realH + (2 * vDelta));
    QPainter p(&pix);
        p.drawPixmap(0, 0, QPixmap::fromImageReader(&imageReader));
        //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/CUEN.png");///

        if (newX < 0 && newY < 0) { // XXX free-for-all
            qDebug() << "CUEN1";
            // top-left
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(readerWidth + newX, readerHeight + newY, hDelta, vDelta));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen1.png");///
            p.drawPixmap(0, 0, QPixmap::fromImageReader(&imageReader));

            // top-right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, readerHeight + newY, newW, vDelta));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(-newX, 0, QPixmap::fromImageReader(&imageReader));

            // bottom-left
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(readerWidth + newX, 0, hDelta, newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen3.png");///
            p.drawPixmap(0, -newY, QPixmap::fromImageReader(&imageReader));
        } else if (newX < 0 && newY >= 0) { // XXX fixed H
            qDebug() << "CUEN2";
            // left
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(readerWidth + newX, newY, -newX, newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen3.png");///
            p.drawPixmap(0, 0, QPixmap::fromImageReader(&imageReader));
        } else if (newY < 0 && newX >= 0 && !bigW) { // XXX fixed V
            qDebug() << "CUEN3";
            // top
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(newX, readerHeight + newY, newW, -newY));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(0, 0, QPixmap::fromImageReader(&imageReader));
        } else if (bigW && bigH) { // free-for-all
            qDebug() << "CUEN4";
            // bottom-right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, 0, realW + (2 * hDelta) - newW, realH + (2 * vDelta) - newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen1.png");///
            p.drawPixmap(newW, newH, QPixmap::fromImageReader(&imageReader));

            // top-right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, newY, realW + (2 * hDelta) - newW, readerHeight - newY));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(readerWidth - newX, 0, QPixmap::fromImageReader(&imageReader));

            // bottom-left
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(newX, 0, readerWidth - newX, realH + (2 * vDelta) - newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen3.png");///
            p.drawPixmap(0, readerHeight - newY, QPixmap::fromImageReader(&imageReader));
        } else if (bigW && !bigH && newY >= 0) { // free-for-all
            qDebug() << "CUEN5";
            // right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, newY, realW + (2 * hDelta) - newW, newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(readerWidth - newX, 0, QPixmap::fromImageReader(&imageReader));
        } else if (!bigW && bigH) { // free-for-all
            qDebug() << "CUEN6";
            // bottom
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(newX, 0, newW, realH + (2 * vDelta) - newH));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(0, newH, QPixmap::fromImageReader(&imageReader));
        } else if (bigW && !bigH && newY < 0) { // free-for-all
            qDebug() << "CUEN7";
            // top-left
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(newX, readerHeight + newY, hDelta, vDelta));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen1.png");///
            p.drawPixmap(0, 0, QPixmap::fromImageReader(&imageReader));

            // top-right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, readerHeight + newY, realW + (2 * hDelta) - newW, vDelta));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(readerWidth - newX, 0, QPixmap::fromImageReader(&imageReader));

            // bottom-right
            imageReader.setFileName(m_source);
            imageReader.setClipRect(QRect(0, 0, realW + (2 * hDelta) - newW, newH - vDelta));
            //QPixmap::fromImageReader(&imageReader).save("/tmp/ABC/cuen2.png");///
            p.drawPixmap(readerWidth - newX, -newY, QPixmap::fromImageReader(&imageReader));
        }

        QColor c(Qt::red);
        c.setAlphaF(0.5);
        p.fillRect(hDelta, vDelta, realW, realH, c);///
    p.end();

    //qDebug() << QString("pixW: %1 pixH: %2").arg(pix.width()).arg(pix.height());///

    pix.save(QString("/tmp/ABC/image%1.png").arg(realX));
}
///

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
