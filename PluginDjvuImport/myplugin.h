#ifndef MYPLUGIN_H
#define MYPLUGIN_H


#include <QLabel>
#include <QDir>

#include <cfplugin.h>

class CFPluginDjvuImport : public CFPluginImport
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginDjvuImport();
    ~CFPluginDjvuImport();

    QString name() const { return tr("Djvu Import plugin"); }

    QString pluginDescription() const;

    QString version() const { return "0.1.0"; }

    QString author() const { return tr("Serhiy Moroz, Modo Ltunzher"); }

    QIcon icon() const;

    QString actionTitle() const { return "Import Djvu"; }

    QStringList doImportFiles(const QString &tempDir);

};

#endif // MYPLUGIN_H
