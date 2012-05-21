#ifndef TMXIMAGELAYER_H
#define TMXIMAGELAYER_H

#include "tmxlayer.h"

#include <libtiled/imagelayer.h>

class TMXImageLayer : public TMXLayer
{
    Q_OBJECT

public:
    explicit TMXImageLayer(Tiled::ImageLayer *imageLayer, QObject *parent = 0)
        : TMXLayer(imageLayer, parent) , m_imageLayer(imageLayer) {}

    Tiled::ImageLayer *tiledImageLayer() const { return m_imageLayer; }
    void setTiledImageLayer(Tiled::ImageLayer *imageLayer) { m_imageLayer = imageLayer; }

private:
    Tiled::ImageLayer *m_imageLayer;
};

#endif // TMXIMAGELAYER_H
