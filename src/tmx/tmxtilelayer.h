#ifndef TMXTILELAYER_H
#define TMXTILELAYER_H

#include "tmxlayer.h"

#include <libtiled/tilelayer.h>

class TMXTileLayer : public TMXLayer
{
    Q_OBJECT

public:
    explicit TMXTileLayer(Tiled::TileLayer *tileLayer, QObject *parent = 0)
        : TMXLayer(tileLayer, parent), m_tileLayer(tileLayer) {}

    Tiled::TileLayer *tiledTileLayer() const { return m_tileLayer; }
    void setTiledTileLayer(Tiled::TileLayer *tileLayer) { m_tileLayer = tileLayer; }

private:
    Tiled::TileLayer *m_tileLayer;
};

#endif // TMXTILELAYER_H
