/*
    This class code was based on ddjvu code
    http://djvu.sourceforge.net/doc/man/ddjvu.html
 */

#ifndef DJVU2TIFFEXPORTER_H
#define DJVU2TIFFEXPORTER_H

#include <libdjvu/ddjvuapi.h>
#include <tiff.h>
#include <tiffio.h>
#include <tiffconf.h>

#include <QString>

class Djvu2TiffExporter
{
public:
    Djvu2TiffExporter();
    bool load(QString exeName, QString djvuFileName);
    int pageCount();
    bool exportPage(int pageIndex, QString outputFileName, int Dpi = 0);

    QString errorMessage;

private:
    bool handle(bool waitMessage=true);
    bool renderPage2TiffFile(ddjvu_page_t *page, int outDpi, const char* outputFileName);

    ddjvu_context_t *mDjvuContext;
    ddjvu_document_t *mDjvuDocument;
    int mPageCount;
};

#endif // DJVU2TIFFEXPORTER_H
