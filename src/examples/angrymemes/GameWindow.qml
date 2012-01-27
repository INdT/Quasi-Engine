import QtQuick 2.0
import QuasiGame 1.0

QuasiGame {
    id: game

    width: 800
    height: 600

    currentScene: scene
    property int initialPos: 1500

    Box2DScene {
        id: scene

        width: 2048
        height: 800

        viewport: Viewport {
            id: gameViewport

            updateInterval: 10

            updateScript: {
                var yDiffTop = projectile.y - gameViewport.yOffset

                // XXX use projectile velocity
                if (Math.abs(yDiffTop) > 100)
                    gameViewport.vScroll(yDiffTop * 0.05)

                var yDiffBot = projectile.y - (gameViewport.yOffset + gameViewport.height)

                if (Math.abs(yDiffBot) > 100)
                    gameViewport.vScroll(yDiffBot * 0.05)

                var xDiff = projectile.x - gameViewport.xOffset

                if (Math.abs(xDiff) > 150)
                    gameViewport.hScroll(xDiff * 0.03)
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
                y: 500
                width: trollface.width
                height: trollface.height

                friction: 0.3
                density: 50
                restitution: 0.35
                sleepingAllowed: false
                bullet: true

                shape: Box2DItem.Circle

                Image {
                    id: trollface
                    source: "resources/trollface.png"
                }
            },
            Box2DItem {
                id: ground

                y: 750

                bodyType: Box2DItem.Static
                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                width: scene.width
                height: 50

                Rectangle {
                    anchors.fill: parent
                    color: "brown"
                }
            },
            Box2DItem {
                id: wood01

                x: initialPos
                y: ground.y - height
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: enemy01

                x: initialPos + wood01.width + 10
                y: ground.y - height
                width: enemy1.width
                height: enemy1.height

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                shape: Box2DItem.Circle

                Image {
                    id: enemy1
                    source: "resources/enemy.png"
                }
            },
            Box2DItem {
                id: wood02

                x: initialPos + wood01.width + enemy01.width + 20
                y: ground.y - height
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: wood011

                x: initialPos + 55
                y: ground.y - (1.6 * height)
                width: 15
                height: 110
                rotation: 90

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: wood022

                x: initialPos + 175
                y: ground.y - (1.6 * height)
                width: 15
                height: 110
                rotation: 90

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: enemy02

                x: initialPos + wood01.width + enemy01.width + wood02.width + 30
                y: ground.y - height
                width: enemy2.width
                height: enemy2.height

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                shape: Box2DItem.Circle

                Image {
                    id: enemy2
                    source: "resources/enemy.png"
                }
            },
            Box2DItem {
                id: wood03

                x: initialPos + wood01.width + enemy01.width + wood02.width + enemy02.width + 40
                y: ground.y - height
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: wood04

                x: initialPos + 60
                y: 500
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: wood044

                x: initialPos + 115
                y: 430
                width: 15
                height: 110
                rotation: 90

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: enemy03

                x: initialPos + wood04.width + 60
                y: ground.y - height - 300
                width: enemy3.width
                height: enemy3.height

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                shape: Box2DItem.Circle

                Image {
                    id: enemy3
                    source: "resources/enemy.png"
                }
            },
            Box2DItem {
                id: wood05

                x: initialPos + wood04.width + enemy03.width + 70
                y: 500
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },

            // forever alone
            Box2DItem {
                id: woodf

                x: initialPos + 400
                y: ground.y - height
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: enemyf

                x: initialPos + 400 + woodf.width + 10
                y: ground.y - height
                width: enemyfa.width
                height: enemyfa.height

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                shape: Box2DItem.Circle

                Image {
                    id: enemyfa
                    source: "resources/alone.png"
                }
            },
            Box2DItem {
                id: woodf2

                x: initialPos + 400 + woodf.width + enemyf.width + 10
                y: ground.y - height
                width: 15
                height: 110

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            },
            Box2DItem {
                id: woodff

                x: initialPos + 450
                y: ground.y - (1.6 * height)
                width: 15
                height: 110
                rotation: 90

                friction: 0.3
                density: 50
                restitution: 0.3
                sleepingAllowed: false

                Image {
                    source: "resources/wood.jpg"
                    anchors.fill: parent
                }
            }
        ]
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            var xLaunch = 800 * (game.mouse.x - projectile.x);
            var yLaunch = 800 * (game.mouse.y - projectile.y);

            xLaunch = xLaunch > 150000 ? 150000 : xLaunch
            yLaunch = yLaunch < -150000 ? -150000 : yLaunch

            print (xLaunch, yLaunch)

            projectile.applyLinearImpulse(Qt.point(xLaunch, yLaunch),
                                          Qt.point(projectile.x + projectile.width / 2.0, projectile.y + projectile.height / 2.0)) // XXX expose b2pos
        }
    }
}
