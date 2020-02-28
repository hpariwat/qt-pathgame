#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <memory>
#include <QSplashScreen>
#include <QCheckBox>

#include "mainview.h"
#include "maincontroller.h"
#include "commandline.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_startGameBtn_clicked();
    void on_exitBtn_clicked();
    void on_menuBtn_clicked();
    void on_generateBtn_clicked();
    void on_preloadMapBtn_clicked();
    void gameReady();
    void gameStop();

private:
    Ui::MainMenu *ui;
    std::unique_ptr<QMediaPlayer> bgMusic{std::make_unique<QMediaPlayer>()};
    std::unique_ptr<QMediaPlaylist> bgPlayList{std::make_unique<QMediaPlaylist>()};
    QString filepath;
    bool gameRunning{false};
    QGraphicsScene mapScene{this};
    QGraphicsPixmapItem *pixmapItem;
    QPixmap pixmap;
    std::shared_ptr<MainController> mainController{nullptr};
    std::shared_ptr<MainView> mainView{nullptr};
    std::shared_ptr<CommandLine> commandLine{nullptr};
    QCheckBox * noPE;
    QCheckBox * noXE;

};

#endif // MAINMENU_H
