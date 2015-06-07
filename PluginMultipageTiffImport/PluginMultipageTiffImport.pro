TEMPLATE = lib
TARGET = CharfacePluginImportTiff
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

INSTALLS += target

INCLUDEPATH += "../charface"

LIBS += -L/usr/lib -lpthread -lX11 -ltiff

HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h \
    plugindialog.h \
    ../charface/numbersegment.h \
    tiffrenderthread.h

SOURCES += \
    myplugin.cpp \
    plugindialog.cpp \
    ../charface/numbersegment.cpp \
    tiffrenderthread.cpp

FORMS += \
    plugindialog.ui
