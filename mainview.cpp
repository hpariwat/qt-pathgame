#include "mainview.h"
#include "ui_mainview.h"
#include "maincontroller.h"
#include "commandline.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <QLabel>
#include <QFileDialog>
#include <QGraphicsView>
#include "lib/world.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <QMovie>
#include <QGraphicsProxyWidget>
#include <QProcess>
#include <limits>
#include <QRegularExpression>
#include <QMessageBox>

#include "enemyview.h"

MainView::MainView(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainView)
{
    qDebug() << "MainView::MainView()";
    ui->setupUi(this);

    scene = std::make_shared<MainScene>(this);
    installEventFilter(this); // for keyboard input
    textViewMap = ui->textDisplay;
    textView = ui->textLineDisplay;
    textLine = ui->lineEdit;
    textLine->setPlaceholderText("Write your command line here");
    textView->setReadOnly(true);
    textViewMap->setReadOnly(true);
    textViewMap->setLineWrapMode(QTextEdit::NoWrap);
    auto healthBar = ui->healthBar;
    QPalette p = healthBar->palette();
    p.setColor(QPalette::Highlight, Qt::red);
    healthBar->setPalette(p);

    pspeedSlider = ui->protaSpeed;
    hweightSlider = ui->heuWeight;

}

MainView::~MainView()
{
    qDebug() << "MainView Deleted...";
    delete ui;
}

void MainView::addPath(int x, int y)
{
   auto path = std::shared_ptr<QGraphicsItem>(scene->addRect(0, 0, 1, 1, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255))));
   path->setZValue(1);
   if (mainController->getPathMode())
   {
       path->setOpacity(1);
       path->setScale(5);
   }
   else
   {
       path->setOpacity(0.5);
       path->setScale(1);
   }

   path->setPos(x,y);
   pathway.push_back(std::pair(path,std::pair(x,y)));
}

void MainView::clearPath()
{
    if (!mainController->getPathMode())
    {
        for(auto &path : pathway)
        {
            replaceTile(path.second.first,path.second.second,mainController->getTileController().getTileChar(false));
        }
    }
    pathway.clear();
    refresh();
}


/*toggle button function to switch page*/
void MainView::changePage(){
    if(display){
        display = false;
        ui->stackedWidget->setCurrentIndex(1); //index 1 = textView
        if(mainController->getPathMode()) loadTextMaze();
        else loadTextMap();
    }
    else{
        display = true;
        ui->stackedWidget->setCurrentIndex(0); // index 0 = graphical view
        refresh();
    }
}

void MainView::loadGraphical()
{
    changeEnergy(100);
    changeHealth(100);
    clickPos = std::make_pair(0,0);
    clickArea = std::shared_ptr<QGraphicsItem>(scene->addRect(0, 0, 1, 1, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 255, 255))));
    clickArea->setZValue(1);
    clickArea->setOpacity(0.5);
    clickArea->setScale(1);
    clickArea->setPos(0,0);

    scene->addItem(mainController->getProtagonistController().getProtaView().get());

    if (!mainController->getPathMode())
    {
        for (auto &healthpackView : mainController->getHealthPackController().getHealthpackViews()) scene->addItem(healthpackView.get());
        for (auto &enemyView : mainController->getEnemyController().getEnemyViews()) scene->addItem(enemyView.get());



        pspeedSlider->setMaximum(10);
        pspeedSlider->setMinimum(1);

        if (!mainController->getProgramRunning())
        {
            connect(this, SIGNAL(keyPressed(int)), &mainController->getProtagonistController(), SLOT(moveProta(int)));
            connect(pspeedSlider, SIGNAL(valueChanged(int)), mainController.get(), SLOT(setPSpeed(int)) );
            connect(pspeedSlider, SIGNAL(valueChanged(int)), this, SLOT(pspeedChanged(int)));
            connect(hweightSlider, SIGNAL(valueChanged(int)), mainController.get(),  SLOT(setHWeight(int)));
            connect(hweightSlider, SIGNAL(valueChanged(int)), this, SLOT(hweightChanged(int)));
        }

    }
    if (!mainController->getProgramRunning()) connect(scene.get(), SIGNAL(mousePressed(int,int,bool)), this, SLOT(mousePressed(int,int,bool)));
    mainController->setProgramRunning(true);
    emit gameReady();
}

void MainView::setMap(QString filepath)
{
    qDebug() << "setMap";
    this->filepath = filepath;
    ui->graphicsView->setScene(scene.get());
    pixmap = scene->addPixmap(QPixmap(filepath));

}


void MainView::mousePressed(int x, int y, bool mouse)
{
    if (!mainController->isWorldRunning()) return;
    auto dim = mainController->getDimension();
    if (x < 0 || x >= dim.second || y < 0 || y >= dim.first)
    {
        qDebug() << "trying to go to" << x << y;
        showMessage(QString("Position (%1,%2) is out of boundary").arg(x).arg(y));
        return;
    }
    auto tile = mainController->getTileController().getTileAt(x,y);
    if (!tile.has_value()) return;
    if (isinf(tile.value()->getValue()))
    {
        showMessage("Unclickable tile");
        return;
    }

    qDebug() << "mouse" << x << y;

    if (mouse == 0)
    {
        ui->statusbar->showMessage(QString("Starting position is set to (%1,%2)").arg(x).arg(y));
        mainController->getProtagonistController().getProta()->setPos(x, y);
        return;
    }

    clickPos = std::pair(x, y);
    clickArea->setPos(clickPos.first, clickPos.second);
    showMessage("Calculating path...");

    clearPath();

    mainController->generateBestPath(x, y);
}

void MainView::showMessage(QString string)
{
    ui->statusbar->showMessage(string);
}

void MainView::replaceTile(int xPos, int yPos,QString symbol)
{
    QString tile =symbol;
    if(!mainController->getPathMode())
    {
        auto enemyat = mainController->getEnemyController().getEnemyAt(xPos,yPos);
        auto healthpackat = mainController->getHealthPackController().getHealthpackAt(xPos,yPos);
        if(enemyat.has_value() && enemyat->get()->getDefeated()) tile =mainController->getTileController().getTileChar(true);
        else if(healthpackat.has_value() && healthpackat->get()->getDefeated()) tile =mainController->getTileController().getHealthChar(true);
    }
    int index = (mainController->getDimension().second*yPos) + xPos;
    mTileMap.replace(index,tile);
}


void MainView::loadTextMap(){
    qDebug() << "MainView::loadTextMap()";
    auto prota = mainController->getProtagonistController().getProta();
    int index = (mainController->getDimension().second*prota->getYPos()) + prota->getXPos();
    if(!mainController->getPathMode()){
        QString protaText=mainController->getProtagonistController().getProtaView()->protaChar();
        mTileMap.replace(index,protaText);
        // print map
        if(display) return;
        textViewMap->clear();
        for(int row = 0; row < mainController->getDimension().first; row++){
            for(int col=0 ; col < mainController->getDimension().second; col++){
                int index =(mainController->getDimension().second*row)+col;
                    textViewMap->insertPlainText(mTileMap.at(index));
            }
            textViewMap->insertPlainText("\n");
        }
    }
}

void MainView::loadTextMaze(){
    textViewMap->clear();
    int maxY = mainController->getDimension().first;
    int maxX =mainController->getDimension().second;
    if(maxY >100) maxY =100;
    if(maxX >100) maxX =100;
    auto prota = mainController->getProtagonistController().getProta();
    int replacepos = (mainController->getDimension().second* prota->getYPos()) + prota->getXPos();
    mTileMaze.at(replacepos) = &protaChar;
    int startY = 0;
    int startX =0;
    int endX =maxX;
    int endY =maxY;
    if(prota->getXPos()<maxX) startX =0;
    else{
        startX = (prota->getXPos()/100)*maxX;
        qDebug()<<"ceil " <<ceil(prota->getXPos()/100);
        endX =(prota->getXPos()/100+1)*endX;
    }
    qDebug()<<"start x end x "<<startX << endX;
    if(prota->getYPos()<maxY) startY=0;
    else{
        startY = prota->getYPos()/100*maxY;
        endY =(prota->getYPos()/100+1)*endY;

    }
    qDebug()<<"start y end y"<<startY<<endY;
    if(endY >= mainController->getDimension().first) endY = mainController->getDimension().first;
    if(endX >= mainController->getDimension().second) endX = mainController->getDimension().second;
    for(int y = startY; y < endY; y++){
        for(int x=startX; x<endX; x++){
            int index = (mainController->getDimension().second*y) + x;
            textViewMap->insertPlainText(*mTileMaze.at(index));
        }
        textViewMap->insertPlainText("\n");
    }
}

void MainView::setupMap(){
    connect(&mainController->getEnemyController(), SIGNAL(currPos(int,int,QString)),this, SLOT(replaceTile(int,int,QString)));
    auto &tile = mainController->getTileController(); // mTilemap
    auto tiles =tile.getTiles();
    auto pushItem = [](auto &vec, auto &mainController,auto &tile, auto blocked, auto pass) {
        for(int i =0 ; i<mainController->getDimension().first;i++){
            for(int j=0; j<mainController->getDimension().second;j++){
                (isinf(tile.getTileAt(j,i)->get()->getValue()))? vec.push_back(blocked):vec.push_back(pass);
            }
        }
    };
    if(!mainController->getPathMode()){
        textView->clear();
        auto block =tile.getTileChar(true);
        auto pass = tile.getTileChar(false);
        mTileMap.clear();
        pushItem(mTileMap,mainController,tile,block,pass);
        qDebug()<<"mTileMap size:"<< mTileMap.size()<<"expect "<<mainController->getDimension().first * mainController->getDimension().second;
    }
    else{
        auto block =&blockedChar;
        auto pass = &passChar;
        mTileMaze.clear();
        pushItem(mTileMaze,mainController,tile,block,pass);
        qDebug()<<"mTileMaze size:"<< mTileMaze.size()<<" expect "<<mainController->getDimension().first * mainController->getDimension().second;
    }
}



void MainView::refreshText(int y)
{
    int yPos =0; //3
    int maxY = mainController->getDimension().second ;
    QTextCursor cursor = textViewMap->textCursor();
    for(int j =-1; j<2;j++){
        yPos =y+j; // y-1 = 2
        if(yPos<0) yPos =0;
        if(yPos>(maxY-1)) yPos= maxY-1;
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, yPos);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        textViewMap->setTextCursor(cursor);
        for(int i =0; i < mainController->getDimension().first; i++){
            int index =(maxY*yPos)+i;
            textViewMap->insertPlainText(mTileMap.at(index));
        }
    }
    textView->append(mainController->getProtagonistController().getProtaInfo());
}

void MainView::resizeEvent(QResizeEvent *event)
{
    if (mainController->isWorldRunning() == false) return;
    QMainWindow::resizeEvent(event);
    ui->graphicsView->fitInView(pixmap, Qt::KeepAspectRatio);
}

bool MainView::eventFilter(QObject *target, QEvent *event)
{ // arrow keys things

    if (mainController->isWorldRunning() == false) return false;
    if (event->type() == QEvent::KeyPress) {

        qDebug() << "key pressed...";

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        switch (keyEvent->key())
        {
            case Qt::Key_W: emit keyPressed(1); break;
            case Qt::Key_A: emit keyPressed(2); break;
            case Qt::Key_S: emit keyPressed(3); break;
            case Qt::Key_D: emit keyPressed(4); break;
        }
        clearPath();

    }
    return QObject::eventFilter(target, event);
}

void MainView::refresh()
{
    if(display) ui->graphicsView->viewport()->repaint();
}

void MainView::changeHealth(int health)
{
    ui->healthBar->setValue(health);
}

void MainView::changeEnergy(int energy)
{
    ui->energyBar->setValue(energy);
}

void MainView::fitInView() const
{
    ui->graphicsView->fitInView(pixmap, Qt::KeepAspectRatio);
}
void MainView::on_actionAutopilot_triggered()
{
    mainController->startAutoPilot();
}

void MainView::pspeedChanged(int s)
{
    ui->statusbar->showMessage(QString("Protagonist speed is changed to (%1)").arg(s));
}

void MainView::hweightChanged(int h)
{
    if(h == 0)
    {
        ui->statusbar->showMessage(QString("heuristic weight = 0, find path based on Dijkstra"));
    }
    else if(h >= 30)
    {
        ui->statusbar->showMessage(QString("heuristic weight is large enough (>=30), find path based on Best-First"));
    }
    else
    {
        ui->statusbar->showMessage(QString("heuristic weight = (%1), find path based on A-star").arg(h));
    }

}

void MainView::on_actionChange_view_triggered()
{
    changePage();
    ui->graphicsView->fitInView(pixmap, Qt::KeepAspectRatio);
}

void MainView::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainView::on_lineEdit_returnPressed()
{
    QString inputRead = textLine->text();
    qDebug()<<"ReadLine" << inputRead;
    emit readLine(inputRead);
    textLine->clear();
}

QTextEdit *MainView::getTextView()
{
    return textView;
}

std::shared_ptr<MainScene> MainView::getScene() const
{
    return scene;
}

void MainView::setMainController(std::shared_ptr<MainController> mainController)
{
    this->mainController = mainController;
}

std::shared_ptr<MainController> MainView::getMainController()
{
    return mainController;
}

void MainView::on_actionRestart_triggered()
{
    mainController->gameOver(false);
}
