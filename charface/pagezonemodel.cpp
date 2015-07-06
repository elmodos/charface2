#include <QPalette>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include "pagezonemodel.h"

PageZoneModel::PageZoneModel() :
    mPolygon()
{
    //
    mZoneType = ZT_Unknown;
}

PageZoneModel::PageZoneModel(const QRect &rectangle) :
    mPolygon()
{
    //
    mZoneType = ZT_Unknown;

    //
    mPolygon.append(rectangle.topLeft());
    mPolygon.append(rectangle.topRight());
    mPolygon.append(rectangle.bottomRight());
    mPolygon.append(rectangle.bottomLeft());
}

PageZoneModel::PageZoneModel(const PageZoneModel &other)
{
    mZoneType = other.mZoneType;
    mPolygon = other.mPolygon;
}

QColor PageZoneModel::zoneColor(const ZoneType zoneType)
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

QString PageZoneModel::zoneToString(ZoneType zoneType)
{
    QStringList list;
    list
        << "Unknown"
        << "Text"
        << "Table"
        << "Image";

    return list.at(zoneType);
}

QJsonValue PageZoneModel::toJson() const
{
    QJsonObject object;

    //
    object["type"] = QJsonValue(mZoneType);

    // Points
    QJsonArray pointsArray;
    foreach(QPoint point, mPolygon)
    {
        QJsonObject pointObject;
        pointObject["x"] = QJsonValue(point.x());
        pointObject["y"] = QJsonValue(point.y());

        pointsArray.append(QJsonValue(pointObject));
    }
    object["polygon"] = QJsonValue(pointsArray);

    //
    return QJsonValue(object);
}


void PageZoneModel::writeToXML(QXmlStreamWriter &writer) const
{
    //
    writer.writeAttribute("type", QString::number(mZoneType));

    // TODO
    /*
    //coordinates
    writer.writeAttribute("left", QString::number(left()));
    writer.writeAttribute("top", QString::number(top()));
    writer.writeAttribute("height", QString::number(height()));
    writer.writeAttribute("width", QString::number(width()));
    */
}

bool PageZoneModel::readFromDomElement(const QDomElement &element)
{
    //
    mZoneType = element.attribute("type", QString::number(ZT_Unknown)).toInt();

    // TODO
    /*
    //coordinates
    setLeft(element.attribute("left", QString("0")).toInt());
    setTop(element.attribute("top", QString("0")).toInt());
    setHeight(element.attribute("height", QString("0")).toInt());
    setWidth(element.attribute("width", QString("0")).toInt());
     */

    //
    return true;
}
