#include "uimanager.h"
#include "pluginswindow.h"
#include "pluginaboutdialog.h"
#include "mainwindow.h"
#include "cfplugin.h"

UIManager *UIManager::mInstance = NULL;

UIManager::UIManager()
{
    mMainWindow = new MainWindow();
}

UIManager::~UIManager()
{
}

void UIManager::createInstance()
{
    if (!mInstance)
        mInstance = new UIManager;
}

void UIManager::destroyInstance()
{
    if (mInstance)
        delete mInstance;
}

UIManager *UIManager::instance()
{
    if (!mInstance)
        createInstance();
    return mInstance;
}

void UIManager::showPluginsDialog()
{
    PluginsWindow window;
    window.exec();
}

void UIManager::showPluginAboutDialog(CFPlugin *plugin)
{
    PluginAboutDialog dialog(plugin);
    dialog.exec();
}

void UIManager::showMainWindow()
{
    mMainWindow->show();
}
