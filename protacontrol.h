#ifndef PROTAGONISTCONTROLLER_H
#define PROTAGONISTCONTROLLER_H

#include <QObject>
#include <QDebug>
#include "lib/world.h"
#include <QMediaPlayer>
#include "protaview.h"


class ProtaControl : public QObject
{
    Q_OBJECT

public:
    ProtaControl();
    ~ProtaControl() {qDebug() << "ProtaControl Deleted...";}

    std::shared_ptr<Protagonist> getProta() const;
    std::shared_ptr<ProtaView> getProtaView() const;
    QString getProtaInfo();
    void setProta(std::shared_ptr<Protagonist> value);


private:
    std::shared_ptr<Protagonist> prota{nullptr};
    std::shared_ptr<ProtaView> protaView{nullptr};

signals:
    void currPos(int x, int y, QString symbol);
    void checkBoundary(int x, int y);

public slots:
    void moveProta(int k);
};

#endif // PROTAGONISTCONTROLLER_H
