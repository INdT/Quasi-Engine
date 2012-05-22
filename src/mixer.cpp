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

#include "mixer.h"

Mixer::Mixer(QObject *parent)
    : QObject(parent)
    , m_volume(1.0)
    , m_paused(false)
    , m_stopped(false)
    , m_muted(false)
{
}

Mixer::~Mixer()
{
}

qreal Mixer::volume() const
{
    return m_volume;
}

void Mixer::setVolume(const qreal &volume)
{
    qreal newVolume = qBound(0.0, volume, 1.0);

    if (m_volume != newVolume) {
        m_volume = newVolume;

        emit volumeChanged(m_volume);
    }
}

void Mixer::pause()
{
    m_paused = !m_paused;

    emit pauseChanged();
}

void Mixer::stop()
{
    m_stopped = !m_stopped;

    emit stopChanged();
}

void Mixer::mute()
{
    m_muted = !m_muted;

    emit muteChanged();
}
