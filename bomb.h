#ifndef XENEMY_H
#define XENEMY_H

#include "lib/world.h"
#include <QDebug>

class Bomb : public Enemy
{
    Q_OBJECT
public:
    Bomb(int x, int y, float strength);
    ~Bomb() {qDebug() << "Bomb Deleted...";}

    void setPos(int x, int y);

    void explode();


private:
    int moveLeft = 20;

public slots:
    void transform();



signals:
    void dead();
    void move();
    void changePos(int, int);
};

#endif // XENEMY_H
