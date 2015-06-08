#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QMultiMap>

#include "cfplugin.h"

#define pluginManager (PluginManager::instance())

typedef QList<CFPlugin*> PluginsList;
typedef QMultiMap<PluginType, CFPlugin*> PluginsMap;

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

    CFPluginOCR *defaultOCRPlugin() { return mDefaultOCRPlugin;}
    void setDefaultOCRPlugin(CFPluginOCR *plugin) { mDefaultOCRPlugin = plugin; }

    CFPluginAnalyze *defaultAnalyzePlugin() { return mDefaultAnalyzePlugin;}
    void setDefaultAnalyzePlugin(CFPluginAnalyze *plugin) { mDefaultAnalyzePlugin = plugin; }

    void loadPluginsFromDir(const QString &dir);

signals:
    void pluginsListChanged();
    void pluginIsBeingLoadedFromFile(const QString&);

private:
    static PluginManager *mInstance;
    PluginManager();
    ~PluginManager();

    void addPLugin(CFPlugin *plugin);
    PluginsMap mPluginsMap;

    bool mEmitPluginsChangedSignal;

    CFPluginOCR *mDefaultOCRPlugin;
    CFPluginAnalyze *mDefaultAnalyzePlugin;
};

#endif // PLUGINMANAGER_H
