#include "myplugin.h"
#include <QDebug>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

CFPluginSimpleImport::CFPluginSimpleImport()
{
    qDebug("CFSimpleImportPlugin Instance created");
}

CFPluginSimpleImport::~CFPluginSimpleImport()
{
    qDebug("CFSimpleImportPlugin destructor");
}

QString CFPluginSimpleImport::pluginDescription() const
{
    return "Stock CFSimpleImportPlugin plugin. It goes with application as an example how to create plugins for charface.<br>"
            "<A href=\"www.google.com.ua\">google.com.ua</A>";
}

QStringList CFPluginSimpleImport::doImportFiles(const QString &tempDir)
{
    //
    Q_UNUSED(tempDir);

    //
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (!dialog.exec()) return QStringList();

    QStringList files = dialog.selectedFiles();

    return files;
}
