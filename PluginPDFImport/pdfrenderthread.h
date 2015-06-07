#ifndef PDFRENDERTHREAD_H
#define PDFRENDERTHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QDir>

#include "typedefs.h"

class PDFRenderThread : public QThread
{
    Q_OBJECT

public:
    PDFRenderThread(QString pdfFileName, IntList pageList, int *incV, QMutex *incM, const QDir &tempDir, int dpi = 300);
    void stop();

    QStringList &processedFiles() { return mProcessedFiles; }

protected:
    void run();

private:
    QStringList mProcessedFiles;
    QString mPdfFileName;
    IntList mPageList;
    QMutex* mIncMutex;
    int *mIncValue;
    int mDpi;
    QDir mDir;
    volatile bool stopped;
};

#endif // PDFRENDERTHREAD_H
