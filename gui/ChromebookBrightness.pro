#-------------------------------------------------
#
# Project created by QtCreator 2016-04-24T10:19:02
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = brightnessctl-gui
TEMPLATE = app

LIBS += -ltjlutils

INCLUDEPATH += include/


SOURCES += src/main.cpp\
           src/mainwindow.cpp\
           src/customlineedit.cpp

HEADERS  += include/mainwindow.h\
            include/customlineedit.h

FORMS    += forms/mainwindow.ui

RESOURCES += resources/icons.qrc
