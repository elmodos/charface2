#include <QtCore>
#include <QImage>
#include <QImageWriter>
#include <QDebug>

#include "tiffrenderthread.h"

#define IMAGE_EXT "tiff"

TiffRenderThread::TiffRenderThread(const ImgRefList imgrefList, IntList indexes, int *incV, QMutex *incM, const QDir &tempDir) :
    mImgRefList(imgrefList)
{
    mIncMutex = incM;
    mIncValue = incV;
    mDir = tempDir;
    stopped = false;
    mIndexes = indexes;
}

void TiffRenderThread::run()
{
    //
    mProcessedFiles.clear();

    //suppose everything was ok till now
    bool errorsWereOccured = false;
    Q_UNUSED(errorsWereOccured);

    //enumerating pages
    for (int i = 0; i < mImgRefList.count(); i++)
    {
        if (stopped) break;

        //get next page index
        Img *img = mImgRefList.at(i);

        mIncMutex->lock();
            (*mIncValue)++;
        mIncMutex->unlock();

        //smth went wrong, skip this page
        if (!img)
        {
            errorsWereOccured = true;
            continue;
        }

        //
        int indexFromList = mIndexes.at(i);
        QString outputFileName = QString::number(indexFromList) + "." + IMAGE_EXT;
        QString absoluteOutputFileName = mDir.absoluteFilePath(outputFileName);

        //
        img->save_tiff(absoluteOutputFileName.toStdString().c_str());
        mProcessedFiles.append(absoluteOutputFileName);
    }

    //
    stopped = false;
}

void TiffRenderThread::stop()
{
    stopped = true;
}
