#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QThread>
#include <QAbstractEventDispatcher>

MainMenu::MainMenu(QWidget *parent) : QDialog(parent), ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    bgPlayList->addMedia(QUrl("qrc:/resources/bgm/menu.mp3"));
    bgPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    bgMusic->setMedia(bgPlayList.get());
    bgMusic->play();

    mainController = std::make_shared<MainController>();
    mainView = std::make_shared<MainView>();
    commandLine = std::make_shared<CommandLine>();
    mainController->setMainView(mainView);
    mainView->setMainController(mainController);
    commandLine->setMainController(mainController);
    commandLine->setMainView(mainView);

    connect(mainView.get(), SIGNAL(gameReady()), this, SLOT(gameReady()));
    connect(mainController.get(), SIGNAL(gameStop()), this, SLOT(gameStop()));
    noPE = ui->noPenemyCheckbox;
    noXE = ui->noXenemyCheckbox;
    connect(noPE, SIGNAL(stateChanged(int)), mainController.get(), SLOT(setPenemyEnabled(int)));
    connect(noXE, SIGNAL(stateChanged(int)), mainController.get(), SLOT(setXenemyEnabled(int)));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_exitBtn_clicked()
{
    QApplication::exit();
}

void MainMenu::on_startGameBtn_clicked()
{
    ui->menuStack->setCurrentIndex(1); // go to preload map
}

void MainMenu::on_menuBtn_clicked()
{
    ui->menuStack->setCurrentIndex(0); // go to main menu
}

void MainMenu::on_generateBtn_clicked()
{
    if (filepath.isEmpty() || gameRunning) return;
    gameRunning = true;
    ui->menuStack->setCurrentIndex(2); // go to main menu
    QApplication::processEvents();
    mainView->setMap(filepath);
    mainController->setDimension(std::pair(pixmap.height(), pixmap.width()));
    if (pixmap.width() * pixmap.height() > 5000) mainController->setPathMode(true);
    mainController->initWorld(filepath, ui->nrEnemySpinner->value(), ui->nrHealthSpinner->value());
}

void MainMenu::on_preloadMapBtn_clicked()
{
    mapScene.clear(); // remove last item before adding new one
    // comment out the graphical view
    filepath = QFileDialog::getOpenFileName(this, tr("Open file"));
    if (filepath.isEmpty()) return;
    std::cout << "Loading from: " << filepath.toStdString() << std::endl;
    ui->preMap->setScene(&mapScene);
    pixmap = QPixmap(filepath);
    pixmapItem = mapScene.addPixmap(pixmap);
    ui->preMap->fitInView(pixmapItem, Qt::KeepAspectRatio);
}

void MainMenu::gameReady()
{
    hide();
    bgMusic->stop();
    mainView->show();
    mainController->setWorldRunning(true);
    mainView->fitInView();
}

void MainMenu::gameStop()
{
    mainView->hide();
    bgMusic->play();
    ui->menuStack->setCurrentIndex(0); // go to main menu
    show();
    gameRunning = false;
}
