#ifndef DJVURENERTHREAD_H
#define DJVURENERTHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QDir>

#include "typedefs.h"

class DjvuRenderThread : public QThread
{
    Q_OBJECT

public:
    DjvuRenderThread(QString pdfFileName, IntList pageList, int *incV, QMutex *incM, const QDir &tempDir, int dpi = 300);
    void stop();

    QStringList &processedFiles() { return mProcessedFiles; }

protected:
    void run();

private:
    QStringList mProcessedFiles;
    QString mDjvuFileName;
    IntList mPageList;
    QMutex* mIncMutex;
    int *mIncValue;
    int mDpi;
    QDir mDir;
    volatile bool stopped;
};

#endif // DJVURENERTHREAD_H
