TEMPLATE = lib
TARGET = CharfacePluginOCRCuneiform
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

INSTALLS += target

LIBS += -lcuneiform

HEADERS += \
    myplugin.h \
    cuneiformbackend.h \
    ../charface/cfplugin.h

SOURCES += \
    myplugin.cpp \
    cuneiformbackend.cpp

FORMS +=

INCLUDEPATH += cuneiform
