TEMPLATE = lib
TARGET = CharfacePluginImageEditColor
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

HEADERS += \
    ../charface/cfplugin.h \
    myplugin.h \
    interactivewidget.h

SOURCES += \
    myplugin.cpp \
    interactivewidget.cpp

FORMS += \
    interactivewidget.ui
