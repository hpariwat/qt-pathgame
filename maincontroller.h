#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <set>
#include <QFileDialog>
#include <QLabel>
#include <QObject>
#include <QDebug>

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <cmath>

#include <QMessageBox>

#include "lib/world.h"
#include "healthcontrol.h"
#include "protacontrol.h"
#include "enemycontrol.h"
#include "tilecontrol.h"
#include "commandline.h"

#include "bomb.h"
#include "fire.h"
#include "boss.h"
#include "bombview.h"

#include "pathfinding.h"

class MainView;
class MainMenu;


// Play sound and if it's already played then reset
#define PLAY(...) ((__VA_ARGS__->state() == QMediaPlayer::PlayingState) ? __VA_ARGS__->setPosition(0) : __VA_ARGS__->play())

template<typename Vec, typename Con, typename Pro, typename Node>  // vector, tilecontroller, protagonist
QString findNearestItem(Vec &vectors, Con &con, Pro prota,std::unique_ptr<PathFinding> &pathfinder, bool health, Node &node){
    QString pos, type, strength;
    (health) ? type ="Healthpack" : type = "Enemy";
    auto nearest = findBestPath(vectors,con,prota,pathfinder,health,node);
    if(nearest.has_value()){
        for(auto &vec : vectors){
            if(vec->getXPos() == nearest->back()->getPosx() && vec->getYPos() == nearest->back()->getPosy()) strength.setNum(int(vec->getValue()));
        }
        pos = "Nearest "+type+" at ("+QString().setNum(nearest->back()->getPosx()) +","+ QString().setNum(nearest->back()->getPosy())+") strength: "\
                +strength;}
    else pos = "Nearest "+type+" not found";
    return pos;
}

template<typename M, typename V, typename E>
void removeEntity(M &models, V &views, E &entity)
{
    int i = 0;
    for (const auto& v : views)
    {
        if (entity == v.get())
        {
            views[i] = std::move(views.back());
            views.pop_back();
            models[i] = std::move(models.back());
            models.pop_back();
            break;
        }
        i++;
    }
}

template<typename V, typename C, typename P, typename N>  // vector, tilecontroller, protagonist
std::optional<std::deque<std::shared_ptr<Node>>> findBestPath(V &vector, C &tileController, P &prota, std::unique_ptr<PathFinding> &pathfinder,bool health, N &allNodes)
{
    float compare = std::numeric_limits<float>::infinity();
    std::deque<std::shared_ptr<Node>> bestPath, dummyPath;
    for(auto &vec : vector)
    {
        if(vec->getDefeated()) continue;
        dummyPath = pathfinder->Astar(prota->getXPos(), prota->getYPos(), vec->getXPos(), vec->getYPos(), allNodes);
        if(!dummyPath.empty())
        {
            float dummyEnergy = 0.0f;
            float cost = 0.0f;
            for(auto &node : dummyPath)
            {
                cost += node->getF();
                auto tile = tileController.getTileAt(node->getPosx(), node->getPosy());
                if (tile.has_value()) dummyEnergy += tile.value()->getValue();
            }
            if (prota->getEnergy() > dummyEnergy)
            {
                if (cost < compare)
                {
                    if (health || prota->getHealth() > vec->getValue())
                    {
                        compare = cost;
                        bestPath = dummyPath;
                    }
                    else qDebug() << "// NOT ENOUGH HEALTH";
                }
                else qDebug() << "// BAD SOLUTION";
            }
            else qDebug() << "// NOT ENOUGH ENERGY";
        }
    }
    if(!bestPath.empty()) return std::move(bestPath);
    return std::nullopt;
}

template<typename P, typename V, typename FE, typename FV>
void checkObstruction(P &path, V &vector, FE &foundEntities, FV &foundValues, bool &foundSomething)
{
    for (auto &e: path)
    {
        for (auto &vec : vector)
        {
            if (e->getPosx() == vec->getXPos() && e->getPosy() == vec->getYPos())
            {
                qDebug() << "// FOUND SOMETHING ON THE BEST PATH";
                if (path.back() == e) continue;
                foundValues.push_back(e->getValue());
                e->setValue(INFINITY);
                foundEntities.push_back(e);
                foundSomething = true;
            }
        }
    }
};


class MainController : public QObject
{
    Q_OBJECT

public:
    MainController();
    ~MainController() {qDebug() << "MainController Deleted...";}

    void cleanGame();
    ProtaControl &getProtagonistController() const;
    EnemyControl &getEnemyController() const;
    HealthControl &getHealthPackController() const;
    TileControl &getTileController() const;


    void setMainView(std::shared_ptr<MainView> mainView);
    std::shared_ptr<MainView> getMainView();
    void initWorld(QString filepath, int nrEnemies, int nrHealthpacks);
    bool isWorldRunning();

    void startAutoPilot();

    void gameOver(bool win);

    void initSound();

    void hitEnemy(const std::optional<std::shared_ptr<Enemy>> &enemy, int x, int y);

    std::pair<int, int> getDimension() const;

    void setDimension(const std::pair<int, int> &value);

    void setWorldRunning(bool value);

    void generateBestPath(int x, int y);

    void setPathMode(bool pathMode);
    bool getPathMode() const;
    QString getNearestEnemy();
    QString getNearestHealthpack();
    std::optional<std::shared_ptr<Fire>> getFireAt(int x, int y);

    void followBestPath(const std::deque<std::shared_ptr<Node> > &bestPath);

    std::deque<std::shared_ptr<Node> > reRoute(std::deque<std::shared_ptr<Node> > dummy);
    bool getProgramRunning() const;

    void setProgramRunning(bool value);

private:

    std::vector<std::unique_ptr<FireView>> fireViews;
    std::vector<std::shared_ptr<Fire>> fires;
    std::vector<std::unique_ptr<BombView>> bombViews;
    std::vector<std::shared_ptr<Bomb>> bombs;

    bool autoPilotMode{false};
    bool programRunning = false;
    bool winGame{false};

    std::shared_ptr<MainView> mainView;
    std::unique_ptr<HealthControl> healthPackController;
    std::unique_ptr<ProtaControl> protagonistController;
    std::unique_ptr<EnemyControl> enemyController;
    std::unique_ptr<TileControl> tileController;


    std::vector<std::shared_ptr<Node>> allNodes;

    bool worldRunning = false;

    std::unique_ptr<World> world;
    std::pair<int,int> dimension;
    std::pair<int,int> destination{std::make_pair(0,0)};

    std::unique_ptr<PathFinding> pathfinder{nullptr};

    bool pathMode{false};
    int moves{0};

    int pSpeed;//the moving speed of protagonist
    bool penemyEnabled;
    bool xenemyEnabled;

    std::unique_ptr<QMediaPlayer> pickupSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> enemyDeadSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> gongSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> gameBGM{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> demonAttackSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> demonDeadSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> explosionSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> sendBombSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> ouchSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> burnSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> runSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> loseBGM{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> winBGM{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlayer> protaDeadSFX{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlaylist> playlist{std::make_unique<QMediaPlaylist>()};

    int enemyLeft{0},healthpackLeft{0};
signals:
   void moveTo(int x, int y); // after prota has moved, send a signal to update
   void currPos(int x, int y, QString symbol);
   void changePos(int x, int y);
   void gameStop();

private slots:
   void checkMoveProta(int x, int y);
   void checkBoundary(int x, int y);
   void poisonChanged(int value);
   void poisonDead();
   void moveBomb();
   void releaseBomb();
   void removeItem();
   void setPSpeed(int value);
   void setHWeight(int value);
   void setPenemyEnabled(int state);
   void setXenemyEnabled(int state);

};

#endif // MAINCONTROLLER_H
