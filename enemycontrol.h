#ifndef ENEMYCONTROLLER_H
#define ENEMYCONTROLLER_H

#include <QDebug>
#include <optional>
#include "lib/world.h"
#include "enemyview.h"
#include "bossview.h"

class EnemyControl : public QObject
{
    Q_OBJECT
public:
    EnemyControl();
    ~EnemyControl() {qDebug() << "EnemyControl Deleted...";}
    void addEnemy(const std::shared_ptr<Enemy> &enemy);
    void clearEnemies();
    std::vector<std::shared_ptr<Enemy>> getEnemies() const;
    std::vector<std::shared_ptr<EnemyView>> getEnemyViews() const;
    std::optional<std::shared_ptr<Enemy>> getEnemyAt(int x, int y);
    void setMakeBoss(bool value);

    void setPeEnabled(bool value);

    void setXeEnabled(bool value);

private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<EnemyView>> enemyViews;
    bool makeBoss{false};
    bool peEnabled;
    bool xeEnabled;

signals:
    void currPos(int,int,QString);


};

#endif // ENEMYCONTROLLER_H
