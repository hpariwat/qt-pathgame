#include "tilecontrol.h"

TileControl::TileControl()
{
    qDebug() << "TileController::TileController()";
}

void TileControl::addTile(const std::shared_ptr<Tile> &t)
{
    tiles.push_back(t);
}

std::vector<std::shared_ptr<Tile>> TileControl::getTiles()
{
    return tiles;
}

void TileControl::clearTiles()
{
    tiles.clear();
    tileViews.clear();
}

QString TileControl::getTileChar(bool blocked)
{
    QString symbol;
    (blocked)? symbol = "☒ ": symbol = "☐ ";
    return symbol;
}

QString TileControl::getHealthChar(bool empty)
{
    QString symbol;
    (empty)? symbol="♡ " : symbol = "♥ ";
    return symbol;
}

QString TileControl::getEnemyChar(TileControl::enemyType type)
{
    QString symbol;
    if(type == Penemy) return symbol  ="☢ ";
    if(type == Xenemy) return symbol  ="💣 ";
    if(type == Enemy)  return  symbol ="☠ ";
}

std::optional<std::shared_ptr<Tile>> TileControl::getTileAt(int x, int y)
{
    //qDebug() << "query at" << (y * xBound + x);
    auto pos = y * xBound + x;
    if (pos < 0 || pos >= (xBound * yBound)) return std::nullopt;
    return tiles.at(pos);

}

void TileControl::setBoundary(const int &x, const int &y)
{
    xBound = x;
    yBound = y;
}

std::vector<std::shared_ptr<FireView> > TileControl::getTileViews()
{
    return tileViews;
}
