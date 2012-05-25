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
    id: panel

    width: 401
    height: 241

    Grid {
        id: grid

        columns: 4
        rows: 3

        Button {
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

            text: "vol\n+"

            onReleased: {
                mixer.volume += 0.1;
            }
        }

        Button {
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

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
            width: panel.width / grid.columns
            height: panel.height / grid.rows

            text: "vol\n-"

            onReleased: {
                mixer.volume -= 0.1;
            }
        }
        Button {
            width: panel.width / grid.columns
            height: panel.height / grid.rows

            text: "mixer\npause"

            onReleased: {
                mixer.pause();
            }
        }
        Button {
            width: panel.width / grid.columns
            height: panel.height / grid.rows

            text: "mixer\nstop"

            onReleased: {
                mixer.stop();
            }
        }
        Button {
            width: panel.width / grid.columns
            height: panel.height / grid.rows

            text: "mixer\nmute"

            onReleased: {
                mixer.mute();
            }
        }
    }

    QuasiMixer {
        id: mixer
    }

    QuasiAudio {
        id: cuen

        source: ":/sounds/cuen.wav"
        mixer: mixer
    }

    QuasiAudio {
        id: tumdumdum

        source: ":/sounds/tumdumdum.wav"
        mixer: mixer
    }

    QuasiAudio {
        id: bg

        source: ":/sounds/bg.wav"
        mixer: mixer
    }

    Component.onCompleted: {
        console.log("\n\nInstructions:");
        console.log("\tLeftClick: play");
        console.log("\tMiddleClick: pause/unpause");
        console.log("\tRigthClick: stop");
    }
}
