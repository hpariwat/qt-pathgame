#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "lib/world.h"

class Boss : public Enemy
{
    Q_OBJECT
public:
    Boss(int x, int y, float strength);
    ~Boss() {qDebug() << "Boss Deleted...";}

    void die();

public slots:
    void prepareBomb();

signals:
    void sendBomb();
    void dead();
};

#endif // BOSS_H
