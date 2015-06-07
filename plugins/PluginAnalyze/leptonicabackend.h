#ifndef LEPTONICABACKEND_H
#define LEPTONICABACKEND_H

#include <QObject>
#include <QStringList>
#include <QImage>
#include "../charface/cfplugin.h"

class LeptonicaBackend : public QObject
{
public:
    LeptonicaBackend();

    bool doAnalyze(QImage &img, ZoneList &result);
    QStringList errorStrings() { return mErrorStrings; }

protected:
    QStringList mErrorStrings;
};

#endif // LEPTONICABACKEND_H
