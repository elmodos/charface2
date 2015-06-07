#ifndef CUNEIFORMBACKEND_H
#define CUNEIFORMBACKEND_H

#include <cuneiform.h>

#include<iostream>
#include<sstream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>

#include <QImage>
#include <QStringList>

using namespace std;

typedef int Puma_Language;

class CuneiformBackend : QObject
{
public:
    CuneiformBackend();
    ~CuneiformBackend();

    static QString cuneiformVersion();

    bool doOCR(QImage &image, QString &outString, Puma_Language langcode = PUMA_LANG_ENGLISH, bool isFax = false, bool isDotMatrix = false, bool isOneColumn = false);

    QStringList errorStrings() { return mErrorStrings; }

protected:
    QStringList mErrorStrings;

    void *createDIB(QImage &image);
};

#endif // CUNEIFORMBACKEND_H
