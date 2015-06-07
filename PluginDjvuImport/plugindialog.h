#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDir>
#include <QTimer>

#include <cfplugin.h>
#include "djvurenderthread.h"

namespace Ui {
class PluginDialog;
}

class CFPluginDjvuImport;

class PluginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginDialog(const QString &tempDirPath, CFPluginDjvuImport *plugin);
    ~PluginDialog();
    
    QStringList fileNames() { return mFileNames; }

private slots:
    void onBrowse();
    void onStart();
    void onCancel();
    void onPagesChanged(const QString &newText);
    void updateProgress();

protected:
    void checkFile(const QString &fileName);
    void processFile(const QString &fileName);
    int getCpuCount();

    void setUiLocked(bool val);
    void stopAllThreads();

    void closeEvent(QCloseEvent *);

private:

    CFPlugin *mPlugin;

    Ui::PluginDialog *ui;
    QStringList mReturnList;
    QDir mTempDir;
    int mPagesCount;

    IntList mAllPageList;
    QList<DjvuRenderThread*> mThreadList;
    QMutex mIncMutex;
    int mProcessedPageCount;

    QTimer *mUpdateTimer;
    QStringList mFileNames;
};

#endif // PLUGINDIALOG_H
