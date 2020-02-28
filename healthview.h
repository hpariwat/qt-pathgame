#ifndef HEALTHPACKVIEW_H
#define HEALTHPACKVIEW_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <memory>
#include "health.h"
#include <QDebug>

class HealthView : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    HealthView(QPixmap pixmap, std::shared_ptr<Health> model);
    ~HealthView() {qDebug() << "HealthView Deleted...";}

private slots:
    void collected();
};

#endif // HEALTHPACKVIEW_H
