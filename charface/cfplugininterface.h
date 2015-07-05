#ifndef CFPLUGININTERFACE_H
#define CFPLUGININTERFACE_H

#include <QString>
#include <QIcon>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QList>

//
enum
{
    PT_All = 0,

    //
    PT_Import,
    PT_ImageEdit,
    PT_Analyze,
    PT_OCR,
    PT_TextPostprocessing,
    PT_Export,

    //
    PT_Count
};
typedef int PluginType;

class Zone;
typedef QList<Zone> ZoneList;

//
class CFPluginInterface
{
public:

    // Supported actions by this plugin
    virtual bool isPluginTypeSupported(const PluginType pluginType) const = 0;

    // Plugin name like "Default stock plugin"
    virtual QString name() const = 0;

    // Description
    virtual QString pluginDescription() const = 0;

    //Version
    virtual QString version() const = 0;

    //Author
    virtual QString author() const = 0;

    //if aboutDialog() returns NULL standard dialog will be used with pluginDescription()
    virtual QDialog *aboutDialog() = 0;

    //Icon for menu entry, toolbar, etc
    virtual QIcon icon() const = 0;

    //Action text
    virtual QString actionTitle() const = 0;
};

#define CharfacePluginIID "charface.plugin"
Q_DECLARE_INTERFACE(CFPluginInterface, CharfacePluginIID)


//
class CFPluginImportInterface : public CFPluginInterface
{
public:

    //store files list and return paths, files in temp dir fill be deleted automatically
    virtual QStringList doImportFiles(const QString &tempDir) = 0;
};

class CFPluginAnalyzeInterface : public CFPluginInterface
{
public:

    //
    virtual bool doAnalyze(QImage &img, ZoneList &result) = 0;

    //
    virtual void showSettingsDialog() = 0;
};


class CFPluginOCRInterface : public CFPluginInterface
{
public:

    //
    virtual bool doOCR(QImage &img, QString &result) = 0;

    //
    virtual void showSettingsDialog() = 0;
};

class CFPluginImageEditInterface : public CFPluginInterface
{
public:

    //
    virtual void setPaths(const QString &imageFile, const QString &tempDir = QString()) = 0;

    //widget for central area
    virtual QWidget *createWidgetInteraction() = 0;

    //widget for toolbox
    virtual QWidget *createWidgetTool() = 0;

    //apply and return list of created files. i.e. split may return 2 or more file names
    virtual QStringList doEdit() = 0;
};


class CFPluginTextPostprocessingInterface : public CFPluginInterface
{
public:

    //return new string, use some interaction e.g. spell check
    virtual QString doTextPostprocessing(const QString &text) = 0;
};

#endif // CFPLUGININTERFACE_H
