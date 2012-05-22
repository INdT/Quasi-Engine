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

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <QtCore/QBuffer>
#include <QtMultimedia/QAudioOutput>

#include "mixer.h"

class Audio: public QObject
{
    Q_OBJECT

    Q_PROPERTY(Mixer *mixer WRITE setMixer)
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume)

public:
    Audio(QObject *parent = 0);
    ~Audio();

    void setMixer(Mixer *mixer);

    QString source() const;
    void setSource(const QString &source);

    qreal volume() const;
    void setVolume(const qreal &volume);

public slots:
    void playLoop();
    void play();
    void pause();
    void stop();
    void mute();

signals:

private slots:
    void evaluate(QAudio::State state);
    void changeVolume(const qreal &volume);
    void changePause();
    void changeStop();
    void changeMute();

private:
    void internalPlay(const bool &loop = false);

    Mixer *m_mixer;

    QAudioOutput *m_audioOutput;
    QAudioFormat m_audioFormat;
    QAudioDeviceInfo m_deviceInfo;
    QBuffer *m_buffer;
    QByteArray *m_byteArray;

    QString m_source;
    qreal m_volume;
    bool m_loop;
    bool m_paused;
    bool m_muted;
};

#endif /* _AUDIO_ */
