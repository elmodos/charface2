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
    documentmodel.cpp \
    settingsmanager.cpp \
    utilities.cpp \
    numbersegment.cpp \
    progressdialog.cpp \
    pagegraphicsscene.cpp \
    pageitemwidget.cpp \
    pagemodel.cpp \
    pagezonemodel.cpp \
    pagezoneview.cpp \
    pageview.cpp \
    documentlistview.cpp

HEADERS += \
    mainwindow.h \
    applicationmanager.h \
    pluginmanager.h \
    pluginswindow.h \
    pluginaboutdialog.h \
    documentmodel.h \
    settingsmanager.h \
    utilities.h \
    numbersegment.h \
    progressdialog.h \
    pagegraphicsscene.h \
    pageitemwidget.h \
    typedefs.h \
    cfplugininterface.h \
    pagemodel.h \
    pagezonemodel.h \
    pagezoneview.h \
    pageview.h \
    documentlistview.h

FORMS += \
    mainwindow.ui \
    pluginswindow.ui \
    pluginaboutdialog.ui \
    progressdialog.ui \
    scrollitem.ui

RESOURCES += \
    icons.qrc
