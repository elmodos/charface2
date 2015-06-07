#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QObject>
#include <QAction>
#include <QList>
#include "batch.h"
#include "progressdialog.h"
#include "typedefs.h"

class ApplicationManager : public QObject
{
    Q_OBJECT
public:

    //singleton
    static void createInstance();
    static void destroyInstance();
    static ApplicationManager *instance();

    //
    Batch *batch() { return mBatch; }
    bool batchNew(const QString &path = QString());
    bool batchOpen(const QString &path);
    bool batchSaveAs(const QString &newPath);

    //
    void addFilesToBatch(const QStringList &list, const QString &tempDirPath, bool interactive = false);

    //
    void readPages(const IntList &indexes);
    void analyzePages(const IntList &indexes);

signals:
    void batchModified();
    void progressChanged(int);
    void finishedAnalyze();

private slots:
    void onCancelAnalyze();
    void onFinishedAnalyze();

    void onProgressChanged(int value);

private:
    //
    static ApplicationManager *mInstance;

    //
    void analyzePagesBackground(const IntList &indexes);
    void addFilesToBatchBackground(const QStringList &list, const QString &tempDirPath, bool interactive = false);

    //
    ApplicationManager();
    ~ApplicationManager();

    //
    Batch *mBatch;

    //
    ProgressDialog *mProgressDialog;

    //
    volatile bool mCancelFlag;
};

#define appManager (ApplicationManager::instance())

#endif // APPLICATIONMANAGER_H
