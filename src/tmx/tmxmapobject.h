#ifndef TMXMAPOBJECT_H
#define TMXMAPOBJECT_H

#include "tmxobject.h"

#include <libtiled/mapobject.h>

class TMXMapObject : public TMXObject
{
    Q_OBJECT

public:
    explicit TMXMapObject(Tiled::MapObject *mapObject, QObject *parent = 0)
        : TMXObject(mapObject, parent), m_mapObject(mapObject) {}

    Tiled::MapObject *tiledMapObject() const { return m_mapObject; }
    void setTiledMapObject(Tiled::MapObject *mapObject) { m_mapObject = mapObject; }

    const QString &name() const { return m_mapObject->name(); }
    void setName(const QString &name) { m_mapObject->setName(name); }

    const QString &type() const { return m_mapObject->type(); }
    void setType(const QString &type) { m_mapObject->setType(type); }

    const QPointF &position() const { return m_mapObject->position(); }
    void setPosition(const QPointF &pos) { m_mapObject->setPosition(pos); }

    qreal x() const { return m_mapObject->x(); }
    void setX(qreal x) { m_mapObject->setX(x); }

    qreal y() const { return m_mapObject->y(); }
    void setY(qreal y) { m_mapObject->setY(y); }

    qreal width() const { return m_mapObject->width(); }
    void setWidth(qreal width) { m_mapObject->setWidth(width); }

    qreal height() const { return m_mapObject->height(); }
    void setHeight(qreal height) { m_mapObject->setHeight(height); }

private:
    Tiled::MapObject *m_mapObject;
};

#endif // TMXMAPOBJECT_H
