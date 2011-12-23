#include "gameitem.h"
#include "gamescene.h"

#include <QDeclarativeExpression>

GameItem::GameItem(GameScene *parent)
    : QQuickItem((QQuickItem *)parent)
    , m_expression(0)
    , m_updateInterval(0)
    , m_collided(false)
    , m_scene(0)
{
}

void GameItem::update(long delta)
{
    if ((m_updateInterval && m_updateTime.elapsed() >= m_updateInterval)
        || !m_updateInterval) {
        m_updateTime.restart();
        if (m_expression)
            m_expression->evaluate();
    }
}

QDeclarativeScriptString GameItem::updateScript() const
{
    return m_updateScript;
}

void GameItem::setUpdateScript(QDeclarativeScriptString updateScript)
{
    if (m_updateScript.script() != updateScript.script()) {
        m_updateScript = updateScript;

        if (m_expression)
            delete m_expression;

        m_expression = new QDeclarativeExpression(m_updateScript.context(), m_updateScript.scopeObject(), m_updateScript.script());

        emit updateScriptChanged();
    }
}

int GameItem::updateInterval() const
{
    return m_updateInterval;
}

void GameItem::setUpdateInterval(int updateInterval)
{
    if (m_updateInterval != updateInterval) {
        m_updateInterval = updateInterval;

        emit updateScriptChanged();
    }
}

bool GameItem::collided()
{
    return m_collided;
}

void GameItem::setCollided(bool collided)
{
    if (m_collided != collided) {
        m_collided = collided;

        emit collidedChanged();
    }
}

QList<QObject *> GameItem::collidedItems()
{
    GameScene *scene = qobject_cast<GameScene *>(parent());

    return scene->collidedItems(this);
}

GameScene *GameItem::scene()
{
    return m_scene;
}

void GameItem::setScene(GameScene *scene)
{
    m_scene = scene;
}
