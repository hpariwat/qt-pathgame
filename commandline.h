#ifndef CLI_H
#define CLI_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTextStream>
#include <QMetaEnum>

#include "lib/world.h"
#include <QTextEdit>

class MainController;
class MainView;

class CommandLine: public QObject
{
    Q_OBJECT
public:
    CommandLine();
    std::pair<int,bool>  compare(QString input, QString function);
    void help( );
    void enemyTextView( );
    void healthpackTextView( );
    void error_handler(QString input);
    void refreshText(int y);
    void getNearestHealthpack();
    void getNearestEnemy();
    void moveto(QString input);
    void setMainController(std::shared_ptr<MainController>);
    void setMainView(std::shared_ptr<MainView> mainView);
    void setMainView();
    void showAll();
    QTextEdit* getTextView();

signals:


public slots:
    void commandlineOutput(QString input);
private:
    std::shared_ptr<MainView> mainView;
    std::shared_ptr<MainController> mainController;

};

#endif // CLI_H
