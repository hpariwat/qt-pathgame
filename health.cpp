#include "health.h"

Health::Health(int x, int y, int energy) : Tile(x, y, energy)
{

}

bool Health::getDefeated() const
{
    return empty;
}

void Health::setEmpty(bool value)
{
    empty = value;
    if (empty == true) setValue(0);
    emit collected();
}
