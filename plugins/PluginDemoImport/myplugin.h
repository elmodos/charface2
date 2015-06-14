#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <cfplugin.h>

class CFPluginSimpleImport : public CFPluginImport
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)
    Q_PLUGIN_METADATA(IID CharfacePluginIID)

public:
    CFPluginSimpleImport();
    ~CFPluginSimpleImport();

    QString name() const { return "Demo Import plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher"; }

    QIcon icon() const { return QIcon::fromTheme("application-install"); }

    QString actionTitle() const { return QString("Import files with plugin %1").arg(name()); }

    QStringList doImportFiles(const QString &tempDir = QString());

};

#endif // MYPLUGIN_H
