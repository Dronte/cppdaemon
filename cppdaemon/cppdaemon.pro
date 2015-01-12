#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T00:50:35
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = cppdaemon
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp \
#    cpphelper.cpp \
    socketlistener.cpp \
    api.cpp \
    task.cpp \
    project.cpp \
    snapshotupdater.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../cplusplus/release/ -lcplusplus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../cplusplus/debug/ -lcplusplus
else:unix: LIBS += -L$$PWD/../cplusplus/ -lcplusplus

INCLUDEPATH += $$PWD/../cplusplus
INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../3rdparty/
DEPENDPATH += $$PWD/../cplusplus/
DEPENDPATH += $$PWD/../3rdparty/

HEADERS += \
#    cpphelper.h \
    project.h \
    socketlistener.h \
    api.h \
    task.h \
    snapshotupdater.h
