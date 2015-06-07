#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "../charface/cfplugin.h"

class CFPluginSimpleImport : public CFPluginImport
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginSimpleImport();

    QString name() const { return "Scanner interface"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher"; }

    QIcon icon() const { return QIcon(":/scanner.png"); }

    QString actionTitle() const { return "Scan"; }

    QStringList doImportFiles(const QString &tempDir = QString());

};

#endif // MYPLUGIN_H
