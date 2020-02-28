#include "fireview.h"
#include <QDebug>

FireView::FireView(std::shared_ptr<Tile> model)
{
    qDebug() << "FireView" << model->getXPos() << model->getYPos();
    setPixmap(QPixmap(":/resources/fire.png"));
    setPos(model->getXPos(), model->getYPos());
    setZValue(3);
}

void FireView::onFire(bool isOnFire)
{
    if (isOnFire) {
        setScale(0.0064516);
        setPixmap(QPixmap(":/resources/fire.png"));
    }
    else {
        emit remove();
    }
}

void FireView::fireLevel(float level)
{
    //qDebug() << "FireView::fireLevel" << level;
    setOpacity(level);
}
