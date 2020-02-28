#include "enemycontrol.h"

#include "penemyview.h"
#include "bombview.h"
#include "maincontroller.h"

EnemyControl::EnemyControl()
{
    peEnabled = true;
    xeEnabled = true;
    qDebug() << "EnemyController::EnemyController()";
    qsrand(time(nullptr));
}

void EnemyControl::addEnemy(const std::shared_ptr<Enemy> &e)
{
    std::shared_ptr<Enemy> enemy = nullptr;
    std::shared_ptr<EnemyView> enemyView = nullptr;

    if (peEnabled && qobject_cast<PEnemy*>(e.get()))
    {
        enemy = std::make_shared<PEnemy>(e->getXPos(), e->getYPos(), e->getValue());
        enemyView = std::make_shared<PEnemyView>(enemy);
        connect(enemy.get(), SIGNAL(dead()), enemyView.get(), SLOT(showDead()));
        connect(enemy.get(), SIGNAL(poisonLevelUpdated(int)), enemyView.get(), SLOT(showPoisoned(int)));
    }
    else
    {
        if (xeEnabled && (!makeBoss))
        {
            makeBoss = true;
            enemy = std::make_shared<Boss>(e->getXPos(), e->getYPos(), e->getValue());
            enemyView = std::make_shared<BossView>(enemy);

            connect(enemy.get(), SIGNAL(dead()), enemyView.get(), SLOT(showDead()));
        }
        else
        {
            enemy = std::make_shared<Enemy>(e->getXPos(), e->getYPos(), e->getValue());
            enemyView = std::make_shared<EnemyView>(enemy);
            connect(enemy.get(), SIGNAL(dead()), enemyView.get(), SLOT(showDead()));
        }

    }

    enemies.push_back(enemy);
    enemyViews.push_back(enemyView);
}


void EnemyControl::clearEnemies()
{
    qDebug() << "EnemyController::clearEnemies()";
    enemies.clear();
    enemyViews.clear();
}

std::optional<std::shared_ptr<Enemy>> EnemyControl::getEnemyAt(int x, int y)
{
    for (auto &enemy : enemies) if (enemy->getXPos() == x && enemy->getYPos() == y) return enemy;
    return std::nullopt;
}

void EnemyControl::setMakeBoss(bool value)
{
    makeBoss = value;
}

void EnemyControl::setPeEnabled(bool value)
{
    peEnabled = value;
}

void EnemyControl::setXeEnabled(bool value)
{
    xeEnabled = value;
}


std::vector<std::shared_ptr<EnemyView>> EnemyControl::getEnemyViews() const
{
    return enemyViews;
}

std::vector<std::shared_ptr<Enemy>> EnemyControl::getEnemies() const
{
    return enemies;
}

