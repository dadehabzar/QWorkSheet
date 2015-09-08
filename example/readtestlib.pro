#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T14:39:10
#
#-------------------------------------------------

QT       += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = readtestlib
TEMPLATE = app
        LIBS += ./qworksheet/release/qworksheet.lib # Windows msvc
        # LIBS += ./qworksheet/release/libqworksheet.a # Mingw or GCC 
        LIBS += -lsetupapi -ladvapi32 -luser32
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    testlib_global.h \
    testlib.h \
    qworksheet.h \



FORMS    += mainwindow.ui
