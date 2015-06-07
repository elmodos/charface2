#include <QDir>
#include <QIcon>
#include <QDebug>
#include <QApplication>
#include <QDesktopServices>
#include <QDir>

#include "utilities.h"
#include "settingsmanager.h"

SettingsManager *SettingsManager::mInstance = NULL;

SettingsManager::SettingsManager() :
    QObject(0)
{
    //icon theme
    QIcon::setThemeName(Utilities::getCurrentIconsThemeName());

    QString appName = qApp->applicationName().toLower();

    mDefaultApplicationDataPath = QDir(getUserDataLocation()).absoluteFilePath(appName);
    mDefaultBatchPath = QDir(mDefaultApplicationDataPath).absoluteFilePath("batch");

    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation); // QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    mDefaultTempPath = QDir(tempDir).absoluteFilePath(appName);

    mSettings.lastUsedAnalyzeAll = true;
    mSettings.lastUsedReadAll = false;
}

SettingsManager::~SettingsManager()
{
}

QString SettingsManager::dirMustExist(const QString &path)
{
    QDir dir;
    dir.mkpath(path);
    return path;
}

void SettingsManager::createInstance()
{
    if (!mInstance)
        mInstance = new SettingsManager();
}

void SettingsManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

SettingsManager *SettingsManager::instance()
{
    if (mInstance == NULL)
        createInstance();
    return mInstance;
}

const QString SettingsManager::defaultApplicationDataPath()
{
    return dirMustExist(mDefaultApplicationDataPath);
//                "/home/elmodos/.charface2");
}

const QString SettingsManager::defaultBatchPath()
{
    return dirMustExist(mDefaultBatchPath);
//                "/home/elmodos/.charface2/batch");
}

const QString SettingsManager::defaultTempPath()
{

    return dirMustExist(mDefaultTempPath);
}

QImage::Format SettingsManager::imageFormatForBatch()
{
    return QImage::Format_RGB16;
}

const QString SettingsManager::imageFormatExt()
{
    return "tiff";
}

const QString SettingsManager::thumbFormatExt()
{
    return "png";
}

const QString SettingsManager::thumbNamePrefix()
{
    return "_thumb";
}

const QSize SettingsManager::thumbSize()
{
    return QSize(128,128);
}

const QString SettingsManager::mainWindowTitle()
{
    return "Charface2 devel";
}

const QStringList SettingsManager::pluginSearchPaths()
{
    QStringList list;
    list
            << "/lib"
            << "/usr/lib"
            << "/usr/local/lib";

    return list;
}

const QString SettingsManager::pluginsInstallPath()
{
    QDir appDir = defaultApplicationDataPath();
    QString pluginsInstallPath = appDir.absoluteFilePath("plugins");
    return dirMustExist(pluginsInstallPath);
}

qreal SettingsManager::pageScaleMin()
{
    return 0.1;
}

qreal SettingsManager::pageScaleMax()
{
    return 2.0;
}

qreal SettingsManager::pageScaleMultiplier()
{
    return 0.1;
}

qreal SettingsManager::zoneRectEdgeMagnetDistance()
{
    return 10.0;
}

qreal SettingsManager::rectItemEdgeWidth()
{
    return 2.0f;
}

int SettingsManager::zoneMinEdgeSize()
{
    return 2 * zoneRectEdgeMagnetDistance();
}

bool SettingsManager::replaceZonesOnAnalyze()
{
    return true;
}

bool SettingsManager::lastUsedReadAll()
{
    return mSettings.lastUsedReadAll;
}

bool SettingsManager::lastUsedAnalyzeAll()
{
    return mSettings.lastUsedAnalyzeAll;
}

QString SettingsManager::getUserDataLocation()
{
#if defined(Q_OS_LINUX) || defined(Q_WS_MACX)

    return QDir(QDir::homePath()).absoluteFilePath(".local/share");

#elif defined(Q_WS_WIN)

    return QDesktopServices::storageLocation(QDesktopServices::DataLocation);

#else

#error Are you going to install Charface on your microwave?

#endif
}
