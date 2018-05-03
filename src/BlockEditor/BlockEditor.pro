#-------------------------------------------------
#
# Project created by QtCreator 2018-04-17T20:22:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlockEditor
TEMPLATE = app

CONFIG += console \
		c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    blockeditorscene.cpp \
    block.cpp \
    port.cpp \
    wire.cpp \
    calculator.cpp

HEADERS += \
        mainwindow.h \
    blockeditorscene.h \
    block.h \
    port.h \
    wire.h \
    calculator.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources_file.qrc
