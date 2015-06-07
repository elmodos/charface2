#include <QDebug>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>

#include "myplugin.h"
#include "plugindialog.h"

Q_EXPORT_PLUGIN2(CFPluginPDFImport, CFPluginPDFImport);

CFPluginPDFImport::CFPluginPDFImport()
{
//    qDebug("CFSimpleImportPlugin Instance created");
}

CFPluginPDFImport::~CFPluginPDFImport()
{
//    qDebug("CFSimpleImportPlugin destructor");
}

QString CFPluginPDFImport::pluginDescription() const
{
    return "PDF import plugin for Charface2<br>"
            "Based on <A href=\"http://doc.qt.nokia.com/qq/qq27-poppler.html\">Poppler Qt4</A><br><br>"
            "<A href=\"https://sourceforge.net/p/charface2\">https://sourceforge.net/p/charface2</A>";
}

QStringList CFPluginPDFImport::doImportFiles(const QString &tempDir)
{
    PluginDialog pluginDialog(tempDir);
    pluginDialog.exec();
    return pluginDialog.fileNames();
}

