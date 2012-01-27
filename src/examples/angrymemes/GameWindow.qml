import QtQuick 2.0
import QuasiGame 1.0

QuasiGame {
    id: game

    width: 800
    height: 601

    currentScene: scene

    Box2DScene {
        id: scene

        width: 1594
        height: 601

        viewport: Viewport {
            id: gameViewport

            property bool __moveRight: false
            property bool __moveLeft: false
            property bool __moveDown: false
            property bool __moveUp: false

            updateInterval: 10
            yOffset: 77

            focus: true

            Keys.onPressed: {

                switch (event.key) {
                    case Qt.Key_Right:
                        __moveRight = true
                    break;

                    case Qt.Key_Left:
                        __moveLeft = true
                    break;

                    case Qt.Key_Down:
                        __moveDown = true
                    break;

                    case Qt.Key_Up:
                        __moveUp = true
                    break;
               }
               event.accepted = true;
            }

            Keys.onReleased: {
                switch (event.key) {
                    case Qt.Key_Right:
                        __moveRight = false
                    break;

                    case Qt.Key_Left:
                        __moveLeft = false
                    break;

                    case Qt.Key_Down:
                        __moveDown = false
                    break;

                    case Qt.Key_Up:
                        __moveUp = false
                    break;
               }
               event.accepted = true;
            }

            updateScript: {
                if (__moveRight) {
                    gameViewport.hScroll(5)
                } else if (__moveLeft) {
                    gameViewport.hScroll(-5)
                } else if (__moveDown) {
                    gameViewport.vScroll(5)
                } else if (__moveUp) {
                    gameViewport.vScroll(-5)
                }
            }
        }

        Image {
            id: background
            source: "resources/background.png"
        }

        gameItems: [
            Box2DItem {
                id: projectile
                x: 100
                width: 10
                height: 10

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false
                bullet: true

                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            },
            Box2DItem {
                id: ground

                y: 440

                bodyType: Box2DItem.Static
                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                width: 1594
                height: 10
            },
            Box2DItem {
                id: mouseItem

                width: 60
                height: 60

                friction: 0.3
                density: 50
                restitution: 0.6
                sleepingAllowed: false

                Rectangle {
                    color: "green"
                    anchors.fill: parent
                }

                x: 0
                y: 160
            },
            Box2DMouseJointItem {
                target: mouseItem
            }
        ]
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            projectile.applyLinearImpulse(Qt.point(10 * (game.mouse.x - projectile.x),
                                                   10 * (game.mouse.y - projectile.y)),
                                          Qt.point(projectile.x, projectile.y))
        }
    }
}
