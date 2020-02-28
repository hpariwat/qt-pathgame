#ifndef PENEMYVIEW_H
#define PENEMYVIEW_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <memory>
#include "lib/world.h"
#include <QDebug>
#include "enemyview.h"

class PEnemyView : public EnemyView
{
    Q_OBJECT
public:
    PEnemyView(std::shared_ptr<Enemy> model);
    ~PEnemyView() {qDebug() << "PEnemyView Deleted...";}

private slots:
    void showDead();
    void showPoisoned(int);

};

#endif // PENEMYVIEW_H
