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

    width: 500
    height: 800

    currentScene: scene

    QuasiPhysicsScene {
        id: scene

        width: parent.width
        height: parent.height

        gravity: Qt.point(0,0)


        QuasiBody {
            id: hockeyBall

            width: 50
            height: 50

            friction: 0.3
            density: 5
            restitution: 0.6
            sleepingAllowed: false
            bullet: true
            shapeGeometry: Quasi.CircleBodyShape

            Rectangle {
                anchors.fill: parent
                color: "black"
                radius: 180
            }

            x: (scene.width - hockeyBall.width) / 2.0
            y: (scene.height - hockeyBall.height) / 2.0
        }

        QuasiBody {
            id: playerMallet

            width: 100
            height: 100

            friction: 0.3
            density: 5
            restitution: 0.6
            sleepingAllowed: false
            shapeGeometry: Quasi.CircleBodyShape

            Image {
                source: ":/mallet.png"
            }

            x: (scene.width - playerMallet.width) / 2
            y: scene.height - playerMallet.height * 2
        }

        MouseArea {
            id: playerMouseArea
            anchors.fill: parent

            onPressed: playerMouseJoint.maxForce = 15000
            onReleased: {
                playerMouseJoint.maxForce = 0
                playerMallet.setLinearVelocity(Qt.point(0, 0))
            }
        }

        QuasiMouseJoint {
            id: playerMouseJoint
            target: playerMallet
            maxForce: 0
        }

        QuasiBody {
            id: enemyMallet

            width: 100
            height: 100

            friction: 0.3
            density: 5
            restitution: 0.6
            sleepingAllowed: false
            shapeGeometry: Quasi.CircleBodyShape

            Image {
                source: ":/mallet.png"
            }

            x: (scene.width - enemyMallet.width) / 2
            y: enemyMallet.height
        }

        /*QuasiScriptBehavior {
            
        }*/

        QuasiBody {
            id: leftWall
            bodyType: Quasi.StaticBodyType

            friction: 0.3
            density: 5
            restitution: 0.6

            width: 5
            height: scene.height

            Rectangle {
                anchors.fill: parent
                color: "black"
            }

            x: 0
            y: 0
        }

        QuasiBody {
            id: rightWall
            bodyType: Quasi.StaticBodyType

            friction: 0.3
            density: 5
            restitution: 0.6

            width: 5
            height: scene.height

            Rectangle {
                anchors.fill: parent
                color: "black"
            }

            x: scene.width - 5
            y: 0
        }

        QuasiBody {
            id: topWall

            bodyType: Quasi.StaticBodyType

            friction: 0.3
            density: 5
            restitution: 0.6

            width: scene.width - 10
            height: 5

            Rectangle {
                anchors.fill: parent
                color: "black"
            }

            x: 5
            y: 0
        }

        QuasiBody {
            id: bottomWall

            bodyType: Quasi.StaticBodyType

            friction: 0.3
            density: 5
            restitution: 0.6

            width: scene.width - 10
            height: 5

            Rectangle {
                anchors.fill: parent
                color: "black"
            }

            x: 5
            y: scene.height - 5
        }

        Rectangle {
            id: centerLine
            width: scene.width
            height: 5
            color: "black"

            anchors.verticalCenter: scene.verticalCenter
        }
    }
}
