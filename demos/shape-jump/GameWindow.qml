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

    QuasiScriptBehavior {
        id: platformCreation

        script: {
            console.log("buga");
        }
    }

    QuasiPhysicsScene {
        id: scene

        //anchors.fill: parent
        width: parent.width
        height: parent.height

        /*viewport: QuasiViewport {
            id: gameViewport

            animationDuration: 1000
        }*/

        QuasiBody {
            id: character

            property int __horizontalImpulse: 20
            property int __verticalImpulse: 30
            property bool __jumping: false

            width: 50
            height: 50

            friction: 0.3
            density: 30
            //restitution: 0.6
            sleepingAllowed: false
            bullet: false

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
                            var impulse = Qt.point(0, y * -__verticalImpulse);
                            var point = Qt.point(x + width / 2, y + height / 2);

                            applyLinearImpulse(impulse, point);
                            __jumping = true;
                        }
                        event.accepted = true;
                        break;
                    }
                }
            }
        }

        onContact: {
            var cube = bodyA;
            var platform = bodyB;

            if (cube.__jumping) {
                cube.__jumping = false;
                platform.opacity = 0;
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
            platformObject.x = (game.width / 2) - platformObject.width / 2;
            platformObject.y = game.height - 50;

            platformObject = platform.createObject(scene);
            platformObject.x = (game.width / 2) - platformObject.width / 2;
            platformObject.y = game.height - 150;
        }
    }

    Component {
        id: platform

        QuasiBody {
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
                if (opacity == 0)
                    destroy();
            }
        }
    }
}
