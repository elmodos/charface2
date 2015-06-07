#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

#include "mainwindow.h"
#include "cfplugin.h"

class UIManager : public QObject
{
    Q_OBJECT
public:

    //singleton
    static void createInstance();
    static void destroyInstance();
    static UIManager *instance();

    //about plugin
    void showPluginsDialog();
    void showPluginAboutDialog(CFPlugin *plugin);

    //main window
    void showMainWindow();
    MainWindow *mainWindow() { return mMainWindow; }

private:
    //
    static UIManager *mInstance;

    //
    UIManager();
    ~UIManager();

    //
    MainWindow *mMainWindow;
};

#define uiManager (UIManager::instance())

#endif // UIMANAGER_H
