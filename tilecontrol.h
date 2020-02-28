#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include <QDebug>
#include "lib/world.h"
#include "fireview.h"

class TileControl : public QObject
{
    Q_OBJECT
public:
    TileControl();
    ~TileControl() {qDebug() << "TileControl Deleted...";}
    enum enemyType { Enemy, Penemy, Xenemy };
    void addTile(const std::shared_ptr<Tile> &tile);
    std::vector<std::shared_ptr<Tile>> getTiles();
    void clearTiles();
    QString getTileChar(bool blocked);
    QString getHealthChar(bool empty);
    QString getEnemyChar(enemyType type);
    std::vector<std::shared_ptr<FireView>> getTileViews();
    std::optional<std::shared_ptr<Tile> > getTileAt(int x, int y);
    void setBoundary(const int &x, const int &y);
private:
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<FireView>> tileViews;
    int xBound{0};
    int yBound{0};

};

#endif // TILECONTROLLER_H
