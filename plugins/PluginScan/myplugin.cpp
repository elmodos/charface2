#include "myplugin.h"
#include "scannerdialog.h"

Q_EXPORT_PLUGIN2(CFPluginSimpleImport, CFPluginSimpleImport);

CFPluginSimpleImport::CFPluginSimpleImport()
{
}

QString CFPluginSimpleImport::pluginDescription() const
{
    return "Stock Scanner interface plugin. It uses SANE for input.<br>"
            "<A href=\"www.google.com.ua\">google.com.ua</A>";
}

QStringList CFPluginSimpleImport::doImportFiles(const QString &tempDir)
{
    ScannerDialog dialog;
    dialog.setTempDirectory(tempDir);
    dialog.exec();

    return dialog.files();
}
