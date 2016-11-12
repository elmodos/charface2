#include <QDir>
#include <QList>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QJsonArray>

#include "PageModel.h"
#include "applicationmanager.h"
#include "settingsmanager.h"
#include "DocumentModel.h" // TODO remove cross references

PageModel::PageModel(const QString &fileName, const QString &thumbName) :
    QObject(0)
{
    //
    mRecognized = false;

    //
    mFileName = fileName;
    mThumbName = thumbName;

    //
    mZones = new RectList();
}

PageModel::~PageModel()
{
    //
    qDeleteAll(*mZones);
    delete mZones;
}

QJsonValue PageModel::toJson() const
{
    QJsonObject object;

    object["file"] = QJsonValue(mFileName);
    object["thumbnail"] = QJsonValue(mThumbName);
    object["recognized"] = QJsonValue(mRecognized);

    // zones
    QJsonArray zonesArray;
    foreach(PageZoneModel *zone, *mZones)
    {
        QJsonValue zoneObject(zone->toJson());
        zonesArray.append(zoneObject);
    }
    object["zones"] = QJsonValue(zonesArray);

    //
    return QJsonValue(object);
}


void PageModel::writeToXML(QXmlStreamWriter &writer) const
{
    //attributes
    writer.writeAttribute("file", mFileName);
    writer.writeAttribute("thumb", mThumbName);
    writer.writeAttribute("recognized", mRecognized ? "1" : "0");

    //zones
    size_t count = mZones->size();
    for (size_t i = 0; i < count; i++)
    {
        writer.writeStartElement("zone");
        mZones->at(i)->writeToXML(writer);
        writer.writeEndElement();
    }
}

bool PageModel::readFromDomElement(const QDomElement &element)
{
    //attributes
    mFileName = element.attribute("file",QString());
    mThumbName = element.attribute("thumb",QString());
    mRecognized = element.attribute("recognozed","0").toInt();

    //zones
    qDeleteAll(*mZones);
    mZones->clear();

    //
    QDomElement zoneElement = element.firstChildElement("zone");
    while ( !zoneElement.isNull())
    {
        PageZoneModel *zone = new PageZoneModel();
        zone->readFromDomElement(zoneElement);
        mZones->push_back(zone);
        zoneElement = zoneElement.nextSiblingElement("zone");
    }

    return true;
}

QString PageModel::fileNameAbsolute()
{
    // TODO
    //QDir dir(ApplicationManager::instance()->document()->path());
    return QString();//dir.absoluteFilePath(mFileName);
}

const QPixmap PageModel::thumb()
{
    // TODO
//    QDir batchDir(ApplicationManager::instance()->document()->path());
//    QPixmap pixmap(batchDir.absoluteFilePath(mThumbName));
    return QPixmap();//pixmap;
}

void PageModel::mergeZones(const ZoneList &zones)
{
    //remove old zones if needed
    if (settingsManager->replaceZonesOnAnalyze())
    {
        qDeleteAll(*mZones);
        mZones->clear();
    }

    foreach( PageZoneModel zone, zones )
    {
        PageZoneModel *temp = new PageZoneModel(zone);
        mZones->append(temp);
    }
}
