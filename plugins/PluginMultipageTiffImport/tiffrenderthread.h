#ifndef TIFFRENDERTHREAD_H
#define TIFFRENDERTHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QStringList>
#include <QDir>
#include "typedefs.h"

//use libtiff to be able read more than 1 page in tiff file
#define cimg_use_tiff
#include <CImg.h>

using namespace cimg_library;

typedef uint8 cimgint;

typedef CImg<cimgint> Img;

typedef QList<Img*> ImgRefList;

class TiffRenderThread : public QThread
{
    Q_OBJECT

public:
    TiffRenderThread(const ImgRefList imgrefList, IntList indexes, int *incV, QMutex *incM, const QDir &tempDir);
    void stop();

    QStringList &processedFiles() { return mProcessedFiles; }

protected:
    void run();

private:
    QStringList mProcessedFiles;
    QMutex* mIncMutex;
    int *mIncValue;
    int mDpi;
    QDir mDir;
    IntList mIndexes;
    const ImgRefList mImgRefList;
    volatile bool stopped;
};

#endif // TIFFRENDERTHREAD_H
