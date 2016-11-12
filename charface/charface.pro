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
    Models/DocumentModel.cpp \
    Models/PageModel.cpp \
    Models/PageZoneModel.cpp \
    numbersegment.cpp \
    pluginmanager.cpp \
    settingsmanager.cpp \
    UI/MainWindow/DocumentListView/DocumentListView.cpp \
    UI/MainWindow/DocumentListView/PageItemWidget/PageItemWidget.cpp \
    UI/MainWindow/MainWindow.cpp \
    UI/MainWindow/PageGraphicsScene/PageGraphicsScene.cpp \
    UI/MainWindow/PageGraphicsScene/PageZoneView/PageZoneView.h.cpp \
    UI/MainWindow/PageView/PageView.cpp \
    UI/PluginAboutDialog/PluginAboutDialog.cpp \
    UI/PluginsWindow/PluginsWindow.cpp \
    UI/ProgressDialog/ProgressDialog.cpp \
    utilities.cpp

HEADERS += \
    applicationmanager.h \
    cfplugininterface.h \
    Models/DocumentModel.h \
    Models/PageModel.h \
    Models/PageZoneModel.h \
    numbersegment.h \
    pluginmanager.h \
    settingsmanager.h \
    typedefs.h \
    UI/MainWindow/DocumentListView/DocumentListView.h \
    UI/MainWindow/DocumentListView/PageItemWidget/PageItemWidget.h \
    UI/MainWindow/MainWindow.h \
    UI/MainWindow/PageGraphicsScene/PageGraphicsScene.h \
    UI/MainWindow/PageGraphicsScene/PageZoneView/PageZoneView.h \
    UI/MainWindow/PageView/PageView.h \
    UI/PluginAboutDialog/PluginAboutDialog.h \
    UI/PluginsWindow/PluginsWindow.h \
    UI/ProgressDialog/ProgressDialog.h \
    utilities.h

FORMS += \
    UI/MainWindow/DocumentListView/PageItemWidget/PageItemWidget.ui \
    UI/MainWindow/MainWindow.ui \
    UI/PluginAboutDialog/PluginAboutDialog.ui \
    UI/PluginsWindow/PluginsWindow.ui \
    UI/ProgressDialog/ProgressDialog.ui

RESOURCES += \
    icons.qrc
