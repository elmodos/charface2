#include <QPalette>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include "zone.h"

Zone::Zone()
{
    //
    mZoneType = ZT_Unknown;
}

Zone::Zone(const Zone &other) :
    QRect(other)
{
    mZoneType = other.mZoneType;
}

QColor Zone::zoneColor(const ZoneType zoneType)
{
    switch (zoneType)
    {
        case ZT_Text:
            return QColor(141,186,63);//QPalette().highlight().color();

        case ZT_Sheet:
            return QColor(63,141,186);

        case ZT_Image:
            return QColor(186,63,63);

        case ZT_Unknown:
            return QColor(128,128,128);

        default:
            return QColor(0,0,0);
    }

    return QColor(0,0,0);
}

QString Zone::zoneToString(ZoneType zoneType)
{
    QStringList list;
    list
        << "Unknown"
        << "Text"
        << "Table"
        << "Image";

    return list.at(zoneType);
}

void Zone::writeToXML(QXmlStreamWriter &writer) const
{
    //
    writer.writeAttribute("type", QString::number(mZoneType));

    //coordinates
    writer.writeAttribute("left", QString::number(left()));
    writer.writeAttribute("top", QString::number(top()));
    writer.writeAttribute("height", QString::number(height()));
    writer.writeAttribute("width", QString::number(width()));
}

bool Zone::readFromDomElement(const QDomElement &element)
{
    //
    mZoneType = element.attribute("type", QString::number(ZT_Unknown)).toInt();

    //coordinates
    setLeft(element.attribute("left", QString("0")).toInt());
    setTop(element.attribute("top", QString("0")).toInt());
    setHeight(element.attribute("height", QString("0")).toInt());
    setWidth(element.attribute("width", QString("0")).toInt());

    //
    return true;
}
