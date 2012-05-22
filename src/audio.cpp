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

#include <QtCore/QFile>
#include <QDebug>///

#include "audio.h"

Audio::Audio(QObject *parent)
    : QObject(parent)
    , m_mixer(0)
    , m_audioOutput(0)
    , m_volume(1.0)
    , m_loop(false)
    , m_paused(false)
    , m_muted(false)
{
    // XXX take  alook on QSoundEffect to use instead of QAudioOutput
    // ^^^ QSoundEffects is just available on Qt5. Cuen.
    // TODO how to expose or get these vars?
    m_audioFormat.setFrequency(44100);
    //m_audioFormat.setFrequency(16000);
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setChannels(1);
    m_audioFormat.setCodec("audio/pcm");

    //
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);

    m_deviceInfo = QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    qDebug() << "Supported codecs:";///
    qDebug() << m_deviceInfo.supportedCodecs();///
}

Audio::~Audio()
{
}

void Audio::setMixer(Mixer *mixer)
{
    m_mixer = mixer;

    connect(m_mixer, SIGNAL(volumeChanged(const qreal &)),
            this, SLOT(changeVolume(const qreal &)));

    connect(m_mixer, SIGNAL(pauseChanged()), this, SLOT(changePause()));
    connect(m_mixer, SIGNAL(stopChanged()), this, SLOT(changeStop()));
    connect(m_mixer, SIGNAL(muteChanged()), this, SLOT(changeMute()));
}

QString Audio::source() const
{
    return m_source;
}

void Audio::setSource(const QString &source)
{
    if (m_source != source) {
        m_source = source;

        stop();

        // initialize sound buffer
        delete m_byteArray;
        delete m_buffer;

        QFile inputFile(m_source);

        // we need to keep this byte array to calculate the volume of the sound on qt4 =/
        inputFile.open(QIODevice::ReadOnly);
        inputFile.seek(44); // skipping wav header
        m_byteArray = new QByteArray(inputFile.readAll());
        inputFile.close();

        m_buffer = new QBuffer(m_byteArray);
    }
}

qreal Audio::volume() const
{
    return m_volume;
}

void Audio::setVolume(const qreal &volume)
{
    qreal newVolume = qBound(0.0, volume, 1.0);

    if (m_volume != newVolume) {
        m_volume = newVolume;

        // TODO set volume
#if QT_VERSION >= 0x050000
        m_audioOutput->setVolume(m_volume);
#else
        qDebug() << "CUEN";
        // TODO qt4?
        /*QByteArray *newByteArray = new QByteArray(m_byteArray->size(), 0);
        if (m_byteArray) {
            qDebug() << m_byteArray->size() << volume;
            for (int i = 0; i < m_byteArray->size(); i++) {
                newByteArray->append((int)m_byteArray->at(i) * 1.0);
                //newByteArray->append(m_byteArray->at(i) * volume);
            }

            m_buffer->close();
            delete m_buffer;
            m_buffer = new QBuffer(newByteArray);
            //m_buffer->setBuffer(newByteArray);
        }*/
#endif
    }
}

void Audio::playLoop()
{
    internalPlay(true);
}

void Audio::play()
{
    internalPlay();
}

void Audio::internalPlay(const bool &loop)
{
    // play in loop?
    if (m_loop != loop)
        m_loop = loop;

    // buffer wasn't intialized
    if (!m_buffer)
        return;

    // audioOutput created and playing
    if (m_audioOutput && m_loop) {
        m_buffer->open(QIODevice::ReadOnly);
        m_audioOutput->setBufferSize(m_buffer->size());///
        m_audioOutput->start(m_buffer);

        return;
    }

    // audioOutput created, but on 'pause'
    if (m_paused) {
        m_audioOutput->resume();
        m_paused = false;

        return;
    }

    // ok, jst play it
    m_audioOutput = new QAudioOutput(m_deviceInfo, m_audioFormat);

    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(evaluate(QAudio::State)));

    m_buffer->open(QIODevice::ReadOnly);
    m_audioOutput->start(m_buffer);
}

void Audio::pause()
{
    if (!m_audioOutput)
        return;

    if (m_paused) {
        m_audioOutput->resume();
        m_paused = false;

        return;
    }

    m_audioOutput->suspend();
    m_paused = true;

    /*if (m_audioOutput->state() == QAudio::ActiveState)
        m_audioOutput->suspend();
    else if (m_audioOutput->state() == QAudio::SuspendedState)
        m_audioOutput->resume();*/
}

void Audio::stop()
{
    if (!m_audioOutput)
        return;

    m_audioOutput->stop();
    m_buffer->close();
    m_paused = false;
}

void Audio::mute()
//void Audio::muted(const bool &muted)
{
    if (!m_audioOutput)
        return;

    if (m_muted) {
        // setVolume(0)?
        m_audioOutput->resume();
        m_muted = false;

        return;
    }

    // setVolume(oldVolume)?
    m_audioOutput->suspend();
    m_muted = true;
}

void Audio::evaluate(QAudio::State state)
{
    // TODO
    if (m_audioOutput)
        qDebug() << "::" << m_audioOutput->error();///

    if (state == QAudio::IdleState && m_loop) {
        internalPlay(m_loop);
    } else if (state == QAudio::IdleState) {
        stop();

        delete m_audioOutput;
        m_audioOutput = 0;
    }
}

void Audio::changeVolume(const qreal &volume)
{
    setVolume(volume);
}

void Audio::changePause()
{
    pause();
}

void Audio::changeStop()
{
    stop();
}

void Audio::changeMute()
{
    mute();
}
