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
#include <QtCore/qmath.h>
#include <QDebug>///

#include "audio.h"

Audio::Audio(QObject *parent)
    : QObject(parent)
    , m_mixer(0)
    , m_audioOutput(0)
    , m_volume(1.0)
    , m_loop(false)
    , m_paused(false)
{
    // XXX take a look on QSoundEffect to use instead of QAudioOutput
    // ^^^ QSoundEffects is just available on Qt5. Cuen.
    m_audioFormat.setCodec("audio/pcm");

    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setSampleType(QAudioFormat::SignedInt);

    m_deviceInfo = QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice());
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

        // initialize audio format
        // based on the code from WavFile, from Expresso lib
        // http://gitorious.org/expresso/expresso
        struct header {
            char chunkId[4];
            quint32 chunkSIze;
            char format[4];
            char subChunk1Id[4];
            quint32 subChunk1Size;
            qint16 audioFormat;
            quint16 numChannels;
            quint32 sampleRate;
            quint32 byteRate;
            quint16 blockAlign;
            quint16 bitsPerSample;
            char subChunk2Id[4];
            quint32 subChunk2Size;
        } wavHeader;

        const int wavHeaderSize = sizeof(struct header);
        if (inputFile.read(reinterpret_cast<char *>(&wavHeader), wavHeaderSize) != wavHeaderSize)
            // XXX show error loading wav file
            qDebug() << "FIAL!";

        const QByteArray cid(wavHeader.chunkId, 4);
        const QByteArray format(wavHeader.format, 4);
        const QByteArray subCid(wavHeader.subChunk1Id, 4);

        const bool validChunk = (wavHeader.subChunk1Size == 16 && wavHeader.audioFormat == 1) ||
                                (wavHeader.subChunk1Size == 40 && wavHeader.audioFormat == -2);

        if (cid != "RIFF" || format != "WAVE" || subCid != "fmt " || !validChunk) {
            qDebug() << "CUEN";
            // XXX show error loading sound format
        }
        // XXX RIFX for big-endian?

        if (wavHeader.bitsPerSample == 0)
            // XXX show error loading sound format
            qDebug() << "FUUUUU";

        m_audioFormat.setSampleSize(wavHeader.bitsPerSample);
        m_audioFormat.setChannels(wavHeader.numChannels);
        m_audioFormat.setSampleRate(wavHeader.sampleRate);

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

void Audio::setVolume(const qreal &volume, const bool &store)
{
    qreal newVolume = qBound(0.0, volume, 1.0);

    if (store && (m_volume != newVolume))
        m_volume = newVolume;

#if QT_VERSION >= 0x050000
    m_audioOutput->setVolume(newVolume);
#else
    // TODO there are noise when calculating volume
    // TODO memory consumption when decreasing/increasing volume
    if (m_byteArray && newVolume != 1.0) {
        QByteArray *newByteArray = new QByteArray();
        int i;
        //qreal multiplier = qTan(newVolume);

        for (i = 0; i < m_byteArray->size(); i++) {
            int newData = static_cast<qint16>(m_byteArray->at(i) * newVolume);
            newByteArray->append(newData);
        }

        m_buffer = new QBuffer(newByteArray);
    } else if (newVolume == 1.0)
        m_buffer = new QBuffer(m_byteArray);
#endif
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
        //m_audioOutput->setBufferSize(m_buffer->size());///
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
    stop();
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
}

void Audio::stop()
{
    if (!m_audioOutput)
        return;

    m_audioOutput->stop();
    m_buffer->close();
    m_paused = false;

    delete m_audioOutput;
    m_audioOutput = 0;
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
    // we need to multiplex local volume with the master one
    qreal realVolume = volume * m_volume;
    setVolume(realVolume, false);
}

void Audio::changePause()
{
    pause();
}

void Audio::changeStop()
{
    stop();
}
