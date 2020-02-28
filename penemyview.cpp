#include "penemyview.h"

PEnemyView::PEnemyView(std::shared_ptr<Enemy> model): EnemyView(model)
{
    setPixmap(QPixmap(":/resources/fireDemonIdle.png"));
    //setPos( model->getXPos(), model->getYPos());
    //setZValue(4);
    setScale(0.00369);
}

void PEnemyView::showDead()
{
    qDebug() << "PEnemyView::showDead()";
    setPixmap(QPixmap(":/resources/pic/fireDemonDead.png"));
}

void PEnemyView::showPoisoned(int)
{
    qDebug() << "PEnemyView::showPoisoned()";
    setPixmap(QPixmap(":/resources/fireDemonActive.png"));
}
