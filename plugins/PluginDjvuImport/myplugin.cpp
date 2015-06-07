#include "myplugin.h"
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include "plugindialog.h"

Q_EXPORT_PLUGIN2(CFPluginDjvuImport, CFPluginDjvuImport);

CFPluginDjvuImport::CFPluginDjvuImport()
{
//    qDebug("CFSimpleImportPlugin Instance created");
}

CFPluginDjvuImport::~CFPluginDjvuImport()
{
//    qDebug("CFSimpleImportPlugin destructor");
}

QString CFPluginDjvuImport::pluginDescription() const
{
    return "Djvu import plugin for Charface2<br>"
            "Based on <A href=\"http://djvu.sourceforge.net/\">DjvuLibre</A><br><br>"
            "<A href=\"https://sourceforge.net/p/charface2\">https://sourceforge.net/p/charface2</A>";
}

QIcon CFPluginDjvuImport::icon() const
{
    if (QIcon::hasThemeIcon("application-djvu"))
        return QIcon::fromTheme("application-djvu");
    else
        return QIcon(":/icons/djvu.svg");
}

QStringList CFPluginDjvuImport::doImportFiles(const QString &tempDir)
{
    PluginDialog pluginDialog(tempDir, this);
    pluginDialog.exec();
    return pluginDialog.fileNames();
}

