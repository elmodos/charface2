#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QtWidgets/QLabel>
#include <QDir>

#include <cfplugin.h>

class CFPluginPDFImport : public CFPluginImport
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginPDFImport();
    ~CFPluginPDFImport();

    QString name() const { return "PDF Import plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.1.0"; }

    QString author() const { return "Modo Ltunzher, Serhiy Moroz"; }

    QIcon icon() const { return QIcon::fromTheme("application-pdf"); }

    QString actionTitle() const { return "Import DPF"; }

    QStringList doImportFiles(const QString &tempDir);

};

#endif // MYPLUGIN_H
