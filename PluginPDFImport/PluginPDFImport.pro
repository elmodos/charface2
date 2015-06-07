TEMPLATE = lib
TARGET = CharfacePluginImportPDF
CONFIG += plugin release
VERSION = 0.0.1
DESTDIR = ../plugins

INSTALLS += target

INCLUDEPATH += \
    ../charface \
    /usr/include/poppler/qt4

LIBS += \
    -L/usr/lib -lpoppler-qt4


HEADERS += \
    myplugin.h \
    ../charface/cfplugin.h \
    plugindialog.h \
    ../charface/numbersegment.h \
    pdfrenderthread.h

SOURCES += \
    myplugin.cpp \
    plugindialog.cpp \
    ../charface/numbersegment.cpp \
    pdfrenderthread.cpp

FORMS += \
    plugindialog.ui
