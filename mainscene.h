#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QObject>

class MainScene: public QGraphicsScene
{
    Q_OBJECT
public:
    MainScene(QObject* parent = 0);
    virtual ~MainScene() = default;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

signals:
    void mousePressed(int x, int y, bool mouse);
};


#endif // SCENE_H
