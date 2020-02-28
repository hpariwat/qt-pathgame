#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include "enemyview.h"

class BossView : public EnemyView
{
    Q_OBJECT
public:
    BossView(std::shared_ptr<Enemy>);
    ~BossView() {qDebug() << "BossView Deleted...";}

private slots:
    void showDead();
};

#endif // BOSSVIEW_H
