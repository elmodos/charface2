#include "leptonicabackend.h"
#include "../charface/zone.h"

LeptonicaBackend::LeptonicaBackend()
{
}

bool LeptonicaBackend::doAnalyze(QImage &img, ZoneList &result)
{
    mErrorStrings.clear();

    Zone zone;
    zone.setX(50);
    zone.setY(50);
    zone.setWidth(img.width() - 100);
    zone.setHeight(img.height() - 100);
    zone.setZoneType( ZT_Text );
    result.append(zone);

    return true;
}
