#-------------------------------------------------
#
# Project created by QtCreator 2017-10-20T13:53:50
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial_port
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    recvthread.cpp \
    protocol.cpp

HEADERS  += mainwindow.h \
    recvthread.h \
    protocol.h

FORMS    += mainwindow.ui

CONFIG +=C++11
