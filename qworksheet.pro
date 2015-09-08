#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T14:28:43
#
#-------------------------------------------------
QT      += sql
QT      += widgets

TARGET = qworksheet
TEMPLATE = lib

CONFIG           += debug_and_release
CONFIG           += build_all

DEFINES += QWORKSHEET_LIBRARY

SOURCES += \
    qworksheet.cpp \
    exportexcelobject.cpp \
    storage.cpp \
    sheetvar.cpp \
    general.cpp

HEADERS +=\
    qworksheet.h \
    exportexcelobject.h \
    storage.h \
    sheetvar.h \
    general.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

RESOURCES += \
    media.qrc
