#include "protaview.h"

ProtaView::ProtaView(QPixmap pixmap, std::shared_ptr<Protagonist> model) : QGraphicsPixmapItem{pixmap}
{
    setPos(0,0); // z-layer start at 1
    setZValue(10);
    setScale(0.000833);
}

QString ProtaView::protaChar()
{
    return QString("♛ ");
}

void ProtaView::posChanged(int x, int y)
{
    setPos(x,y);
}
