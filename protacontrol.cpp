#include "protacontrol.h"

ProtaControl::ProtaControl()
{
    qDebug() << "ProtagonistController::ProtagonistController()";

}

void ProtaControl::moveProta(int key)
{
    qDebug() << "ProtagonistController::moveProta(" << key << ")";
    int xPos = prota->getXPos();
    int yPos = prota->getYPos();
    emit currPos(xPos, yPos,"☐ "); // prota position before move
    switch(key)
    {
        case 1: yPos--; break;
        case 2: xPos--; break;
        case 3: yPos++; break;
        case 4: xPos++; break;
    }

    emit checkBoundary(xPos, yPos);

}


std::shared_ptr<Protagonist> ProtaControl::getProta() const { return prota; }

std::shared_ptr<ProtaView> ProtaControl::getProtaView() const
{
    return protaView;
}

QString ProtaControl::getProtaInfo()
{
    QString result,xPos,yPos;
    auto checkValue = [](auto a) {
        if(a>100) return a = 100;
        else if(a<0)   return a = 0;
        else return a;
    };
    int energy = int(checkValue(prota->getEnergy()));
    int health = int(checkValue(prota->getHealth()));
    xPos.setNum(prota->getXPos());
    yPos.setNum(prota->getYPos());
    result ="Prota at position(" + xPos +"," +yPos+ ") health= " + QString().setNum(health) + " energy= "+ QString().setNum(energy);
    return result;
}

void ProtaControl::setProta(std::shared_ptr<Protagonist> value)
{
    prota = std::move(value);
    protaView = std::make_shared<ProtaView>(QPixmap(":/resources/prota.png"), prota);
}
