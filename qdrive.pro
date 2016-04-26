#-------------------------------------------------
#
# Project created by QtCreator 2016-04-09T20:04:44
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qdrive
TEMPLATE = app

LIBS    += -lmodbus

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    settingsdlg.cpp \
    drive.cpp \
    driveworker.cpp

HEADERS  += mainwindow.h \
    settings.h \
    settingsdlg.h \
    drive.h \
    driveworker.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    res.qrc
