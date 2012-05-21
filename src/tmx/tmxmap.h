#ifndef TMXMAP_H
#define TMXMAP_H

#include "tmximagelayer.h"
#include "tmxobject.h"
#include "tmxobjectlayer.h"
#include "tmxtilelayer.h"

#include <libtiled/map.h>

class TMXMap : public TMXObject
{
    Q_OBJECT

public:
    explicit TMXMap(Tiled::Map *tiledMap, QObject *parent = 0)
        : TMXObject(tiledMap, parent), m_tiledMap(tiledMap) {}

    Tiled::Map *tiledMap() const { return m_tiledMap; }
    void setTiledMap(Tiled::Map *tiledMap) { m_tiledMap = tiledMap; }

    int width() const { return m_tiledMap->width(); }
    void setWidth(int width) { m_tiledMap->setWidth(width); }

    int height() const { return m_tiledMap->height(); }
    void setHeight(int height) { m_tiledMap->setHeight(height); }

    int tileWidth() const { return m_tiledMap->tileWidth(); }
    int tileHeight() const { return m_tiledMap->tileHeight(); }

private:
    Tiled::Map *m_tiledMap;
};

#endif // TMXMAP_H
