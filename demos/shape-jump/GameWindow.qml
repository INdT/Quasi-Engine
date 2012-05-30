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

    width: 400
    height: 800

    currentScene: scene

    QuasiPhysicsScene {
        id: scene

        anchors.fill: parent
        //width: parent.width
        //height: parent.height

        viewport: QuasiViewport {
            id: gameViewport

            //animationDuration: 1000
        }


        QuasiEntity {
            id: god

            property int count: 0
            property int __max: 10
            property int deltaY: 100
            property int __latestX: 0
            property int __latestY: 0

            function generatePos(minX, minY, maxX, maxY)
            {
                var x = Math.floor(Math.random() * (maxX - minX + 1) + minX);
                var y = Math.floor(Math.random() * (maxY - minY + 1) + minY);

                return [x, y];
            }

            behavior: QuasiScriptBehavior {
                script: {
                    if (god.count < god.__max) {
                        var platformObject;
                        platformObject = platform.createObject(scene);

                        var pos = god.generatePos(0, god.__latestY - (god.deltaY * 1.5),
                                                 game.width - platformObject.width,
                                                 god.__latestY - (god.deltaY / 2));

                        platformObject.x = pos[0];
                        platformObject.y = pos[1];
                        platformObject.z = 0;

                        god.__latestX = platformObject.x;
                        //god.__latestY -= god.deltaY;
                        god.__latestY = platformObject.y;
                        god.count++;
                    }
                }
            }
        }

        QuasiBody {
            id: character

            property int __horizontalImpulse: 20
            property int __verticalImpulse: 30
            property bool __jumping: false

            width: 50
            height: width

            friction: 0.3
            density: 30
            //restitution: 0.6
            sleepingAllowed: false
            bullet: false
            z: 1

            shapeGeometry: Quasi.RectangleBodyShape
            //behavior: script

            Rectangle {
                color: "white"
                border.color: "black"
                border.width: 5
                anchors.fill: parent
            }

            x: (game.width / 2) - width / 2
            y: game.height - 100

            focus: true
            Keys.onPressed: {
                switch (event.key) {
                    case Qt.Key_A:
                    case Qt.Key_Left:
                        var impulse = Qt.point(x * -__horizontalImpulse, 0);
                        var point = Qt.point(x + width / 2, y + height / 2);

                        applyLinearImpulse(impulse, point);
                        event.accepted = true;
                        break;
                    case Qt.Key_D:
                    case Qt.Key_Right:
                        var impulse = Qt.point(x * __horizontalImpulse, 0);
                        var point = Qt.point(x + width / 2, y + height / 2);

                        applyLinearImpulse(impulse, point);
                        event.accepted = true;
                        break;
                    case Qt.Key_W:
                    case Qt.Key_Up: {
                        if (!__jumping) {
                            __jumping = true;
                            var impulse = Qt.point(0, y * -__verticalImpulse);
                            var point = Qt.point(x + width / 2, y + height / 2);

                            applyLinearImpulse(impulse, point);
                        }
                        event.accepted = true;
                        break;
                    }
                }
            }

            onXChanged: {
                // TODO fix logic
                /*if (x <= gameViewport.xOffset)
                    gameViewport.hScroll(gameViewport.xOffset - (game.width / 2));
                else if (x + width >= gameViewport.xOffset + game.width)
                    gameViewport.hScroll(gameViewport.xOffset + (game.width / 2));*/
            }
        }

        onContact: {
            var cube = bodyA;
            var platform = bodyB;

            if (cube.__jumping) {
                cube.__jumping = false;
                //platform.opacity = 0; // XXX
            }
        }

        onPreContact: {
            var cube = bodyA;
            var platform = bodyB;
            var bottom = cube.y + cube.height;

            if (bottom > platform.y)
                contact.enabled = false;
        }

        Component.onCompleted: {
            var platformObject;

            platformObject = platform.createObject(scene);
            //platformObject.x = game.width - (game.width);// - platformObject.width);
            //platformObject.y = game.height - (game.height);// - platformObject.height);
            platformObject.x = (game.width / 2) - platformObject.width / 2;
            platformObject.y = game.height - 50;
            platformObject.z = 0;

            god.__latestX = platformObject.x;
            god.__latestY = platformObject.y;
            console.log(platformObject.y);
            god.count++;
        }
    }

    Component {
        id: platform

        QuasiBody {
            id: platformBody

            width: 80
            height: 20

            friction: 0.3
            density: 50
            restitution: 0.6
            sleepingAllowed: false

            bodyType: Quasi.StaticBodyType
            shapeGeometry: Quasi.RectangleBodyShape

            Rectangle {
                color: "white"
                border.color: "black"
                border.width: 3
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 2000
                }
            }

            onOpacityChanged: {
                if (opacity == 0) {
                    platformBody.destroy();
                    god.count--;
                }
            }
        }
    }
}
