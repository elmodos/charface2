#include <QDebug>
#include <QPluginLoader>
#include <QDir>

#include "pluginmanager.h"

PluginManager * PluginManager::mInstance = NULL;

//Q_DECLARE_METATYPE(CFPluginInterface)

PluginManager::PluginManager()
{
    // TODO: CFPlugin is not a QObject any more
    //make CFPlugin* visible for QVariant::fromValue()
    //qRegisterMetaType<CFPluginInterface *>();

    //this value will be set to false while adding a batch of plugins
    mEmitPluginsChangedSignal = true;

    //
    mDefaultOCRPlugin = NULL;
    mDefaultAnalyzePlugin = NULL;
}

PluginManager::~PluginManager()
{
}

void PluginManager::addPLugin(CFPluginInterface *plugin)
{
    // take care of this baby
    QObject *pluginObject = dynamic_cast<QObject *>(plugin);
    if (pluginObject)
    {
        pluginObject->setParent(this);
    }

    //store
    void *pointer = static_cast<void *>(plugin);
    mPluginsMap.insert(PT_All, pointer);

    // categorize
    for (int i = PT_All + 1; i < PT_Count; i++)
    {
        PluginType pluginType = (PluginType)i;
        if (plugin->isPluginTypeSupported(pluginType))
        {
            mPluginsMap.insert(pluginType, pointer);
        }
    }

    //set default OCR plugin if it is not set
    if (!mDefaultOCRPlugin)
    {
        CFPluginOCRInterface *pluginOCR = dynamic_cast<CFPluginOCRInterface *>(plugin);
        if (pluginOCR)
            setDefaultOCRPlugin(pluginOCR);
    }

    //set default Analyze plugin if it is not set
    if (!mDefaultAnalyzePlugin)
    {
        CFPluginAnalyzeInterface *pluginAnalyze = dynamic_cast<CFPluginAnalyzeInterface *>(plugin);
        if (pluginAnalyze)
            setDefaultAnalyzePlugin(pluginAnalyze);
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
        CFPluginInterface *pluginInstance = dynamic_cast<CFPluginInterface *>(plugin);

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
    filters << "*.so" << "*.dll" << "*.dylib";
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
        qDebug() << "Full plugin path" << fullName;
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
    // TODO: speed up later
    PluginsList result;
    foreach (void *pointer, mPluginsMap.values(pt))
    {
        auto pluginPointer = static_cast<CFPluginInterface *>(pointer);
        if (pluginPointer)
        {
            result.append(pluginPointer);
        }
    }

    return result;
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
        qDebug() << "Loading plugin" << name;
        PluginManager::instance()->registerPlugin( pluginsDir.filePath(name) );
        emit pluginIsBeingLoadedFromFile(name);
    }
}

