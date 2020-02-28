#ifndef ENEMYVIEW_H
#define ENEMYVIEW_H

#include <QObject> // no need if include lib/world.h
#include <QGraphicsPixmapItem>
#include <memory>
#include <QDebug>
#include "lib/world.h"

class EnemyView : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    EnemyView(std::shared_ptr<Enemy> model);
    ~EnemyView() {qDebug() << "EnemyView Deleted...";}

private slots:
    void showDead();

};

#endif // ENEMYVIEW_H
