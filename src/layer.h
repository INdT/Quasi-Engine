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

#ifndef _LAYER_H_
#define _LAYER_H_

#include "enums.h"
#include "entity.h"
#include "quasipainteditem.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#include <QtGui/QImageReader>

class Offsets {
public:
    typedef QList<Offsets> OffsetsList;

    Offsets() { m_point = m_size = 0; m_mirror = false; }
    Offsets(const int &point, const int &size, const bool &mirror = false)
    {
        m_point = point;
        m_size = size;
        m_mirror = mirror;
    }
    ~Offsets() {}

    int point() const { return m_point; }
    void setPoint(const int &point)
    {
        if (point != m_point)
            m_point = point;
    }

    int size() const { return m_size; }
    void setSize(const int &size)
    {
        if (size != m_size)
            m_size = size;
    }

    bool mirror() const { return m_mirror; }
    void setMirror(const bool &mirror)
    {
        if (mirror != m_mirror)
            m_mirror = mirror;
    }

    bool operator==(const Offsets &other) const {
        return ((this->point() == other.point())
                && (this->size() == other.size()));
    }

private:
    int m_point;
    int m_size;
    bool m_mirror;
};

//! A layer class
class Layer: public QuasiPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(qreal factor READ factor WRITE setFactor)
    Q_PROPERTY(Quasi::Ordering order READ order WRITE setOrder)
    Q_PROPERTY(Quasi::LayerType layerType READ layerType WRITE setLayerType NOTIFY layerTypeChanged)
    Q_PROPERTY(Quasi::LayerDirection direction READ direction WRITE setDirection NOTIFY directionChanged)
    // XXX expose on Layers only
    Q_PROPERTY(qreal percentLoading READ percentLoading WRITE setPercentLoading)

public:
    typedef QList<Layer *> LayerList; //! A layer list based on QList

    Layer(QuasiDeclarativeItem *parent = 0);
    ~Layer();

    void setSource(const QString &source);
    QString source() const;

    void setFactor(const qreal &factor);
    qreal factor() const;

    void setOrder(const Quasi::Ordering &order);
    Quasi::Ordering order() const;

    Quasi::LayerType layerType() const { return m_type; };
    void setLayerType(const Quasi::LayerType &type);

    Quasi::LayerDirection direction() const { return m_direction; };
    void setDirection(const Quasi::LayerDirection &direction);

    void setPercentLoading(const qreal &percent);
    qreal percentLoading() const;

    int count() const;

    void drawPixmap();
    void initialize();

signals:
    void directionChanged();
    void layerTypeChanged();

protected:
    QImage *m_currentImage;
    Quasi::LayerDirection m_direction;

    qreal m_factor;

private:
    QImageReader imageReader;
    QPixmap generatePartialPixmap(int startPoint, int size);
    void generateFirstImage(const QPoint &pos = QPoint(0, 0));// TODO maybe this function will be another one
    void generateOffsets();

    QList<Offsets::OffsetsList> m_offsets;

    QString m_source;
    Quasi::LayerType m_type;

    int m_columnOffset;
    int m_imageWidth;
    int m_imageHeight;
    qreal m_percentLoading;
};

#endif /* _LAYER */
