#include "pluginaboutdialog.h"
#include "cfplugininterface.h"
#include "ui_pluginaboutdialog.h"

PluginAboutDialog::PluginAboutDialog(CFPluginInterface *plugin) :
    QDialog(0),
    ui(new Ui::PluginAboutDialog)
{
    ui->setupUi(this);

    //
    this->setWindowTitle(plugin->name());
    this->setWindowIcon(plugin->icon());

    //
    ui->closeButton->setIcon(QIcon::fromTheme("window-close"));

    //icon
    ui->labelIcon->setPixmap(plugin->icon().pixmap(48));

    //bold name
    ui->labelNameBig->setText("<H2><b>"+plugin->name()+ " " + plugin->version() + "</b></H2>");

    //description
    ui->labelDecription->setText("<br>" + plugin->pluginDescription() + "<br>");

    //author
    ui->labelAuthor->setText("<H4>" + plugin->author() + "</H4>");
}

PluginAboutDialog::~PluginAboutDialog()
{
    delete ui;
}

int PluginAboutDialog::exec()
{
    resize(sizeHint());
    return QDialog::exec();
}

