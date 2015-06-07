#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
\
#include "plugindialog.h"
#include "myplugin.h"

Q_EXPORT_PLUGIN2(CFPluginTIFFImport, CFPluginTIFFImport);

CFPluginTIFFImport::CFPluginTIFFImport()
{
//    qDebug("CFSimpleImportPlugin Instance created");
}

CFPluginTIFFImport::~CFPluginTIFFImport()
{
//    qDebug("CFSimpleImportPlugin destructor");
}

QString CFPluginTIFFImport::pluginDescription() const
{
    return "TIFF import plugin for Charface2<br>"
            "Based on <A href=\"http://cimg.sourceforge.net/\">Cimg</A>  library<br><br>"
            "<A href=\"https://sourceforge.net/p/charface2\">https://sourceforge.net/p/charface2</A>";
}

QStringList CFPluginTIFFImport::doImportFiles(const QString &tempDir)
{
    PluginDialog pluginDialog(tempDir);
    pluginDialog.exec();
    return pluginDialog.fileNames();
}

