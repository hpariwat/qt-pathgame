#ifndef HEALTHPACK_H
#define HEALTHPACK_H

#include "lib/world.h"
#include <QObject>
#include <QDebug>

class Health : public QObject, public Tile
{
    Q_OBJECT
public:
    Health(int x, int y, int energy);
    ~Health() {qDebug() << "Health Deleted...";}

    bool getDefeated() const;
    void setEmpty(bool value);

private:
    bool empty{false};

signals:
    void collected();
};

#endif // HEALTHPACK_H
