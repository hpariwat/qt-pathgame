#include "bomb.h"
#include <QTimer>
#include <QDebug>

Bomb::Bomb(int x, int y, float strength) : Enemy(x, y, strength)
{
    qsrand(time(nullptr));
    transform();
}

void Bomb::transform()
{
    if (getDefeated()) return;
    emit move();
    moveLeft--;

    QTimer::singleShot(100, this, SLOT(transform()));

}

void Bomb::setPos(int x, int y)
{
    setXPos(x);
    setYPos(y);
    emit changePos(x, y);
}

void Bomb::explode()
{
    setDefeated(true);
    emit dead();
}
