#include <QtCore>
#include <QImage>
#include <QImageWriter>
#include <QDebug>
#include <poppler-qt4.h>

#include "pdfrenderthread.h"

#define IMAGE_EXT "tiff"

PDFRenderThread::PDFRenderThread(QString pdfFileName, IntList pageList, int *incV, QMutex *incM, const QDir &tempDir, int dpi)
{
    mPdfFileName = pdfFileName;
    mIncMutex = incM;
    mIncValue = incV;
    mDpi = dpi;
    mDir = tempDir;
    stopped = false;

    //copy indexes
    mPageList = pageList;
}

void PDFRenderThread::run()
{
    //
    mProcessedFiles.clear();

    //try load document
    Poppler::Document *document = Poppler::Document::load(mPdfFileName);

    //document was not loaded
    if (!document)
        return ;

    //suppose everything was ok till now
    bool errorsWereOccured = false;
    Q_UNUSED(errorsWereOccured);

    //enumerating pages
    for (int i = 0; i < mPageList.count(); i++)
    {
        if (stopped) break;

        //get next page index
        int indexFromList = mPageList.at(i);

        mIncMutex->lock();
            (*mIncValue)++;
        mIncMutex->unlock();

        //load page
        Poppler::Page *pdfPage = document->page(indexFromList);

        //smth went wrong, skip this page
        if (!pdfPage)
        {
            errorsWereOccured = true;
            continue;
        }

        //generate a QImage of the rendered page
        QImage image = pdfPage->renderToImage(mDpi, mDpi);
        if (image.isNull())
        {
            delete pdfPage;
            errorsWereOccured = true;
            continue;
        }

        //
        QString outputFileName = QString::number(indexFromList) + "." + IMAGE_EXT;
        QString absoluteOutputFileName = mDir.absoluteFilePath(outputFileName);

        //
        QImageWriter imageWriter(absoluteOutputFileName, IMAGE_EXT);
        imageWriter.setCompression(1);
        bool isOk = imageWriter.write(image);
        if (isOk)
            mProcessedFiles.append(absoluteOutputFileName);

        //
        delete pdfPage;
    }

    //
    delete document;
    stopped = false;
}

void PDFRenderThread::stop()
{
    stopped = true;
}
