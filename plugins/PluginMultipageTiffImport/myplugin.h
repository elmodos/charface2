#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QLabel>
#include <QDir>

#include <cfplugin.h>

class CFPluginTIFFImport : public CFPluginImport
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginTIFFImport();
    ~CFPluginTIFFImport();

    QString name() const { return "Multipage TIFF Import plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher, Serhiy Moroz"; }

    QIcon icon() const { return QIcon::fromTheme("image-tiff"); }

    QString actionTitle() const { return "Import multipage TIFF"; }

    QStringList doImportFiles(const QString &tempDir);

};

#endif // MYPLUGIN_H
