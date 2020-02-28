#ifndef PROTAGONISTVIEW_H
#define PROTAGONISTVIEW_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "lib/world.h"
#include <QDebug>

class ProtaView : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ProtaView(QPixmap pixmap, std::shared_ptr<Protagonist> model);
    QString protaChar();
    ~ProtaView() {qDebug() << "ProtaView Deleted...";}

private slots:
    void posChanged(int x, int y);
};

#endif // PROTAGONISTVIEW_H
