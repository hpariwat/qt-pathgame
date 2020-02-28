#ifndef XENEMYVIEW_H
#define XENEMYVIEW_H


#include "enemyview.h"

class BombView : public EnemyView
{
    Q_OBJECT
public:
    BombView(std::shared_ptr<Enemy> model);
    ~BombView() {qDebug() << "BombView Deleted...";}

private:
    float fadeLevel = 1;
    int rotation = 0;
    int rotateSpeed = 0;

private slots:
    void showDead();
    void changePos(int x, int y);
    void fadeExplosion();

signals:
    void remove();
};

#endif // XENEMYVIEW_H
