#include "myplugin.h"
#include <QDebug>
#include <QtWidgets/QPushButton>
#include "../charface/zone.h"
#include "settingsdialog.h"

Q_EXPORT_PLUGIN2(CFPluginAnalyzeLeptonica, CFPluginAnalyzeLeptonica);

CFPluginAnalyzeLeptonica::CFPluginAnalyzeLeptonica()
{
    qDebug("CFPluginAnalyzeLeptonica Instance created");

    mBackend = NULL;

    mSettings.a = 1;
    mSettings.b = false;
}

CFPluginAnalyzeLeptonica::~CFPluginAnalyzeLeptonica()
{
    qDebug("CFPluginAnalyzeLeptonica destructor");

    if (mBackend)
        delete mBackend;
}

QString CFPluginAnalyzeLeptonica::pluginDescription() const
{
    return "Stock CFPluginAnalyzeLeptonica plugin. A lot of text goes here.";
}

bool CFPluginAnalyzeLeptonica::doAnalyze(QImage &img, ZoneList &result)
{
    qDebug() << "bool CFPluginAnalyzeLeptonica::doAnalyze(QImage &img, CFPluginAnalyze::ZoneList &result)";

    if (!mBackend)
        mBackend = new LeptonicaBackend();

    //analyze with backend
    bool isOk = mBackend->doAnalyze(img, result);

    //some debug info
    if (mBackend->errorStrings().count())
    {
        qDebug() << "Erorr strings:";
        qDebug() << mBackend->errorStrings();
    }
    return isOk;
}

void CFPluginAnalyzeLeptonica::showSettingsDialog()
{
    qDebug() << "void CFPluginAnalyzeLeptonica::showSettingsDialog()";

    SettingsDialog *dialog = new SettingsDialog(mSettings);
    dialog->exec();
    mSettings = dialog->settings();
    delete dialog;
}
