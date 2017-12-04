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






unix|win32: LIBS += -L$$PWD/../../Qt_Test_Work/Qt_Library/lib/ -lQt_library

INCLUDEPATH += $$PWD/../../Qt_Test_Work/Qt_Library/include
DEPENDPATH += $$PWD/../../Qt_Test_Work/Qt_Library/include
