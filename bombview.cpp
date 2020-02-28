#include "bombview.h"
#include <QTimer>

BombView::BombView(std::shared_ptr<Enemy> model): EnemyView(model)
{
    setPixmap(QPixmap(":/resources/pic/bomb.png"));
    setScale(0.05);
    qsrand(time(nullptr));
    rotateSpeed = (qrand() % 15) + 5;
}

void BombView::showDead()
{
    qDebug() << "XEnemyView::showDead()";
    setScale(0.2); // was 0.05
    setOffset(-6, -6); // was -8
    setPixmap(QPixmap(":/resources/pic/bombexplode.png"));
    fadeExplosion();
}

void BombView::changePos(int x, int y)
{
    setPos(x, y);
    setRotation(rotation);
    rotation += rotateSpeed;

}

void BombView::fadeExplosion()
{
    if (fadeLevel <= 0 ) {
        emit remove();
        return;
    }
    fadeLevel -= 0.2;
    setOpacity(fadeLevel);
    QTimer::singleShot(200, this, SLOT(fadeExplosion()));
}
