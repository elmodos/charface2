#include <QApplication>

//managers
#include "applicationmanager.h"
#include "pluginmanager.h"
#include "settingsmanager.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("ltunzher");
    app.setApplicationName("Charface2");

    ApplicationManager::createInstance();
    SettingsManager::createInstance();
    PluginManager::createInstance();

    // load plugins before creating main window avoiding ui updates
    PluginManager::instance()->loadPluginsFromDir("/usr/share/charface/plugins");
    PluginManager::instance()->loadPluginsFromDir("/usr/local/share/charface/plugins");
    PluginManager::instance()->loadPluginsFromDir("/opt/charface/plugins");

    PluginManager::instance()->loadPluginsFromDir("/home/elmodos/Desktop/build/plugins");

    // Show main window
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    int retVal = app.exec();

    PluginManager::destroyInstance();
    SettingsManager::destroyInstance();
    ApplicationManager::destroyInstance();

    return retVal;
}
