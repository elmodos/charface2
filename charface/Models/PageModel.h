#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QList>
#include <QIcon>
#include <QJsonValue>

#include "PageZoneModel.h"
#include "cfplugininterface.h"

typedef QList<PageZoneModel*> RectList;

class QXmlStreamWriter;
class QDomElement;
class PageModel : public QObject
{
    Q_OBJECT
public:
    //
    explicit PageModel(const QString &fileName = QString(), const QString &thumbName = QString());
    ~PageModel();

    //
    QJsonValue toJson() const;
    void writeToXML(QXmlStreamWriter &writer) const;
    bool readFromDomElement(const QDomElement &element);

    //
    QString fileName() { return mFileName; }
    QString fileNameAbsolute();
    QString thumbName() const { return mThumbName; }
    bool recognized() { return mRecognized; }
    void setRecognized(const bool val) { mRecognized = val; }

    //
    const QPixmap thumb();

    //
    RectList *zones() const { return mZones; }
    void mergeZones(const ZoneList &zones);

private:
    QString mFileName;
    QString mThumbName;
    RectList *mZones;
    bool mRecognized;
};

#endif // PAGE_H
