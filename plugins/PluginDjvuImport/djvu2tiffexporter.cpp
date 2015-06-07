#include "djvu2tiffexporter.h"

Djvu2TiffExporter::Djvu2TiffExporter()
{

    mDjvuContext = 0;
    mDjvuDocument = 0;
    mPageCount = -1;
}

bool Djvu2TiffExporter::load(QString exeName ,QString djvuFileName)
{

    /* Create context and document */
    if (! (mDjvuContext = ddjvu_context_create(exeName.toStdString().c_str())))
    {
        errorMessage = "Cannot create djvu context.";
        return false;
     }

    if (! (mDjvuDocument = ddjvu_document_create_by_filename_utf8(mDjvuContext, djvuFileName.toUtf8().data(), TRUE)))
    {
        errorMessage = "Cannot open djvu document.";
        return false;
    }

    while (! ddjvu_document_decoding_done(mDjvuDocument))
      handle(true);

    if (ddjvu_document_decoding_error(mDjvuDocument))
    {
        errorMessage = "Cannot decode document.\n" + errorMessage;
        return false;
    }

    /* Process all pages */
     mPageCount = ddjvu_document_get_pagenum(mDjvuDocument);

    return true;
}

int Djvu2TiffExporter::pageCount()
{
    return mPageCount;
}

bool Djvu2TiffExporter::handle(bool waitMessage)
{
    const ddjvu_message_t *message;

    if (!mDjvuContext)
      return false;

    if (waitMessage)
      message = ddjvu_message_wait(mDjvuContext);

    while ((message = ddjvu_message_peek(mDjvuContext)))
      {
        switch(message->m_any.tag)
          {
          case DDJVU_ERROR:
            errorMessage = message->m_error.message;
            return false;

          default:
            break;
          }
        ddjvu_message_pop(mDjvuContext);
      }

    return true;
}


bool Djvu2TiffExporter::renderPage2TiffFile(ddjvu_page_t *page, int outDpi, const char* outputFileName)
{
    errorMessage = "";
    ddjvu_rect_t pageRect;

    ddjvu_format_style_t djvu_format_style;
    ddjvu_render_mode_t djvu_render_mode;
    ddjvu_format_t *djvu_fomat;

    int pageWidth = ddjvu_page_get_width(page);
    int pageHeight = ddjvu_page_get_height(page);
    int pageDpi = ddjvu_page_get_resolution(page);

    ddjvu_page_type_t type = ddjvu_page_get_type(page);

    char *imageBuffer = 0;
    int rowSize;

    /* Process size specification */
    pageRect.x = 0;
    pageRect.y = 0;

    if ((outDpi != 0)&&(pageDpi != outDpi))
    {
        double dpiScale = (double)outDpi / pageDpi;

        pageRect.w = (unsigned int) (pageWidth * dpiScale);
        pageRect.h = (unsigned int) (pageHeight * dpiScale);
    }
    else
    {
        pageRect.w = pageWidth;
        pageRect.h = pageHeight;
    }

    /* Process mode specification */
    djvu_render_mode = DDJVU_RENDER_COLOR;

    /* Determine output pixel format */
    djvu_format_style = DDJVU_FORMAT_RGB24;
    if (djvu_render_mode==DDJVU_RENDER_BLACK ||
      djvu_render_mode==DDJVU_RENDER_MASKONLY ||
      (djvu_render_mode==DDJVU_RENDER_COLOR && type==DDJVU_PAGETYPE_BITONAL))
    {
      djvu_format_style = DDJVU_FORMAT_GREY8;
      if ((int)pageRect.w == pageWidth && (int)pageRect.h == pageHeight)
        djvu_format_style = DDJVU_FORMAT_MSBTOLSB;
    }

    if (! (djvu_fomat = ddjvu_format_create(djvu_format_style, 0, 0)))
    {
        errorMessage = "Cannot determine pixel style for page ";
        return false;
    }

    ddjvu_format_set_row_order(djvu_fomat, 1);

    /* Allocate buffer */
    if (djvu_format_style == DDJVU_FORMAT_MSBTOLSB)
    {
        rowSize = (pageRect.w + 7) / 8;
    }
    else if (djvu_format_style == DDJVU_FORMAT_GREY8)
            rowSize = pageRect.w;
        else
            rowSize = pageRect.w * 3;

    if (! (imageBuffer = (char*)malloc(rowSize * pageRect.h)))
    {
        errorMessage = "Cannot allocate image buffer for page.";
        ddjvu_format_release(djvu_fomat);
        return false;
    }


    /* Render */
    if (! ddjvu_page_render(page, djvu_render_mode, &pageRect, &pageRect, djvu_fomat, rowSize, imageBuffer))
    {
        errorMessage = "Error render page";
        ddjvu_format_release(djvu_fomat);
        return false;
    }
    char* tiffImageBuffer = imageBuffer;
    /* Output */
    TIFF *tiffStruct = TIFFOpen(outputFileName, "w");
    if (! tiffStruct)
    {
        errorMessage = "Error create Tiff file";
        return false;
    }

    int tiffCompression;
//    if (TIFFFindCODEC(COMPRESSION_LZMA))
//        compression = COMPRESSION_LZMA;
    if (TIFFFindCODEC(COMPRESSION_DEFLATE))
        tiffCompression = COMPRESSION_DEFLATE;
    else
        tiffCompression = COMPRESSION_NONE;


    TIFFSetField(tiffStruct, TIFFTAG_IMAGEWIDTH, (uint32)pageRect.w);
    TIFFSetField(tiffStruct, TIFFTAG_IMAGELENGTH, (uint32)pageRect.h);
    TIFFSetField(tiffStruct, TIFFTAG_XRESOLUTION,(float)((pageDpi*pageRect.w+pageWidth/2)/pageWidth));
    TIFFSetField(tiffStruct, TIFFTAG_YRESOLUTION,(float)((pageDpi*pageRect.h+pageHeight/2)/pageHeight));
    TIFFSetField(tiffStruct, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tiffStruct, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

    TIFFSetField(tiffStruct, TIFFTAG_ROWSPERSTRIP, (uint32)64);
    if (djvu_format_style == DDJVU_FORMAT_MSBTOLSB)
    {
        TIFFSetField(tiffStruct, TIFFTAG_BITSPERSAMPLE, (uint16)1);
        TIFFSetField(tiffStruct, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
        TIFFSetField(tiffStruct, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
        TIFFSetField(tiffStruct, TIFFTAG_COMPRESSION, tiffCompression);
        TIFFSetField(tiffStruct, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
    }
    else
    {
        TIFFSetField(tiffStruct, TIFFTAG_BITSPERSAMPLE, (uint16)8);
        if (djvu_format_style == DDJVU_FORMAT_GREY8)
        {
            TIFFSetField(tiffStruct, TIFFTAG_SAMPLESPERPIXEL, (uint16)1);
            TIFFSetField(tiffStruct, TIFFTAG_COMPRESSION, tiffCompression);
            TIFFSetField(tiffStruct, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        }
        else
        {
            TIFFSetField(tiffStruct, TIFFTAG_SAMPLESPERPIXEL, (uint16)3);
            TIFFSetField(tiffStruct, TIFFTAG_COMPRESSION, tiffCompression);
            TIFFSetField(tiffStruct, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        }
    }

//    TIFFSetField(tiffStruct, TIFFTAG_LZMAPRESET, 9);

    if (rowSize != TIFFScanlineSize(tiffStruct))
    {
        errorMessage = "internal error rowSize!=TiffScanLineSize";
        ddjvu_format_release(djvu_fomat);
        free(imageBuffer);
        return false;
    }

    for (int i=0; i<(int)pageRect.h; i++, tiffImageBuffer+=rowSize)
        TIFFWriteScanline(tiffStruct, tiffImageBuffer, i, 0);

    if (! TIFFFlush(tiffStruct))
    {
      errorMessage = "Error while flushing tiff file.";
      free(imageBuffer);
      return false;
    }

    TIFFClose(tiffStruct);
    free(imageBuffer);
    return true;
}

bool Djvu2TiffExporter::exportPage(int pageIndex, QString outputFileName, int Dpi)
{
    errorMessage = "";
    ddjvu_page_t *djvuPage = 0;

    /* Decode page */
    if (! (djvuPage = ddjvu_page_create_by_pageno(mDjvuDocument, pageIndex)))
    {
        errorMessage = "Cannot access page.";
        return false;
    }

    while (! ddjvu_page_decoding_done(djvuPage))
        handle(true);

    if (ddjvu_page_decoding_error(djvuPage))
    {
        errorMessage = "Cannot decode page.";
        ddjvu_page_release(djvuPage);
        return false;
    }

    /* render and save page */
    if (! renderPage2TiffFile(djvuPage, Dpi, outputFileName.toStdString().c_str()))
    {
        errorMessage = "Error when render Page"+errorMessage;
        return false;
    }

    ddjvu_page_release(djvuPage);
    return true;
}


