#include "bossview.h"

BossView::BossView(std::shared_ptr<Enemy> model) : EnemyView(model)
{
    setPixmap(QPixmap(":/resources/pic/lyon.png"));
    setPos(model->getXPos(), model->getYPos());
    setZValue(4);
    setScale(0.01389);
}

void BossView::showDead()
{
    qDebug() << "Boss::showDead()";
    setPixmap(QPixmap(":/resources/pic/bombexplode.png"));
}
