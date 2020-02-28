#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <iostream>
#include <QGraphicsItem>
#include "lib/world.h"
#include "enemyview.h"
#include "commandline.h"
//#include "maincontroller.h" // avoid circular dependency

#include <QLabel>
#include <QFileDialog>
#include <QGraphicsView>
#include "lib/world.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QMovie>
#include <QGraphicsProxyWidget>
#include <QPlainTextEdit>
#include <QVector>
#include <QList>
#include <QTextEdit>
#include <QLineEdit>
#include <QGraphicsSceneMouseEvent>
#include <QSlider>

#include "mainscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainView; }
QT_END_NAMESPACE

class MainController;
template<typename Vec>
QString getItemTextView(Vec &vectors, QString Type){
    QString result, xPos,yPos, strength;
    for(auto &vec : vectors){
        xPos.setNum(vec->getXPos());
        yPos.setNum(vec->getYPos());
        strength.setNum(vec->getValue());
        result += Type+" at position ("+xPos+","+yPos+") strength: "+strength +"\n";
    }
    return result;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    ~MainView();
    void setMainController(std::shared_ptr<MainController>);
    std::shared_ptr<MainController> getMainController();

    bool eventFilter(QObject *target, QEvent *event);

    void setMap(QString filepath);
    void fitInView() const;

    void loadGraphical();
    void loadTextMap();
    void loadTextMaze();
    void setupMap();

    bool getRep();

    QTextEdit * getTextView();

    std::shared_ptr<MainScene> getScene() const;

    void addPath(int x, int y);
    void clearPath();
    void refreshText(int y);


    void showMessage(QString string);
private:
    Ui::MainView *ui;

    std::shared_ptr<MainScene> scene{nullptr};
    QGraphicsPixmapItem *pixmap;
    QString filepath;
    bool display = true; // true for graphical view
    QVector<QString> mTileMap;
    QTextEdit * textViewMap;
    QTextEdit * textView;
    QLineEdit * textLine;
    std::vector<std::pair<std::shared_ptr<QGraphicsItem>,std::pair<int, int>>> pathway;

    std::shared_ptr<MainController> mainController;

    std::pair<int,int> clickPos;
    std::shared_ptr<QGraphicsItem> clickArea;

    QSlider * pspeedSlider;
    QSlider * hweightSlider;

    QString protaChar ="♛";
    QString blockedChar ="☒";
    QString passChar ="☐";
    std::vector<QString *> mTileMaze;
signals:
    void keyPressed(int k);
    void updateProta();
    void gameReady();
    void readLine(QString input);
private slots:
    void resizeEvent(QResizeEvent *event);
    //void showProta(int x, int y);
    void replaceTile(int xPos, int yPos, QString symbol);


    void changePage();

    void on_actionChange_view_triggered();

    void on_actionQuit_triggered();

    void mousePressed(int x, int y, bool mouse);


    void on_lineEdit_returnPressed();

    void on_actionAutopilot_triggered();

    void pspeedChanged(int s);
    void hweightChanged(int h);

    void on_actionRestart_triggered();

public slots:
    void refresh();
    void changeHealth(int health);
    void changeEnergy(int energy);
};
#endif // MAINVIEW_H
