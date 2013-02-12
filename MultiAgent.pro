#-------------------------------------------------
#
# Project created by QtCreator 2012-11-23T14:04:06
#
#-------------------------------------------------

QT       += core gui declarative

TARGET = MultiAgent

TEMPLATE += app

CONFIG += qt gui

SOURCES += \
    widgetview.cpp \
    warrior.cpp \
    strategie.cpp \
    robot.cpp \
    resource.cpp \
    planet.cpp \
    pathfinder.cpp \
    MersenneTwister.cpp \
    mainwindow.cpp \
    main.cpp \
    clanmember.cpp \
    clan.cpp \
    commercial.cpp

HEADERS  += \
    widgetview.h \
    warrior.h \
    structure.h \
    strategie.h \
    robot.h \
    resource.h \
    planet.h \
    pathfinder.h \
    MersenneTwister.h \
    mainwindow.h \
    clanmember.h \
    clan.h \
    commercial.h

FORMS    +=

OTHER_FILES += \
    Rules.txt
