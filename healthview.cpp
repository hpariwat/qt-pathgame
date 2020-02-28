#include "healthview.h"

HealthView::HealthView(QPixmap pixmap, std::shared_ptr<Health> model): QGraphicsPixmapItem(pixmap)
{
    setPos(model->getXPos(), model->getYPos());
    setScale(0.0037736);
    setZValue(3);
}



void HealthView::collected()
{
    setPixmap(QPixmap(":/resources/potion_used.png"));
}
