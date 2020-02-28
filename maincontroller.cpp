#include "maincontroller.h"
#include "mainview.h"
#include "commandline.h"
#include <memory>
#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

MainController::MainController()
{
    qDebug() << "MainController::MainController()";
    tileController = std::make_unique<TileControl>();;
    protagonistController = std::make_unique<ProtaControl>();
    enemyController = std::make_unique<EnemyControl>();
    healthPackController = std::make_unique<HealthControl>();
    pathfinder = std::make_unique<PathFinding>();
    pSpeed = 10;
    penemyEnabled = true;
    xenemyEnabled = true;
    initSound();
}

void MainController::initWorld(QString filepath, int nrEnemies, int nrHealthpacks)
{
    if (pathMode) { nrEnemies = 0; nrHealthpacks = 0; }
;
    enemyController->setPeEnabled(penemyEnabled);
    enemyController->setXeEnabled(xenemyEnabled);

    world = std::make_unique<World>();
    world->createWorld(filepath, nrEnemies, nrHealthpacks);
    worldRunning = true;
    enemyLeft = nrEnemies;
    healthpackLeft = nrHealthpacks;
    tileController->setBoundary(world->getCols(), world->getRows());
    protagonistController->setProta(world->getProtagonist());

    for (auto &tile : world->getTiles()) tileController->addTile(std::move(tile));

    for(auto &i: tileController->getTiles())
    {
        int x = i->getXPos();
        int y = i->getYPos();
        float v = i->getValue();
        if(std::isinf(v)) v = 100;
        allNodes.push_back(std::make_shared<Node>(x,y,v,0,0,0));
    }

    mainView->setupMap(); // to change back
    connect(this, SIGNAL(currPos(int, int, QString)), mainView.get(), SLOT(replaceTile(int, int, QString)));

    for (auto &enemy : world->getEnemies()) enemyController->addEnemy(std::move(enemy));
    for (auto &healthpack : world->getHealthPacks()) healthPackController->addHealthpack(std::move(healthpack));
    for (auto &enemy : enemyController->getEnemies())
    {
        if (auto e = qobject_cast<Boss*>(enemy.get()))
        {
            connect(e, SIGNAL(sendBomb()), this, SLOT(releaseBomb()));
            currPos(e->getXPos(),e->getYPos(),tileController->getEnemyChar(TileControl::Xenemy));
        }
        else if (auto e = qobject_cast<PEnemy*>(enemy.get()))
        {
            connect(e, SIGNAL(poisonLevelUpdated(int)), this, SLOT(poisonChanged(int)));
            connect(e, SIGNAL(dead()), this, SLOT(poisonDead()));
            currPos(e->getXPos(),e->getYPos(),tileController->getEnemyChar(TileControl::Penemy));
        }
        else currPos(enemy->getXPos(),enemy->getYPos(),tileController->getEnemyChar(TileControl::Enemy));
    }

    for(auto healthpack : healthPackController->getHealthpacks()) currPos(healthpack->getXPos(),healthpack->getYPos(),tileController->getHealthChar(false));

    connect(protagonistController.get(),SIGNAL(currPos(int, int,QString)),mainView.get(), SLOT(replaceTile(int, int,QString)));
    connect(protagonistController.get(), SIGNAL(checkBoundary(int, int)), this, SLOT(checkBoundary(int, int)));
    connect(protagonistController->getProta().get(), SIGNAL(healthChanged(int)), mainView.get(), SLOT(changeHealth(int)));
    connect(protagonistController->getProta().get(), SIGNAL(energyChanged(int)), mainView.get(), SLOT(changeEnergy(int)));
    connect(protagonistController->getProta().get(), SIGNAL(posChanged(int,int)), protagonistController->getProtaView().get(), SLOT(posChanged(int,int)));

    mainView->loadGraphical();
    if (!pathMode) mainView->loadTextMap();
    if (pathMode)  mainView->loadTextMaze();
    PLAY(gongSFX);
    PLAY(gameBGM);

}

void MainController::checkMoveProta(int x, int y)
{
    auto tile = tileController->getTileAt(x, y);
    auto enemy = enemyController->getEnemyAt(x, y);
    auto healthpack = healthPackController->getHealthpackAt(x, y);

    if (tile.has_value() == false) return;
    moves++;

    float cost = 1 - tile.value()->getValue();
    auto energy = protagonistController->getProta()->getEnergy();
    auto health = protagonistController->getProta()->getHealth();

    energy -= cost;

    if (enemy.has_value())
    {
        hitEnemy(enemy, x, y);
        auto str = enemy.value()->getValue();
        health -= str;
        if (cost < 0) { // if cost is negative, the tile is impassable
            energy = 0;
        } else {
            energy = 100;
        }
        tile.value()->setValue(INFINITY); // impassable
        allNodes.at(y * dimension.second + x)->setValue(100);
    }

    if (healthpack.has_value()) // set full condition here as well
    {
        float potion = healthpack.value()->getValue();;
        healthpack.value()->setEmpty(true); // change from false to true
        healthpackLeft--;
        health += potion;

        PLAY(pickupSFX);
    }

    auto fire = getFireAt(x, y);
    if (fire.has_value() && fire.value()->getIsOnFire())
    {
        health -= cost; // walk on fire will decrease health...
        PLAY(ouchSFX);
    }

    if (health > 100) health = 100;
    else if (health < 0) health = 0;
    if (energy < 0) energy = 0;

    protagonistController->getProta()->setEnergy(energy);
    protagonistController->getProta()->setHealth(health);
    mainView->refresh();
    if (health <= 0 || energy <= 0) gameOver(false);
    else if (enemyLeft <= 0) gameOver(true);
}


void MainController::hitEnemy(const std::optional<std::shared_ptr<Enemy> > &enemy, int x, int y)
{
    enemyLeft--;
    if (auto e = qobject_cast<PEnemy*>(enemy.value().get()))
    {
        for (int i = -2; i < 3; i++)
        {
            for (int j = -2; j < 3; j++)
            {
                auto tempTile = tileController->getTileAt(x-i, y-j);
                if (!tempTile.has_value()) continue;
                emit currPos(x-i,y-j,tileController->getEnemyChar(TileControl::Penemy)); //TODO refactor
                auto f = std::make_shared<Fire>(x-i, y-j, tempTile.value()->getValue());
                auto fv = std::make_unique<FireView>(tempTile.value());
                connect(f.get(), SIGNAL(onFire(bool)), fv.get(), SLOT(onFire(bool)));
                connect(f.get(), SIGNAL(fireLevel(float)), fv.get(), SLOT(fireLevel(float)));
                connect(fv.get(), SIGNAL(remove()), this, SLOT(removeItem()));
                f->setIsOnFire(1);
                mainView->getScene()->addItem(fv.get());
                fires.push_back(std::move(f));
                fireViews.push_back(std::move(fv));
            }
        }
        mainView->loadTextMap();
        e->poison();
        PLAY(demonAttackSFX);
    }
    else if (auto e = qobject_cast<Bomb*>(enemy.value().get()))
    {
        e->setDefeated(true);
    }
    else if (auto e = qobject_cast<Boss*>(enemy.value().get()))
    {
        e->die();
        PLAY(enemyDeadSFX);
    }
    else if (auto e = qobject_cast<Enemy*>(enemy.value().get()))
    {   
        e->setDefeated(true);

        PLAY(enemyDeadSFX);
    }
}

void MainController::checkBoundary(int x, int y)
{
    if (!worldRunning) return;
    //qDebug() << "checkBounary" << x << y;
    auto tile = getTileController().getTileAt(x, y);
    if (x < 0 || x >= dimension.first || y < 0 || y >= dimension.second || isinf(tile.value()->getValue())) return;

    PLAY(runSFX);

    auto prota = protagonistController->getProta();
    prota->setPos(x,y);
    currPos(x,y,getProtagonistController().getProtaView()->protaChar());
    mainView->refreshText(y);
    checkMoveProta(x, y);
}


void MainController::poisonChanged(int value)
{
    if (!worldRunning) return;
    qDebug() << "poisonChanged " << value;
    auto pro = protagonistController->getProta();
    auto cEnemy = qobject_cast<PEnemy*>(sender());

    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            auto tv = tileController->getTileAt(cEnemy->getXPos()-i, cEnemy->getYPos()-j);
            if (!tv.has_value()) continue;
            auto fire = getFireAt(cEnemy->getXPos()-i, cEnemy->getYPos()-j);
            if (fire.has_value()) fire.value()->setFireLevel((float)((value / 100.0f)+0.2f));
            if (pro->getXPos() == tv.value()->getXPos() && pro->getYPos() == tv.value()->getYPos())
            {
                auto finalHealth = pro->getHealth() - cEnemy->getValue() / 10;
                if (finalHealth < 0) finalHealth = 0;
                protagonistController->getProta()->setHealth(finalHealth);
                if (finalHealth == 0) gameOver(false);
                else PLAY(ouchSFX);
            }
        }
    }
    mainView->refresh();
    mainView->loadTextMap();
    PLAY(burnSFX);
    //if (!worldRunning) return;
}

void MainController::poisonDead()
{
    if (!worldRunning) return;
    auto cEnemy = qobject_cast<PEnemy*>(sender());
    PLAY(demonDeadSFX);

    int xPos = cEnemy->getXPos();
    int yPos = cEnemy->getYPos();
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            auto tv = tileController->getTileAt(xPos-i, yPos-j);
            if (!tv.has_value()) continue;
            (isinf(tv->get()->getValue()))?currPos(xPos-i,yPos-j,tileController->getTileChar(true)): currPos(xPos-i,yPos-j,tileController->getTileChar(false));
            auto fire = getFireAt(xPos-i, yPos-j);
            if (fire.has_value())
            {
                fire.value()->setIsOnFire(false);
            }
        }
    }
    currPos(xPos,yPos,tileController->getTileChar(true));
    mainView->refresh();
    mainView->loadTextMap();
}

void MainController::moveBomb()
{
    if (!worldRunning) return;
    auto enemy = qobject_cast<Bomb*>(sender());
    auto protax = protagonistController->getProta()->getXPos();
    auto protay = protagonistController->getProta()->getYPos();
    auto enemyx = enemy->getXPos();
    auto enemyy = enemy->getYPos();

    if (protax < enemyx) enemyx--;
    else if (protax > enemyx) enemyx++;
    if (protay < enemyy) enemyy--;
    else if (protay > enemyy) enemyy++;

    enemy->setPos(enemyx, enemyy);

    if (protax == enemyx && protay == enemyy)
    {
        enemy->explode();
        PLAY(explosionSFX);
        auto energy = protagonistController->getProta()->getEnergy();
        auto health = protagonistController->getProta()->getHealth();
        energy += 5;
        health -= enemy->getValue();
        protagonistController->getProta()->setEnergy(energy);
        protagonistController->getProta()->setHealth(health);
        if (health <= 0) gameOver(false);
    }
}

void MainController::releaseBomb()
{
    if (!worldRunning) return;
    auto boss = qobject_cast<Boss*>(sender());
    auto b = std::make_shared<Bomb>(boss->getXPos(), boss->getYPos(), 5);
    auto bv = std::make_unique<BombView>(b);
    connect(b.get(), SIGNAL(move()), this, SLOT(moveBomb()));
    connect(b.get(), SIGNAL(dead()), bv.get(), SLOT(showDead()));
    connect(b.get(), SIGNAL(changePos(int,int)), bv.get(), SLOT(changePos(int, int)));
    connect(bv.get(), SIGNAL(remove()), this, SLOT(removeItem()));
    mainView->getScene()->addItem(bv.get());
    bombs.push_back(std::move(b));
    bombViews.push_back(std::move(bv));
    PLAY(sendBombSFX);
}

void MainController::gameOver(bool win)
{
    winGame = win;
    worldRunning = false;
    if (autoPilotMode) return;
    loseBGM->stop();
    winBGM->stop();
    gameBGM->stop();

    QMessageBox msgBox;
    if (win == false)
    {    
        PLAY(protaDeadSFX);
        PLAY(loseBGM);
        protagonistController->getProtaView()->setPixmap(QPixmap(":/resources/pic/protaDead.png"));
        msgBox.setText("You lose...");
    }
    else
    {
        PLAY(winBGM);
        msgBox.setText("You win!");
    }

    if (enemyLeft < 0) enemyLeft = 0;
    if (healthpackLeft < 0) healthpackLeft = 0;

    msgBox.setInformativeText(QString("Number of moves: %1\nEnemy left: %2\nHealthpack left: %3").arg(moves).arg(enemyLeft).arg(healthpackLeft));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, tr("Main Menu"));
    msgBox.setButtonText(QMessageBox::No, tr("Close"));
    moves = 0;
    auto ret = msgBox.exec();
    if (ret == QMessageBox::Yes)
    {
        cleanGame();
        emit gameStop();
    }
}

void MainController::startAutoPilot()
{
    autoPilotMode = true;
    std::deque<std::shared_ptr<Node>> solution, blank;
    auto prota = protagonistController->getProta();
    auto enemies = enemyController->getEnemies();
    auto healthpacks = healthPackController->getHealthpacks();
    auto checkRoute = [=](auto &solution, auto &route)
    {
        route.value() = reRoute(route.value());
        solution = route.value();
    };
    while(enemyLeft > 0 && prota->getEnergy() > 0 && prota->getHealth() > 0)
    {
        if (!worldRunning) break;
        auto dummy = findBestPath(enemies, *tileController, prota, pathfinder, false, allNodes);
        if(dummy.has_value()) checkRoute(solution, dummy);
        else
        {
            auto dummy = findBestPath(healthpacks,*tileController,prota,pathfinder,true, allNodes);
            if(dummy.has_value()) checkRoute(solution, dummy);
        }
        if (solution.empty()) break;
        followBestPath(solution);
        solution.clear();
        mainView->clearPath();
    }
    autoPilotMode = false;
    if (enemyLeft > 0 && solution.empty()) mainView->showMessage("No solution found");
    else gameOver(true);
}

std::deque<std::shared_ptr<Node>> MainController::reRoute(std::deque<std::shared_ptr<Node>> oldPath)
{
    std::vector<float> tempValues;
    std::vector<std::shared_ptr<Node>> tempNodes;
    std::deque<std::shared_ptr<Node>> newPath;
    auto prota = protagonistController->getProta();
    auto enemies = enemyController->getEnemies();
    auto healthpacks = healthPackController->getHealthpacks();
    bool dummy{false};
    checkObstruction(oldPath, enemies, tempNodes, tempValues, dummy);
    checkObstruction(oldPath, healthpacks, tempNodes, tempValues, dummy);
    if (tempValues.empty()) return oldPath;
    bool clean = false;
    while (!clean)
    {
        qDebug() << "there's something on the best path...";
        bool foundSomething = false;
        newPath = pathfinder->Astar(prota->getXPos(), prota->getYPos(), oldPath.back()->getPosx(), oldPath.back()->getPosy(), allNodes);
        checkObstruction(newPath, enemies, tempNodes, tempValues, foundSomething);
        checkObstruction(newPath, healthpacks, tempNodes, tempValues, foundSomething);
        if (!foundSomething) clean = true;
    }
    while (!tempNodes.empty())
    {
        tempNodes.back()->setValue(tempValues.back());
        tempNodes.pop_back();
        tempValues.pop_back();
    }
    return newPath;
}

void MainController::generateBestPath(int x, int y)
{
    if (x < 0 || x >= dimension.second || y < 0 || y >= dimension.first) return;
    auto protax = protagonistController->getProta()->getXPos();
    auto protay = protagonistController->getProta()->getYPos();
    qDebug() << "generateBestPath from" << protax << protay << "to" << x << y;

    auto t1 = std::chrono::steady_clock::now();
    auto bestPath = pathfinder->Astar(protax, protay, x , y, allNodes);
    auto t2 = std::chrono::steady_clock::now();
    float t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
    mainView->showMessage(QString("Time: %1 ms").arg(t3/1000));
    followBestPath(bestPath);
}

void MainController::followBestPath(const std::deque<std::shared_ptr<Node>> &bestPath)
{
    int msToDelay = 20 * (10 - pSpeed);
    qDebug()<< "msToDelay = "<< msToDelay << pathMode;
    double msPassed;
    bool flag;
    for(auto &i : bestPath)
    {
        auto bestx = i->getPosx();
        auto besty = i->getPosy();
        mainView->addPath(bestx, besty);
        if (!pathMode)
        {
            protagonistController->getProta()->setPos(bestx, besty);
            mainView->refresh();
            checkBoundary(bestx, besty);
            auto startTime = std::chrono::steady_clock::now();
            flag = true;
            while (flag)
            {
                msPassed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
                if(msPassed >= msToDelay)
                {
                    flag = false;
                }
                if (!worldRunning) return;
            }
        }
        QCoreApplication::processEvents(); // cut the loop
    }
    protagonistController->getProta()->setPos(bestPath.back()->getPosx(), bestPath.back()->getPosy());
}

std::optional<std::shared_ptr<Fire> > MainController::getFireAt(int x, int y)
{
    for (auto &f : fires) if (f->getXPos() == x && f->getYPos() == y) return f;
    return std::nullopt;
}

void MainController::removeItem()
{
    qDebug() << "REMOVE ITEM";
    if (auto bomb = qobject_cast<BombView*>(sender())) removeEntity(bombs, bombViews, bomb);
    else if (auto fire = qobject_cast<FireView*>(sender())) removeEntity(fires, fireViews, fire);
}

void MainController::initSound()
{
    playlist->addMedia(QUrl("qrc:/resources/bg_game.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    gameBGM->setPlaylist(playlist.get());
    gameBGM->setVolume(36);
    loseBGM->setMedia(QUrl("qrc:/resources/bgm/lose.mp3"));
    winBGM->setMedia(QUrl("qrc:/resources/bgm/win.mp3"));
    gongSFX->setMedia(QUrl("qrc:/resources/sfx/gong.wav"));
    pickupSFX->setMedia(QUrl("qrc:/resources/protapickup.wav"));
    runSFX->setMedia(QUrl("qrc:/resources/protarun.wav"));
    ouchSFX->setMedia(QUrl("qrc:/resources/sfx/ouch.wav"));
    burnSFX->setMedia(QUrl("qrc:/resources/sfx/burn.wav"));
    protaDeadSFX->setMedia(QUrl("qrc:/resources/sfx/protaDead.wav"));
    enemyDeadSFX->setMedia(QUrl("qrc:/resources/enemydead.wav"));
    demonAttackSFX->setMedia(QUrl("qrc:/resources/sfx/demonAttack.wav"));
    demonDeadSFX->setMedia(QUrl("qrc:/resources/sfx/demonDead.wav"));
    explosionSFX->setMedia(QUrl("qrc:resources/sfx/explosion.wav"));
    sendBombSFX->setMedia(QUrl("qrc:resources/sfx/sendBomb.wav"));
}

void MainController::setPenemyEnabled(int state)
{
    if(state == Qt::Checked) penemyEnabled = false;
    else penemyEnabled = true;
}

void MainController::setXenemyEnabled(int state)
{
    if(state == Qt::Checked) xenemyEnabled = false;
    else xenemyEnabled = true;
}

void MainController::setHWeight(int value)
{
    pathfinder->setHeuWeight(value);
}

void MainController::setPSpeed(int value)
{
    pSpeed = value;
}

QString MainController::getNearestEnemy()
{
    auto enemies = enemyController->getEnemies();
    return findNearestItem(enemies,*tileController,getProtagonistController().getProta(),pathfinder,false,allNodes);
}

QString MainController::getNearestHealthpack()
{
    auto healthpacks = healthPackController->getHealthpacks();
    return findNearestItem(healthpacks,*tileController,getProtagonistController().getProta(),pathfinder,true,allNodes);
}
bool MainController::isWorldRunning()
{
    return worldRunning;
}

TileControl &MainController::getTileController() const
{
    return *tileController;
}

std::pair<int, int> MainController::getDimension() const
{
    return dimension;
}

void MainController::setDimension(const std::pair<int, int> &value)
{
    dimension = value;
}

void MainController::setWorldRunning(bool value)
{
    worldRunning = value;
}

HealthControl &MainController::getHealthPackController() const
{
    return *healthPackController;
}

ProtaControl &MainController::getProtagonistController() const
{
    return *protagonistController;
}

EnemyControl &MainController::getEnemyController() const
{
    return *enemyController;
}

void MainController::setMainView(std::shared_ptr<MainView> mainView)
{
    this->mainView = mainView;
}

std::shared_ptr<MainView> MainController::getMainView()
{
    return mainView;
}

void MainController::setPathMode(bool pathMode)
{
    this->pathMode = pathMode;
}

bool MainController::getPathMode() const
{
    return pathMode;
}

bool MainController::getProgramRunning() const
{
    return programRunning;
}

void MainController::setProgramRunning(bool value)
{
    programRunning = value;
}

void MainController::cleanGame()
{
    tileController->clearTiles();
    enemyController->clearEnemies();
    healthPackController->clearHealthpacks();
    fires.clear();
    fireViews.clear();
    bombs.clear();
    bombViews.clear();
    allNodes.clear();
    enemyController->setMakeBoss(false);
    loseBGM->stop();
    winBGM->stop();
    mainView->clearPath();
    protagonistController->getProta().reset();
    protagonistController->getProtaView().reset();
    tileController->disconnect();
    enemyController->disconnect();
    healthPackController->disconnect();
    protagonistController->disconnect();
    emit gameStop();
}
