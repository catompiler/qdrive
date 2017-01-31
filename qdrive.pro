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

LIBS    += -Lc:/MinGW/lib -lmodbus

INCLUDEPATH += ../lib c:/MinGW/include

SOURCES += main.cpp\
    mainwindow.cpp \
    settings.cpp \
    settingsdlg.cpp \
    drive.cpp \
    driveworker.cpp \
    parameter.cpp \
    paramview.cpp \
    future.cpp \
    ../lib/menu/menu.c \
    paramsmodel.cpp \
    eventsmodel.cpp \
    eventmodel.cpp \
    driveevent.cpp \
    driveoscillogram.cpp \
    oscview.cpp \
    utils.cpp \
    selectoscsdlg.cpp \
    paramsdelegate.cpp

HEADERS  += mainwindow.h \
    settings.h \
    settingsdlg.h \
    drive.h \
    driveworker.h \
    parameters_ids.h \
    parameter.h \
    paramview.h \
    drive_types.h \
    parameters_list.h \
    future.h \
    ../lib/menu/menu.h \
    paramsmodel.h \
    eventsmodel.h \
    eventmodel.h \
    driveevent.h \
    driveoscillogram.h \
    oscview.h \
    utils.h \
    selectoscsdlg.h \
    paramsdelegate.h \
    parameters_menu.h

FORMS    += mainwindow.ui \
    settingsdlg.ui \
    selectoscsdlg.ui

RESOURCES += \
    res.qrc
