TEMPLATE = lib
TARGET = CharfacePluginAnalyzeStock
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

#need to get rid of this
QT += core gui xml

INSTALLS += target

#LIBS += -l

HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h \
    ../charface/zone.h \
    settingsdialog.h \
    leptonicabackend.h

SOURCES += \
    myplugin.cpp \
    settingsdialog.cpp \
    leptonicabackend.cpp \
    ../charface/zone.cpp

FORMS += \
    settingsdialog.ui

#INCLUDEPATH +=
