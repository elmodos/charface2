#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QImage>
#include <QSize>


class SettingsManager : public QObject
{
    Q_OBJECT

public:
    //singleton
    static void createInstance();
    static void destroyInstance();
    static SettingsManager *instance();

    //paths
    const QString defaultApplicationDataPath();
    const QString defaultBatchPath();
    const QString defaultTempPath();

    //image format
    QImage::Format imageFormatForBatch();
    const QString imageFormatExt();

    //thumbs
    const QString thumbFormatExt();
    const QString thumbNamePrefix();
    const QSize thumbSize();

    //strings
    const QString mainWindowTitle();
    const QStringList pluginSearchPaths();
    const QString pluginsInstallPath();

    //page image zoom values
    qreal pageScaleMin();
    qreal pageScaleMax();
    qreal pageScaleMultiplier();

    //items on page
    qreal zoneRectEdgeMagnetDistance();
    qreal rectItemEdgeWidth();
    int zoneMinEdgeSize();

    //analyze plugin
    bool replaceZonesOnAnalyze();

    // toolbar actions
    bool lastUsedReadAll();
    bool lastUsedAnalyzeAll();

    void setLastUsedReadAll(bool val) { mSettings.lastUsedReadAll = val; }
    void setLastUsedAnalyzeAll(bool val) { mSettings.lastUsedAnalyzeAll = val; }

protected:
    struct Settings {
        bool lastUsedReadAll;
        bool lastUsedAnalyzeAll;
    };

    Settings mSettings;

private:
    // default paths
    QString mDefaultApplicationDataPath;
    QString mDefaultBatchPath;
    QString mDefaultTempPath;

    QString getUserDataLocation();

    SettingsManager();
    ~SettingsManager();
    static SettingsManager *mInstance;

    QString dirMustExist(const QString &path);


};

#define settingsManager (SettingsManager::instance())

#endif // SETTINGSMANAGER_H
