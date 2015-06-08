#include "djvurenderthread.h"
#include <QtCore>
#include <iostream>
#include <QImage>
#include <QImageWriter>
#include <QDebug>

#include "djvu2tiffexporter.h"

#define IMAGE_EXT "tiff"

DjvuRenderThread::DjvuRenderThread(QString djvuFileName, IntList pageList, int *incV,
                                   QMutex *incM, const QDir &tempDir, int dpi):
    mDjvuFileName(djvuFileName),
    mIncMutex(incM),
    mIncValue(incV),
    mDpi(dpi),
    mDir(tempDir),
    stopped(false),
    mPageList(pageList) // Copy indexes
{
}

void DjvuRenderThread::run()
{
    //
    mProcessedFiles.clear();
    Djvu2TiffExporter djvuExporter;

    //try load document
    //document was not loaded
    if (! djvuExporter.load(qApp->arguments().at(0), mDjvuFileName))
        return ;

    //suppose everything was ok till now
    bool errorsWereOccured = false;

    //enumerating pages
    for (int i = 0; i < mPageList.count(); i++)
    {
        if (stopped) break;

        //get next page index
        int indexFromList = mPageList.at(i);
        QString outputFileName = QString::number(indexFromList) + "." + IMAGE_EXT;
        QString absoluteOutputFileName = mDir.absoluteFilePath(outputFileName);


        mIncMutex->lock();
            (*mIncValue)++;
        mIncMutex->unlock();

        //export page
        if (djvuExporter.exportPage(indexFromList, absoluteOutputFileName, mDpi))
        {
            mProcessedFiles.append(absoluteOutputFileName);
        }
        else
        {
            errorsWereOccured = true;
            continue;
        }

    }

    Q_UNUSED(errorsWereOccured);

    stopped = false;
}

void DjvuRenderThread::stop()
{
    stopped = true;
}
