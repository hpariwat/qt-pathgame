#include "mainscene.h"

MainScene::MainScene(QObject *parent): QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::black);
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    qDebug() << event->scenePos().x() << event->scenePos().y();
    qDebug() << floor(event->scenePos().x()) << floor(event->scenePos().y());

    if (event->buttons() & Qt::LeftButton) emit mousePressed(floor(event->scenePos().x()),floor(event->scenePos().y()),0);
    else if (event->buttons() & Qt::RightButton) emit mousePressed(floor(event->scenePos().x()),floor(event->scenePos().y()),1);

}

