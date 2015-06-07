#include <QDebug>
#include <QPluginLoader>
#include <QDir>

#include "pluginmanager.h"

PluginManager * PluginManager::mInstance = NULL;

PluginManager::PluginManager()
{
    //make CFPlugin* visible for QVariant::fromValue()
    qRegisterMetaType<CFPlugin*>();

    //this value will be set to false while adding a batch of plugins
    mEmitPluginsChangedSignal = true;

    //
    mDefaultOCRPlugin = NULL;
    mDefaultAnalyzePlugin = NULL;
}

PluginManager::~PluginManager()
{
}

void PluginManager::addPLugin(CFPlugin *plugin)
{
    // take care of this baby
    plugin->setParent(this);

    // categorize
    PluginType pluginType = plugin->pluginType();
    qDebug() << "Adding plugin instance, type" << CFPlugin::pluginTypeToSting(pluginType) << "\n";

    //store
    mPluginsMap.insert(PT_All, plugin);
    mPluginsMap.insert(pluginType, plugin);

    //set default OCR plugin if it is not set
    if (!mDefaultOCRPlugin)
    {
        CFPluginOCR *pluginOCR = dynamic_cast<CFPluginOCR*>(plugin);
        if (pluginOCR) setDefaultOCRPlugin(pluginOCR);
    }

    //set default Analyze plugin if it is not set
    if (!mDefaultAnalyzePlugin)
    {
        CFPluginAnalyze *pluginAnalyze = dynamic_cast<CFPluginAnalyze*>(plugin);
        if (pluginAnalyze) setDefaultAnalyzePlugin(pluginAnalyze);
    }

}

void PluginManager::createInstance()
{
    if (!mInstance)
        mInstance = new PluginManager();
}

void PluginManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

PluginManager *PluginManager::instance()
{
    if (mInstance == NULL)
        createInstance();
    return mInstance;
}

bool PluginManager::registerPlugin(const QString &filePath)
{
    bool pluginWasLoaded = false;

    //load plugin
    QPluginLoader *pluginLoader = new QPluginLoader(filePath);
    QObject *plugin = pluginLoader->instance();

    if (!plugin)
    {
        qDebug() << "Load plugin error message: " << pluginLoader->errorString() << "\n";
    }
    else
    {
        CFPlugin *pluginInstance = dynamic_cast<CFPlugin*>(plugin);

        //plugin is CFPlugin instance
        if (pluginInstance)
        {
            pluginWasLoaded = true;
            qDebug() << "Plugin successfully loaded" << pluginInstance->name();

            //store
            addPLugin(pluginInstance);

            if (mEmitPluginsChangedSignal)
                emit pluginsListChanged();
        }
    }

    return pluginWasLoaded;
}

bool PluginManager::registerPluginsFromDirectory(const QString &dirPath)
{
    QDir dir(dirPath);

    if (!dir.exists()) return false;

    //filter
    QStringList filters;
    filters << "*.so" << "*.dll";
    dir.setNameFilters(filters);

    dir.setFilter(QDir::Files | QDir::Readable);

    QStringList files = dir.entryList(QDir::Files | QDir::Readable);
    qDebug() << files;

    //prevent of multiple signal emitation
    mEmitPluginsChangedSignal = false;
    bool pluginsChanged = false;

    foreach(QString fileName, files)
    {
        QString fullName = dir.filePath(fileName);
        qDebug() << fullName;
        pluginsChanged = pluginsChanged || registerPlugin(fullName);
    }

    mEmitPluginsChangedSignal = true;
    //

    if (pluginsChanged)
        emit pluginsListChanged();

    return true;
}

PluginsList PluginManager::plugins(PluginType pt) const
{
    return mPluginsMap.values(pt);
}

void PluginManager::loadPluginsFromDir(const QString &dir)
{
    //
    QDir pluginsDir(dir, "*.so");

    // get files list
    QStringList list = pluginsDir.entryList();

    // load each plugin
    foreach(QString name, list)
    {
        emit pluginIsBeingLoadedFromFile(name);
        qDebug() << "Loading plugin" << name;
        PluginManager::instance()->registerPlugin( pluginsDir.filePath(name) );
    }
}

