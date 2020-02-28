#include "healthcontrol.h"

HealthControl::HealthControl()
{
    qDebug() << "HealthPackController::HealthPackController()";
}

void HealthControl::addHealthpack(const std::shared_ptr<Tile> &h)
{
    auto healthpack = std::make_shared<Health>(h->getXPos(), h->getYPos(), h->getValue());
    healthpacks.push_back(healthpack);
    auto healthpackView = std::make_shared<HealthView>(QPixmap(":/resources/potion.png"), healthpack);
    connect(healthpack.get(), SIGNAL(collected()), healthpackView.get(), SLOT(collected()));
    healthpackViews.push_back(healthpackView);
}

void HealthControl::clearHealthpacks()
{
    qDebug() << "HealthPackController::clearHealthpacks()";
    healthpacks.clear();
    healthpackViews.clear();
}

std::vector<std::shared_ptr<Health>> HealthControl::getHealthpacks() const
{
    return healthpacks;
}

std::optional<std::shared_ptr<Health> > HealthControl::getHealthpackAt(int x, int y) const
{
    for (auto &healthpack : healthpacks)
    {
        if (x == healthpack->getXPos() && y == healthpack->getYPos())
        {
            return healthpack;
        }
    }
    return std::nullopt;
}

std::vector<std::shared_ptr<HealthView>> HealthControl::getHealthpackViews()
{
    return healthpackViews;
}
