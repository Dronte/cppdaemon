DEFINES += NDEBUG
unix:QMAKE_CXXFLAGS_DEBUG += -O2
win32:QMAKE_CXXFLAGS_DEBUG += -O2
TEMPLATE = lib
QT -= gui
CONFIG += dll
TARGET = cplusplus
VERSION = 3.2.2

include(cplusplus-lib.pri)
