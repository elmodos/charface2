#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QMultiMap>

#include "cfplugininterface.h"

#define pluginManager (PluginManager::instance())

typedef QList<CFPluginInterface *> PluginsList;
typedef QMultiMap<PluginType, void *> PluginsMap; //TODO: I had trobles with adding pure virual with metatype

class PluginManager : public QObject
{
    Q_OBJECT

public:
    static void createInstance();
    static void destroyInstance();
    static PluginManager *instance();

    bool registerPlugin(const QString &filePath);
    bool registerPluginsFromDirectory(const QString &dirPath);
    PluginsList plugins(PluginType pt = PT_All) const;

    CFPluginOCRInterface *defaultOCRPlugin() { return mDefaultOCRPlugin;}
    void setDefaultOCRPlugin(CFPluginOCRInterface *plugin) { mDefaultOCRPlugin = plugin; }

    CFPluginAnalyzeInterface *defaultAnalyzePlugin() { return mDefaultAnalyzePlugin;}
    void setDefaultAnalyzePlugin(CFPluginAnalyzeInterface *plugin) { mDefaultAnalyzePlugin = plugin; }

    void loadPluginsFromDir(const QString &dir);

signals:
    void pluginsListChanged();
    void pluginIsBeingLoadedFromFile(const QString&);

private:
    static PluginManager *mInstance;
    PluginManager();
    ~PluginManager();

    void addPLugin(CFPluginInterface *plugin);
    PluginsMap mPluginsMap;

    bool mEmitPluginsChangedSignal;

    CFPluginOCRInterface *mDefaultOCRPlugin;
    CFPluginAnalyzeInterface *mDefaultAnalyzePlugin;
};

#endif // PLUGINMANAGER_H
