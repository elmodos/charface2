#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDebug>

#include "pagemodel.h"

#define maxBatchVersionSupported (1.0f)
#define batchFileName ("batch.xml")
#define batchFileVersion (QString::number(maxBatchVersionSupported))

typedef QList<PageModel *> PagesList;

class DocumentModel : public QObject
{
    Q_OBJECT

public:
    //
    explicit DocumentModel(const QString path = QString());
    ~DocumentModel();

    //
    PagesList *pages() { return mPages; }
    const QString path() const { return mPath; }
    const QString tempPath(bool createPath);
    bool isSaved();
    void loadFromDir(const QString &path);
    void saveToDir(const QString &path);
    void save();

    bool addFile(const QString &path, bool move);
    bool deletePage(int index);

    //
    QString errorMessage() { return mErrorMessage; }

    //need to call directly from app manager when pageView rectangle changes
    bool saveXML();

    bool removeAllFilesInDir(const QString &path);

private:
    //
    bool loadXML();

    //
    bool copyBatchFilesTo(const QString &newPath);
    bool copyFile(const QString &fromF, const QString &toF);

    PagesList *mPages;
    QString mPath;

    QString mErrorMessage;
};

#endif // DOCUMENTMODEL_H
