QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG+=sdk_no_version_check
SOURCES += \
    bomb.cpp \
    bombview.cpp \
    boss.cpp \
    bossview.cpp \
    commandline.cpp\
    enemycontrol.cpp \
    enemyview.cpp \
    fire.cpp \
    fireview.cpp \
    health.cpp \
    healthcontrol.cpp \
    healthview.cpp \
    main.cpp \
    maincontroller.cpp \
    mainmenu.cpp \
    mainscene.cpp \
    mainview.cpp \
    penemyview.cpp \
    protacontrol.cpp \
    protaview.cpp \
    tilecontrol.cpp \
    node.cpp \
    pathfinding.cpp

HEADERS += \
    bomb.h \
    bombview.h \
    boss.h \
    bossview.h \
    commandline.h\
    enemycontrol.h \
    enemyview.h \
    fire.h \
    fireview.h \
    health.h \
    healthcontrol.h \
    healthview.h \
    lib/world.h \
    lib/world_global.h \
    lib/pos.h \
    maincontroller.h \
    mainmenu.h \
    mainscene.h \
    mainview.h \
    penemyview.h \
    protacontrol.h \
    protaview.h \
    tilecontrol.h \
    node.h \
    pathfinding.h

FORMS += \
    mainmenu.ui \
    mainview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

unix: LIBS += -L$$PWD/lib/ -lworld

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES += \
    lib/libworld.so

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../world_v3/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../world_v3/debug/ -lworld
else:unix: LIBS += -L$$PWD/../world_v3/ -lworld

INCLUDEPATH += $$PWD/../world_v3
DEPENDPATH += $$PWD/../world_v3
