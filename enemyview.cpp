#include "enemyview.h"

EnemyView::EnemyView(std::shared_ptr<Enemy> model)
{
    setPixmap(QPixmap(":/resources/enemy.png"));
    setPos(model->getXPos(), model->getYPos());
    setZValue(4);
    setScale(0.0119);
}

void EnemyView::showDead()
{
    qDebug() << "EnemyView::showDead()";
    setPixmap(QPixmap(":/resources/pic/enemyDead.png"));
}

