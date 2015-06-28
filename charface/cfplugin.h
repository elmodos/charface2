#ifndef CFPLUGIN_H
#define CFPLUGIN_H

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

//
class Zone;
typedef QList<Zone> ZoneList;

//
class CFPlugin: public QObject
{
    Q_OBJECT

public:
    static const QString pluginTypeToSting(const PluginType pt)
    {
        switch(pt)
        {
            case PT_All:return "All plugins";
            case PT_Import:return "Import plugins";
            case PT_ImageEdit:return "Image manipulation plugins";
            case PT_Analyze:return "Image structure analyzing plugins";
            case PT_OCR:return "OCR plugins";
            case PT_TextPostprocessing:return "Text postprocessing plugins";
            case PT_Export:return "Export plugins";
            default:return "Unknown";
        }
    }

    //
    //CFPlugin() {}
    //virtual ~CFPlugin() = 0;//{}

    // Supported actions by this plugin
    virtual PluginType pluginType() const = 0;

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
    virtual QIcon icon() const = 0;// { return QIcon(); }

    //Action text
    virtual QString actionTitle() const = 0;// { return name(); }
};


class CFPluginImport : public CFPlugin
{
    Q_OBJECT

public:

    //plugin type
    virtual PluginType pluginType() const = 0;// { return PT_Import; }

    //store files list and return paths, files in temp dir fill be deleted automatically
    virtual QStringList doImportFiles(const QString &tempDir) = 0;

};

class CFPluginAnalyze : public CFPlugin
{
    Q_OBJECT

public:

    //plugin type
    PluginType pluginType() const { return PT_Analyze; }

    //
    virtual bool doAnalyze(QImage &img, ZoneList &result) = 0;

    //
    void showSettingsDialog() { /*Do nothing if plugin is not customizable*/ }
};


class CFPluginOCR : public CFPlugin
{
    Q_OBJECT

public:

    //plugin type
    PluginType pluginType() const { return PT_OCR; }

    //
    virtual bool doOCR(QImage &img, QString &result) = 0;

    //
    void showSettingsDialog() { /*Do nothing if plugin is not customizable*/ }
};

class CFPluginImageEdit : public CFPlugin
{
    Q_OBJECT

public:

    //plugin type
    PluginType pluginType() const { return PT_ImageEdit; }

    //
    void setPaths(const QString &imageFile, const QString &tempDir = QString()) { mImageFile = imageFile; mTempDir = tempDir; }

    //widget for central area
    virtual QWidget *createWidgetInteraction() = 0;

    //widget for toolbox
    virtual QWidget *createWidgetTool() = 0;

    //apply and return list of created files. i.e. split may return 2 or more file names
    virtual QStringList doEdit() = 0;

protected:
    QString mImageFile;
    QString mTempDir;
};


class CFPluginTextPostprocessing : public CFPlugin
{
    Q_OBJECT

public:

    //plugin type
    PluginType pluginType() const { return PT_TextPostprocessing; }

    //return new string, use some interaction e.g. spell check
    virtual QString doTextPostprocessing(const QString &text) = 0;

};

#define CharfacePluginIID "charface.plugin"
Q_DECLARE_INTERFACE(CFPlugin, CharfacePluginIID)

#endif // CFPLUGIN_H
