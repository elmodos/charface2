#ifndef ZONE_H
#define ZONE_H

#include <QRect>
#include <QColor>

enum {
    ZT_Unknown = 0,
    ZT_Text,
    ZT_Sheet,
    ZT_Image,

    //
    ZT_Count
};
typedef int ZoneType;

class QXmlStreamWriter;
class QDomElement;

class Zone : public QRect
{

public:
    Zone();
    Zone(const Zone &other);

    static QColor zoneColor(const ZoneType zoneType);
    static QString zoneToString(ZoneType zoneType);

    //
    ZoneType zoneType() { return mZoneType; }
    void setZoneType(const ZoneType zoneType) { mZoneType = zoneType; }

    //
    void writeToXML(QXmlStreamWriter &writer) const;
    bool readFromDomElement(const QDomElement &element);

signals:
    void zoneTypeChanged();

private:
    ZoneType mZoneType;
};

#endif // ZONE_H
