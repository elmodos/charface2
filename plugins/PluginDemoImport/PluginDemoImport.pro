TEMPLATE = lib
QT += core gui widgets
TARGET = cfPluginImportDemo
CONFIG += plugin debug
VERSION = 0.0.0

INCLUDEPATH += ../../charface

HEADERS += myplugin.h
SOURCES += myplugin.cpp

unix:!macx { # Linux
    CONFIG += c++11
}

macx: {
    LIBS += -stdlib=libc++
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_CXXFLAGS += -std=c++11
}
