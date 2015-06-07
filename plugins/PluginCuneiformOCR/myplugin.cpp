#include "myplugin.h"
#include <QDebug>
#include <QtWidgets/QPushButton>

Q_EXPORT_PLUGIN2(CFPluginOCRCuneiform, CFPluginOCRCuneiform);

CFPluginOCRCuneiform::CFPluginOCRCuneiform()
{
    qDebug("CFCuneiformOCRPlugin Instance created");
    mCuneiform = NULL;
}

CFPluginOCRCuneiform::~CFPluginOCRCuneiform()
{
    qDebug("CFCuneiformOCRPlugin destructor");
    if (mCuneiform)
        delete mCuneiform;
}

QString CFPluginOCRCuneiform::pluginDescription() const
{
    return "Stock CFCuneiformOCRPlugin plugin. A lot of text goes here.";
}

bool CFPluginOCRCuneiform::doOCR(QImage &img, QString &result)
{
    qDebug() << "bool CFPluginOCRCuneiform::doOCR(const QImage &img, Qstring &result)";

    //
    if (!mCuneiform)
        mCuneiform = new CuneiformBackend();

    //
    mCuneiform->doOCR(img, result);

    if (mCuneiform->errorStrings().count())
    {
        qDebug() << "Erorr strings:";
        qDebug() << mCuneiform->errorStrings();
    }

    return false;
}

void CFPluginOCRCuneiform::showSettingsDialog()
{
    qDebug() << "void CFPluginOCRCuneiform::showSettingsDialog()";
}
