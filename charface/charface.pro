QT       += core gui xml widgets

TARGET = charface2
DESTDIR = ../bin
TEMPLATE = app

# INCLUDEPATH +=

unix:!macx { # Linux
    CONFIG += c++11
}

macx: {
    LIBS += -stdlib=libc++
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += \
    main.cpp \
    applicationmanager.cpp \
    pluginmanager.cpp \
    mainwindow.cpp \
    pluginswindow.cpp \
    pluginaboutdialog.cpp \
    batch.cpp \
    page.cpp \
    settingsmanager.cpp \
    zone.cpp \
    utilities.cpp \
    numbersegment.cpp \
    progressdialog.cpp \
    pagegraphicsscene.cpp \
    zonerectitem.cpp \
    pagelistview.cpp \
    pageitemwidget.cpp \
    pagegraphicsview.cpp

HEADERS += \
    mainwindow.h \
    applicationmanager.h \
    pluginmanager.h \
    pluginswindow.h \
    pluginaboutdialog.h \
    batch.h \
    page.h \
    settingsmanager.h \
    zone.h \
    utilities.h \
    numbersegment.h \
    progressdialog.h \
    pagegraphicsscene.h \
    zonerectitem.h \
    pagelistview.h \
    pageitemwidget.h \
    typedefs.h \
    pagegraphicsview.h \
    cfplugininterface.h

FORMS += \
    mainwindow.ui \
    pluginswindow.ui \
    pluginaboutdialog.ui \
    progressdialog.ui \
    scrollitem.ui

RESOURCES += \
    icons.qrc
