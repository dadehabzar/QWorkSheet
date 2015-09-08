

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
        LIBS += C:\Users\Mohsen\Desktop\build-qworksheet-Qt_5_5_0_msvc2012-Release\release\qworksheet.lib  # Windows msvc
        # LIBS += ./libqworksheet.a # Mingw or GCC 
        LIBS += -lsetupapi -ladvapi32 -luser32
INCLUDEPATH += C:\Users\Mohsen\Desktop\QWorksheet-master
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    testlib_global.h \
    testlib.h \
    qworksheet.h \



FORMS    += mainwindow.ui


