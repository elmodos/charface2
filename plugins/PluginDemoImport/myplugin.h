#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <cfplugininterface.h>
#include <QtWidgets/QDialog>

class CFPluginSimpleImport : public QObject, public CFPluginImportInterface
{
    Q_OBJECT
    Q_INTERFACES(CFPluginInterface)
    Q_PLUGIN_METADATA(IID CharfacePluginIID)

public:
    CFPluginSimpleImport();
    ~CFPluginSimpleImport();

    // Supported actions by this plugin
    bool isPluginTypeSupported(PluginType pluginType) const { return pluginType == PT_Import; }

    QString name() const { return "Demo Import plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher"; }

    QIcon icon() const { return QIcon::fromTheme("application-install"); }

    QDialog *aboutDialog() { return nullptr; }

    QString actionTitle() const { return QString("Import files with plugin %1").arg(name()); }

    QStringList doImportFiles(const QString &tempDir);

};

#endif // MYPLUGIN_H
