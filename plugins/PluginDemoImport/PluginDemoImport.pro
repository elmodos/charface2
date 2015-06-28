TEMPLATE = lib
QT += core gui widgets
TARGET = cfPluginImportDemo
CONFIG += plugin debug
VERSION = 0.0.0

INCLUDEPATH += ../../charface

HEADERS += myplugin.h
SOURCES += myplugin.cpp

LIBS += -stdlib=libc++

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
