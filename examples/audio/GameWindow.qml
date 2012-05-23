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

QuasiGame {
    id: game

    width: 401
    height: 250

    Button {
        id: sc

        anchors {
            top: parent.top
            left: parent.left
        }

        width: 100
        height: 80

        text: "single cuen"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                cuen.play();
            else if (mouse.button == Qt.RightButton)
                cuen.stop();
            else if (mouse.button == Qt.MiddleButton)
                cuen.pause();
        }
    }
    Button {
        id: lc

        anchors {
            top: parent.top
            left: sc.right
        }
        width: 100
        height: 80

        text: "loop cuen"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                cuen.playLoop();
            else if (mouse.button == Qt.RightButton)
                cuen.stop();
            else if (mouse.button == Qt.MiddleButton)
                cuen.pause();
        }
    }

    Button {
        id: st

        anchors {
            top: parent.top
            left: lc.right
        }

        width: 100
        height: 80

        text: "single tum"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                tumdumdum.play();
            else if (mouse.button == Qt.RightButton)
                tumdumdum.stop();
            else if (mouse.button == Qt.MiddleButton)
                tumdumdum.pause();
        }
    }
    Button {
        id: lt

        anchors {
            top: parent.top
            left: st.right
        }
        width: 100
        height: 80

        text: "loop tum"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                tumdumdum.playLoop();
            else if (mouse.button == Qt.RightButton)
                tumdumdum.stop();
            else if (mouse.button == Qt.MiddleButton)
                tumdumdum.pause();
        }
    }

    Button {
        id: volumeUp

        anchors {
            top: sc.bottom
            left: parent.left
        }
        width: 100
        height: 80

        text: "vol\n+"

        onReleased: {
            mixer.volume += 0.1;
        }
    }

    Button {
        id: sb

        anchors {
            top: lc.bottom
            left: volumeUp.right
        }
        width: 100
        height: 80

        text: "single bg"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                bg.play();
            else if (mouse.button == Qt.RightButton)
                bg.stop();
            else if (mouse.button == Qt.MiddleButton)
                bg.pause();
        }
    }
    Button {
        id: lb

        anchors {
            top: lc.bottom
            left: sb.right
        }
        width: 100
        height: 80

        text: "loop bg"

        onReleased: {
            if (mouse.button == Qt.LeftButton)
                bg.playLoop();
            else if (mouse.button == Qt.RightButton)
                bg.stop();
            else if (mouse.button == Qt.MiddleButton)
                bg.pause();
        }
    }

    Button {
        id: volumeDown

        anchors {
            top: lt.bottom
            left: lb.right
        }
        width: 100
        height: 80

        text: "vol\n-"

        onReleased: {
            mixer.volume -= 0.1;
        }
    }

    QuasiMixer {
        id: mixer
    }

    QuasiAudio {
        id: cuen

        source: ":/cuen.wav"
        mixer: mixer
    }

    QuasiAudio {
        id: tumdumdum

        source: ":/tumdumdum.wav"
        mixer: mixer
    }

    QuasiAudio {
        id: bg

        source: ":/bg.wav"
        mixer: mixer
    }

    Component.onCompleted: {
        console.log("\n\nInstructions:");
        console.log("\tLeftClick: play");
        console.log("\tMiddleClick: pause/unpause");
        console.log("\tRigthClick: stop");
    }
}
