#ifndef HEALTHPACKCONTROLLER_H
#define HEALTHPACKCONTROLLER_H

#include <QDebug>
//#include "lib/world.h"
#include <QObject>
#include "health.h"
#include "healthview.h"

class HealthControl : public QObject
{
    Q_OBJECT
public:
    HealthControl();
    ~HealthControl() {qDebug() << "HealthControl Deleted...";}
    void addHealthpack(const std::shared_ptr<Tile> &healthpack);
    void clearHealthpacks();
    std::vector<std::shared_ptr<Health>> getHealthpacks() const;
    std::optional<std::shared_ptr<Health>> getHealthpackAt(int x, int y) const;

    std::vector<std::shared_ptr<HealthView>> getHealthpackViews();

private:
    std::vector<std::shared_ptr<Health>> healthpacks;
    std::vector<std::shared_ptr<HealthView>> healthpackViews;
signals:
    void currPos(int,int,QString );
};

#endif // HEALTHPACKCONTROLLER_H
