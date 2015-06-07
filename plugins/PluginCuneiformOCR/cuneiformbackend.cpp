#include "cuneiformbackend.h"
#include <QDebug>
#include <QImageWriter>
#include <QBuffer>
#include <QByteArray>
#include <QPixmap>
#include <QFile>

#define TEXT_BUFFER_SIZE ((size_t)1024 * 1024 * 10)

CuneiformBackend::CuneiformBackend()
{
    if(!PUMA_Init(0, 0))
        mErrorStrings << tr("PUMA_Init failed.");
}

CuneiformBackend::~CuneiformBackend()
{
    if( !PUMA_Done() )
        qDebug() << "PUMA_Done failed.";
}

QString CuneiformBackend::cuneiformVersion()
{
    return "version here";//CF_VERSION;
}

bool CuneiformBackend::doOCR(QImage &image, QString &outString, Puma_Language langcode, bool isFax, bool isDotMatrix, bool isOneColumn)
{
    qDebug() << "backend doOCR";

    //
    mErrorStrings.clear();

    void *dib = createDIB(image);

    //default output format for charface
    int32_t outputformat = PUMA_TOHTML; //PUMA_TOHOCR - did not work, but seems to be same as HTML

    //set options
    PUMA_SetImportData(PUMA_Word32_Language,      &langcode);
    PUMA_SetImportData(PUMA_PumaBool32_DotMatrix, &isDotMatrix);
    PUMA_SetImportData(PUMA_PumaBool32_Fax100,    &isFax);
    PUMA_SetImportData(PUMA_PumaBool32_OneColumn, &isOneColumn);

    //load buffer
    if( !PUMA_XOpen(dib + 14, NULL) )//shift from bmp 1st byte
    {
        qDebug() << "PUMA_XOpen failed";
        mErrorStrings << tr("PUMA_Xopen failed.");
        return false;
    }
    qDebug() << "PUMA_XOpen ok";

    PumaBool res = PUMA_XFinalRecognition();

    qDebug() << "Return code" << res;
    if( !res )
    {
        qDebug() << "PUMA_XFinalRecognition failed";
        mErrorStrings << tr("PUMA_XFinalrecognition failed. %1").arg(res);
        return false;
    }
    qDebug() << "PUMA_XFinalRecognition ok";

    //outputstring
    char *buffer = (char*)malloc(TEXT_BUFFER_SIZE);
    if( !PUMA_SaveToMemory(NULL, outputformat, PUMA_CODE_UTF8, buffer, TEXT_BUFFER_SIZE) )
    {
        free(buffer);
        qDebug() << "PUMA_SaveToMemory failed";
        mErrorStrings << tr("PUMA_SaveToMemory failed.");
        outString = "";
        return false;
    }
    else
    {
        qDebug() << "saving string";
        outString = QString(buffer);
    }
    free(buffer);
    qDebug() << "PUMA_SaveToMemory ok";

    if( !PUMA_XClose() )
    {
        qDebug() << "PUMA_XClose failed";
        mErrorStrings << tr("PUMA_XClose failed.");
        return false;
    }
    qDebug() << "PUMA_SaveToMemory ok";


    return true;
}

#define USE_BMP

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;
typedef uchar BYTE;

#ifdef USE_BMP

typedef struct tagBITMAPFILEHEADER
{
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
  BYTE rgbRed;
  BYTE rgbGreen;
  BYTE rgbBlue;
  BYTE rgbReserved;
} RGBQUAD;

#else // USE_BMP

// 14 bytes since each WORD is 2 bytes and DWORD is 4
typedef struct tagBITMAPFILEHEADER // bmfh
{
  WORD bfType ; // signature word "BM" or 0x4D42
  DWORD bfSize ; // entire size of file
  WORD bfReserved1 ; // must be zero
  WORD bfReserved2 ; // must be zero
  DWORD bfOffsetBits ; // offset in file of DIB pixel bits
}
BITMAPFILEHEADER, * PBITMAPFILEHEADER ;

typedef struct tagBITMAPCOREHEADER // bmch
{
  DWORD bcSize ; // size of the structure = 12
  WORD bcWidth ; // width of image in pixels
  WORD bcHeight ; // height of image in pixels
  WORD bcPlanes ; // = 1
  WORD bcBitCount ; // bits per pixel (1, 4, 8, or 24)
}
BITMAPCOREHEADER, * PBITMAPCOREHEADER ;// Also defined in WINGDI.H

typedef struct tagRGBTRIPLE // rgbt
{
  BYTE rgbtBlue ; // blue level
  BYTE rgbtGreen ; // green level
  BYTE rgbtRed ; // red level
}
RGBTRIPLE ;

#endif // USE_BMP

#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

void *CuneiformBackend::createDIB(QImage &image)
{
    image = image.convertToFormat(QImage::Format_Mono,Qt::MonoOnly|Qt::ThresholdDither);

    qDebug() << "Bitmap data" << image.byteCount();

    //
    unsigned int imageSize = image.byteCount();
    char *ret = NULL, *buffer = NULL;

#ifdef USE_BMP

    BITMAPFILEHEADER bitmapfileheader;
    bitmapfileheader.bfType = 0x4D42;
    bitmapfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD);
    bitmapfileheader.bfSize = bitmapfileheader.bfOffBits + imageSize;
    bitmapfileheader.bfReserved1 = 0;
    bitmapfileheader.bfReserved2 = 0;

    buffer = (char *)malloc(bitmapfileheader.bfSize);
    ret = buffer;

    //
    memcpy(buffer, &bitmapfileheader, sizeof(bitmapfileheader));
    buffer += sizeof(bitmapfileheader);

    // update the bitmap info header
    BITMAPINFOHEADER bih;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = image.size().width();
    bih.biHeight = -image.size().height();
    bih.biPlanes = 1;
    bih.biBitCount = 1;
    bih.biCompression = 0;
    bih.biSizeImage = imageSize;
    bih.biXPelsPerMeter = bih.biWidth;
    bih.biYPelsPerMeter = bih.biHeight;
    bih.biClrUsed = 2;
    bih.biClrImportant = 0;

    //
    memcpy(buffer, &bih, sizeof(BITMAPINFOHEADER));
    buffer += sizeof(BITMAPINFOHEADER);
    qDebug() << "sizeof(BITMAPINFOHEADER)"<< sizeof(BITMAPINFOHEADER);
    qDebug() << sizeof(WORD) <<sizeof(DWORD) <<sizeof(LONG)<<sizeof(BYTE);
;
    //setup mono palette
    {
        // add white to buffer
        RGBQUAD white;
        white.rgbRed = 255;
        white.rgbGreen = 255;
        white.rgbBlue = 255;
        white.rgbReserved = 0;

        // add black to buffer
        RGBQUAD black;
        black.rgbRed = 0;
        black.rgbGreen = 0;
        black.rgbBlue = 0;
        black.rgbReserved = 0;

        memcpy(buffer, &white, sizeof(RGBQUAD));
        buffer += sizeof(RGBQUAD);
        memcpy(buffer, &black, sizeof(RGBQUAD));
        buffer += sizeof(RGBQUAD);
    }

#else // USE_BMP

    BITMAPFILEHEADER bitmapfileheader;

    bitmapfileheader.bfType = 0x4D42;
    bitmapfileheader.bfOffsetBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER) + 2 * sizeof(RGBTRIPLE);
    bitmapfileheader.bfSize = bitmapfileheader.bfOffsetBits + imageSize;
    bitmapfileheader.bfReserved1 = 0;
    bitmapfileheader.bfReserved2 = 0;

    buffer = (char *)malloc(bitmapfileheader.bfSize);
    ret = buffer;

    //
    memcpy(buffer, &bitmapfileheader, sizeof(bitmapfileheader));
    buffer += sizeof(bitmapfileheader);

    // update the bitmap info header
    BITMAPCOREHEADER bih;
    bih.bcSize = sizeof(BITMAPCOREHEADER);
    bih.bcWidth = image.size().width();
    bih.bcHeight = -image.size().height();
    bih.bcPlanes = 1;
    bih.bcBitCount = 1;

    //
    memcpy(buffer, &bih, sizeof(BITMAPCOREHEADER));
    buffer += sizeof(BITMAPCOREHEADER);
    qDebug() << "sizeof(BITMAPCOREHEADER)"<< sizeof(BITMAPCOREHEADER);
    qDebug() << sizeof(WORD) <<sizeof(DWORD) <<sizeof(LONG)<<sizeof(BYTE);

    //setup mono palette
    {
        // add white to buffer
        RGBTRIPLE white;
        white.rgbtRed = 255;
        white.rgbtGreen = 255;
        white.rgbtBlue = 255;

        // add black to buffer
        RGBTRIPLE black;
        black.rgbtRed = 0;
        black.rgbtGreen = 0;
        black.rgbtBlue = 0;

        memcpy(buffer, &white, sizeof(RGBTRIPLE));
        buffer += sizeof(RGBTRIPLE);
        memcpy(buffer, &black, sizeof(RGBTRIPLE));
        buffer += sizeof(RGBTRIPLE);
    }


#endif // USE_BMP

    // write the bitmap data
    memcpy(buffer, image.bits(), image.byteCount());

    /*QFile file("/home/elmodos/Desktop/100.bmp");
    file.open(QIODevice::WriteOnly);
    file.write((char*)ret,bitmapfileheader.bfSize);*/

    return ret;
}
