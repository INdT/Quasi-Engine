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
