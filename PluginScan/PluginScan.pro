TEMPLATE = lib
TARGET = CharfacePluginImportScanner
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

INSTALLS += target

HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h \
    scannerdialog.h

SOURCES += \
    myplugin.cpp \
    scannerdialog.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    scannerdialog.ui
