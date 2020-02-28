#include "commandline.h"
#include <string>
#include <iostream>
#include <sstream>
#include <QStringList>
#include <QProcess>
#include <QObject>
#include <QRegularExpression>
#include "maincontroller.h"
#include "mainview.h"

CommandLine::CommandLine(){

}

void CommandLine::commandlineOutput(QString input)
{
    qDebug()<<"CommandLine :: Commandlineoutput" << input;
    auto textView = getTextView();
    if(compare(input,"help").second) help();
    else if(compare(input,"moveto").second) moveto(input);
    else if(compare(input,"showEnemy").second) enemyTextView();
    else if(compare(input,"showHealthpack").second) healthpackTextView();
    else if(compare(input,"clear").second) textView->clear();
    else if(compare(input,"getNearestHealthpack").second) getNearestHealthpack();
    else if(compare(input,"getNearestEnemy").second) getNearestEnemy();
    else if(compare(input,"showAll").second) showAll();
    else error_handler(input);
}

void CommandLine::help()
{
    auto textView = getTextView();
    QString command = "help";
    textView->append("Help: \nshowAll: to show all enemies and healthpacks on the map");
    textView->append("showEnemy: to show all the enemies position on the map");
    textView->append("showHealthpack: to show all the healthpacks position on the map");
    textView->append("showNearestEnemy: to show the nearest enemy");
    textView->append("showNearestHealthpack: to show the nearest healthpack available \nclear: to clear console");
}

void CommandLine::error_handler(QString input)
{
    auto textView = getTextView();
    if(compare(input,"showEnemy").first>4) textView->append("Command "+input+" not found do you mean 'showEnemy'");
    else if(compare(input,"showHealthpack").first>4) textView->append("Command "+input+" not found do you mean 'showHealthpack'");
    else if(compare(input,"showAll").first>4) textView->append("Commamd "+input +" not found do you mean 'showAll'");
    else if(compare(input,"show").first>1) textView->append("Command "+input+" not found do you mean 'showHealthpack' or 'showEnemy' or 'showAll'  ");
    else if(compare(input,"getNearestHealthpack").first>10) textView->append("Command "+input+" not found do you mean getNearestHealthpack");
    else if(compare(input,"getNearestEnemy").first>10) textView->append("command "+input+" not found do you mean getNearestEnemy");
    else if(compare(input,"getNearest").first>1) textView->append("Command "+ input+" not found do you mean getNearestEnemy or getNearestHealthpack");
    else if (compare(input,"moveto").first>1) textView->append("Command" +input +"not found, do you mean 'moveto(xPos,yPos)'");
    else if(compare(input, "clear").first>1)textView->append("Command "+input+" not found do you mean 'clear'");
    else if(compare(input,"help").first>1) textView->append("Command "+input+" not found do you mean 'help'");
    else textView->append("Command "+input+" not found");
}


std::pair<int,bool> CommandLine::compare(QString input, QString function)
{
    std::pair<int,bool> match;
    match.first =0;
    for(int i =0; i<input.length(); i++){
        if(function.length()>i && input.at(0).toLower() == input.at(0).toLower())
            if(function.at(i).toLower() == input.at(i).toLower()) match.first++;
    }
    if(match.first == function.length()) match.second= true;
    else match.second = false;
    return match; // return # of char match
}

void CommandLine::moveto(QString input){
    QRegularExpression rx("(\\d+)");
    int maxX = mainController->getDimension().second;
    int maxY = mainController->getDimension().first;
    int lower = 0;
    QList<int> list;
    QRegularExpressionMatchIterator i = rx.globalMatch(input);
    auto checkValue = [](int a,int upper, int lower) {
        if(a>(upper-1)) a = upper-1;
        else if(a<lower) a= lower;
        return a;
    };

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString word = match.captured(1);
        list << word.toInt();
    }
    int x,y;
    x = checkValue(list.first(),maxX,lower);
    y = checkValue(list.back(),maxY,lower);
    mainController->generateBestPath(x,y);
    getTextView()->append("move to "+ QString().setNum(x)+"," +QString().setNum(y)+" position");
}

QTextEdit *CommandLine::getTextView()
{
    return  mainView->getTextView();
}

void CommandLine::getNearestHealthpack(){ getTextView()->append(mainController->getNearestHealthpack()); }

void CommandLine::getNearestEnemy(){ getTextView()->append(mainController->getNearestEnemy()); }

void CommandLine::healthpackTextView()
{
   auto healthpacks = mainController->getHealthPackController().getHealthpacks();
   getTextView()->append(getItemTextView(healthpacks,"Healthpack"));

}

void CommandLine::enemyTextView(){
    auto enemies = mainController->getEnemyController().getEnemies();
    getTextView()->append(getItemTextView(enemies,"Enemy"));
}

void CommandLine::showAll()
{
    enemyTextView();
    healthpackTextView();
}


void CommandLine::setMainView(std::shared_ptr<MainView> mainView)
{
    this->mainView = mainView;
    qDebug()<<"connec slot ReadLine";
    connect(mainView.get(), SIGNAL(readLine(QString)),this, SLOT(commandlineOutput(QString)));
}

void CommandLine::setMainController(std::shared_ptr<MainController> mainController)
{
    this->mainController = mainController;
}












