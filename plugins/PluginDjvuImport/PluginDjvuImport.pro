TEMPLATE = lib
TARGET = CharfacePluginImportDjvu
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

INSTALLS += target

INCLUDEPATH += ../charface/

LIBS += -ldjvulibre -ltiff

HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h \
    plugindialog.h \
    ../charface/numbersegment.h \
    djvurenderthread.h \
    djvu2tiffexporter.h

SOURCES += \
    myplugin.cpp \
    plugindialog.cpp \
    ../charface/numbersegment.cpp \
    djvurenderthread.cpp \
    djvu2tiffexporter.cpp

FORMS += \
    plugindialog.ui

RESOURCES += \
    resources.qrc
