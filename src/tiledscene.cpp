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

#include "tiledscene.h"

#include <libtiled/mapreader.h>

TiledScene::TiledScene(Game *parent)
    : Scene(parent)
    , m_map(0)
{
}

void TiledScene::setSource(const QString &source)
{
    if (m_source == source)
        return;

    m_source = source;

    if (!loadMap(source))
        return;

    setImplicitWidth(m_map->width() * m_map->tileWidth());
    setImplicitHeight(m_map->height() * m_map->tileHeight());

    loadLayers();

    emit sourceChanged();
}

bool TiledScene::loadMap(const QString &source)
{
    Tiled::MapReader reader;
    Tiled::Map *tiledMap = 0;

    if (m_map)
        m_map->deleteLater();

    tiledMap = reader.readMap(source);
    if (!tiledMap) {
        qCritical("Failed to read map: %s", qPrintable(source));
        return false;
    }

    m_map = new TMXMap(tiledMap, this);

    return true;
}

void TiledScene::loadLayers()
{
}
