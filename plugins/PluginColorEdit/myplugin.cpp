#include "myplugin.h"
#include <QDebug>
#include <QtWidgets/QPushButton>
#include "interactivewidget.h"

Q_EXPORT_PLUGIN2(CFPluginColorEdit, CFPluginColorEdit);

CFPluginColorEdit::CFPluginColorEdit()
{
    qDebug("CFCuneiformOCRPlugin Instance created");
}

CFPluginColorEdit::~CFPluginColorEdit()
{
    qDebug("CFCuneiformOCRPlugin destructor");
}

QString CFPluginColorEdit::pluginDescription() const
{
    return "Stock CFCuneiformOCRPlugin plugin. A lot of text goes here.";
}

QWidget *CFPluginColorEdit::createWidgetInteraction()
{
    return new InteractiveWidget();
}

QWidget *CFPluginColorEdit::createWidgetTool()
{
    return new QPushButton("I am plugin settings widget");
}

QStringList CFPluginColorEdit::doEdit()
{
    return QStringList() << mImageFile;
}
