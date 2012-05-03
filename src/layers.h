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

#ifndef _LAYERS_H_
#define _LAYERS_H_

#include "enums.h"
#include "entity.h"
#include "layer.h"

#include <QtCore/QList>
#include <QtCore/QString>

class Layers : public Entity
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PROPERTY(QQmlListProperty<Layer> layers READ layers)
#else
    Q_PROPERTY(QDeclarativeListProperty<Layer> layers READ layers)
#endif
    Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged)
    Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged)

public:
    Layers(Scene *parent = 0);
    ~Layers();

#if QT_VERSION >= 0x050000
    QQmlListProperty<Layer> layers() const;
#else
    QDeclarativeListProperty<Layer> layers() const;
#endif

    qreal xOffset() const { return m_xOffset; }
    void setXOffset(const qreal &xOffset);

    qreal yOffset() const { return m_yOffset; }
    void setYOffset(const qreal &yOffset);

    void update(const int &delta);

signals:
    void xOffsetChanged();
    void yOffsetChanged();

public slots:
    void changeXOffset();
    void changeYOffset();

private:

#if QT_VERSION >= 0x050000
    static void append_layer(QQmlListProperty<Layer> *list, Layer *layer);
#else
    static void append_layer(QDeclarativeListProperty<Layer> *list, Layer *layer);
#endif

    Layer::LayerList m_layers;

    qreal m_xOffset;
    qreal m_yOffset;
};

#endif /* _LAYERS_H_ */
