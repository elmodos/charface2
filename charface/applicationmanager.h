#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QObject>

//#include "batch.h"
#include "progressdialog.h"
#include "typedefs.h"

#define appManager (ApplicationManager::instance())

class DocumentModel;

class ApplicationManager : public QObject
{
    Q_OBJECT
public:

    //singleton
    static void createInstance();
    static void destroyInstance();
    static ApplicationManager *instance();

    //
    bool documentNew(const QString &path = QString());
    bool documentOpen(const QString &path);
    bool documentSaveAs(const QString &newPath);

    //
    void addFilesToDocument(const QStringList &list, const QString &tempDirPath, bool interactive = false);

    //
    void readPages(const IntList &indexes);
    void analyzePages(const IntList &indexes);

signals:
    void documentModified();
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
    DocumentModel *document() { return mDocument; }

    //
    void analyzePagesBackground(const IntList &indexes);
    void addFilesToBatchBackground(const QStringList &list, const QString &tempDirPath, bool interactive = false);

    //
    ApplicationManager();
    ~ApplicationManager();

    //
    DocumentModel *mDocument;

    //
    ProgressDialog *mProgressDialog;

    //
    volatile bool mCancelFlag;
};


#endif // APPLICATIONMANAGER_H
