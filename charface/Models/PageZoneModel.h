#ifndef ZONE_H
#define ZONE_H

#include <QObject>
#include <QRect>
#include <QColor>
#include <QPolygon>

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

class PageZoneModel : public QObject
{
    Q_OBJECT

public:

    //
    PageZoneModel();
    PageZoneModel(const QRect &rectangle);
    PageZoneModel(const PageZoneModel &other);

    // move from model to view
    static QColor zoneColor(const ZoneType zoneType);

    //
    static QString zoneToString(ZoneType zoneType);

    //
    ZoneType zoneType() { return mZoneType; }
    void setZoneType(const ZoneType zoneType) { mZoneType = zoneType; }

    //
    QJsonValue toJson() const;
    void writeToXML(QXmlStreamWriter &writer) const;
    bool readFromDomElement(const QDomElement &element);

    //
    QPolygon &getPolygon() { return mPolygon; }

signals:
    void zoneTypeChanged();

private:
    ZoneType mZoneType;
    QPolygon mPolygon;
};

#endif // ZONE_H
