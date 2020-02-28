#ifndef FIREVIEW_H
#define FIREVIEW_H

#include "lib/world.h"
#include "QGraphicsPixmapItem"
#include <QDebug>

class FireView : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    FireView(std::shared_ptr<Tile> model);
    ~FireView() {qDebug() << "FireView Deleted...";}

private slots:
    void onFire(bool isOnFire);
    void fireLevel(float level);

signals:
    void remove();
};

#endif // FIREVIEW_H
