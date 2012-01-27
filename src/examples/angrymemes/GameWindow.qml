import QtQuick 2.0
import QuasiGame 1.0

QuasiGame {
    id: game

    width: 800
    height: 600

    currentScene: scene

    Box2DScene {
        id: scene

        width: 2048
        height: 800

        viewport: Viewport {
            id: gameViewport

            property bool __moveRight: false
            property bool __moveLeft: false
            property bool __moveDown: false
            property bool __moveUp: false

            updateInterval: 10

            updateScript: {
                var yDiffTop = projectile.y - gameViewport.yOffset

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
            }
        ]
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            projectile.applyLinearImpulse(Qt.point(800* (game.mouse.x - projectile.x),
                                                   800* (game.mouse.y - projectile.y)),
                                          Qt.point(projectile.x, projectile.y))
        }
    }
}
