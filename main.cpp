#include "mainview.h"
#include "maincontroller.h"
#include "commandline.h"

#include <QApplication>
#include "mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto mainMenu = MainMenu();
    mainMenu.show();
    return a.exec();
}
