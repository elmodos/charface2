TEMPLATE = lib
TARGET = cfPluginImportDemo
CONFIG += plugin release
VERSION = 0.0.1

INSTALLS += target

HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h

SOURCES += \
    myplugin.cpp \
