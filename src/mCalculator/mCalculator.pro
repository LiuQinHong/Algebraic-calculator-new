#-------------------------------------------------
#
# Project created by QtCreator 2018-11-24T12:34:40
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mCalculator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mcalculator.cpp \
    cell.cpp \
    item.cpp \
    itemlist.cpp \
    merge.cpp \
    separation.cpp \
    transform.cpp \
    tcpsever.cpp \
    network.cpp \
    protocol.cpp \
    cal_server.cpp \
    mainwindow.cpp \
    cal_client.cpp \
    qthreadnet.cpp

HEADERS  += mcalculator.h \
    cell.h \
    item.h \
    itemlist.h \
    merge.h \
    separation.h \
    transform.h \
    tcpsever.h \
    network.h \
    protocol.h \
    cal_server.h \
    mainwindow.h \
    cal_client.h \
    qthreadnet.h

FORMS    += mcalculator.ui \
    cal_server.ui \
    mainwindow.ui \
    cal_client.ui

RESOURCES += \
    images.qrc
