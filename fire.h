#ifndef FIRE_H
#define FIRE_H

#include "lib/world.h"
#include <QObject>
#include <QDebug>

class Fire : public QObject, public Tile
{
    Q_OBJECT
public:
    Fire(int x, int y, float value);
    ~Fire() {qDebug() << "FireModel Deleted... at" << getXPos() << getYPos();}

    bool getIsOnFire() const;
    void setIsOnFire(bool value);

    void setFireLevel(float level);

private:
    bool isOnFire = false;

signals:
    void onFire(bool isOnFire);
    void fireLevel(float level);
};

#endif // FIRE_H
