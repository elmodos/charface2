#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QtWidgets/QDialog>
#include <QStringList>
#include <QDir>
#include <QTimer>

#include "pdfrenderthread.h"

namespace Ui {
class PluginDialog;
}

class PluginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginDialog(const QString &tempDirPath);
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
    Ui::PluginDialog *ui;
    QStringList mReturnList;
    QDir mTempDir;
    int mPagesCount;

    IntList mAllPageList;
    QList<PDFRenderThread*> mThreadList;
    QMutex mIncMutex;
    int mProcessedPageCount;

    QTimer *mUpdateTimer;
    QStringList mFileNames;
};

#endif // PLUGINDIALOG_H
