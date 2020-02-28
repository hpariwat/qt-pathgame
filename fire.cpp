#include "fire.h"

Fire::Fire(int x, int y, float value) : Tile(x, y, value)
{
    qDebug() << "creating fire model with" << x << y << value;
}

bool Fire::getIsOnFire() const
{
    return isOnFire;
}

void Fire::setIsOnFire(bool value)
{
    isOnFire = value;
    emit onFire(isOnFire);
}

void Fire::setFireLevel(float level)
{
    emit fireLevel(level);
}
