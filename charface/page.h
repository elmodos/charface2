#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QList>
#include <QIcon>

#include "cfplugin.h"
#include "zone.h"

class Batch;
typedef QList<Zone*> RectList;

class QXmlStreamWriter;
class QDomElement;
class Page : public QObject
{
    Q_OBJECT
public:
    explicit Page(const QString &fileName = QString(), const QString &thumbName = QString());
    ~Page();
    //
    void writeToXML(QXmlStreamWriter &writer) const;
    bool readFromDomElement(const QDomElement &element);

    //
    QString fileName() { return mFileName; }
    QString fileNameAbsolute();
    QString thumbName() { return mThumbName; }
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
