#include "boss.h"

Boss::Boss(int x, int y, float strength) : Enemy(x, y, strength)
{
    qsrand(time(nullptr));
    prepareBomb();
}

void Boss::prepareBomb()
{
    if (getDefeated()) return;
    emit sendBomb();
    int t = (qrand() % 2000);
    QTimer::singleShot(t + 500, this, SLOT(prepareBomb()));
}

void Boss::die()
{
    setDefeated(true);
    emit dead();
}
