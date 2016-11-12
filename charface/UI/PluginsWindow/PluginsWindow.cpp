#include <QFileDialog>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

#include "pluginmanager.h"
#include "applicationmanager.h"
#include "UI/PluginsWindow/PluginsWindow.h"
#include "ui_PluginsWindow.h"
#include "UI/PluginAboutDialog/PluginAboutDialog.h"

PluginsWindow::PluginsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginsWindow)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint );

    //
    ui->buttonClose->setIcon(QIcon::fromTheme("window-close"));
    ui->buttonAboutPlugin->setIcon(QIcon::fromTheme("gtk-about"));
    ui->buttonAddPlugin->setIcon(QIcon::fromTheme("list-add"));
    ui->buttonAddPluginDir->setIcon(QIcon::fromTheme("list-add"));

    //
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    //fill pluginType list
    ui->comboBoxPluginTypes->clear();
    for (int pt = PT_All; pt < PT_Count; pt++)
    {
        QString str = pluginTypeToString((PluginType)pt);
        ui->comboBoxPluginTypes->addItem(str);
    }
}

PluginsWindow::~PluginsWindow()
{
    delete ui;
}

void PluginsWindow::onLoadPlugin()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.dlyb");

    if (!dialog.exec()) return;
    QStringList list = dialog.selectedFiles();
    QString name = list[0];

    bool loaded = PluginManager::instance()->registerPlugin(name);
    if (loaded)
        showPluginsList();
}

void PluginsWindow::onRegisterPluginsFromDir()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);

    if (!dialog.exec()) return;

    bool loaded = PluginManager::instance()->registerPluginsFromDirectory(dialog.directory().absolutePath());
    if (loaded)
        showPluginsList();
}

void PluginsWindow::onAboutPlugin()
{
    //Get pluginType from filter
    int index = ui->comboBoxPluginTypes->currentIndex();
    PluginType filtePluginType = (PluginType)index;

    //get plugins list
    PluginsList plugins = PluginManager::instance()->plugins(filtePluginType);
    int currentIndex = ui->tableWidget->currentRow();

    //validate
    if (currentIndex < 0 || currentIndex >= plugins.size())
        return;

    CFPluginInterface *plugin = plugins[ui->tableWidget->currentRow()];

    //get inner about dialog
    QDialog *aboutDialog = plugin->aboutDialog();
    if (aboutDialog)
        aboutDialog->exec();
    else
        showPluginAboutDialog(plugin);
}

void PluginsWindow::showPluginAboutDialog(CFPluginInterface *plugin)
{
    PluginAboutDialog dialog(plugin);
    dialog.exec();
}

void PluginsWindow::showPluginsList()
{
    //Get pluginType from filter
    int index = ui->comboBoxPluginTypes->currentIndex();
    PluginType filtePluginType = (PluginType)index;

    //resize tableWidget content
    ui->tableWidget->clear();

    //col titles and alignment
    QStringList headerLabels;
    headerLabels << "Plugin name" << "Version" << "Author";
    ui->tableWidget->setColumnCount(headerLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //get plugins list
    PluginsList plugins = PluginManager::instance()->plugins(filtePluginType);
    ui->tableWidget->setRowCount(plugins.size());

    //add entries
    for (int i = 0; i < plugins.size(); i++)
    {
        CFPluginInterface *plugin = plugins[i];

        //name
        QTableWidgetItem *item = new QTableWidgetItem(plugin->icon(), plugin->name());
        ui->tableWidget->setItem(i, 0, item);

        //version
        item = new QTableWidgetItem(plugin->version());
        ui->tableWidget->setItem(i, 1, item);

        //name
        item = new QTableWidgetItem(plugin->author());
        ui->tableWidget->setItem(i, 2, item);
    }

    //resize columns
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

const QString PluginsWindow::pluginTypeToString(const PluginType pt) const
{
    switch(pt)
    {
        case PT_All: return QString("All plugins");
        case PT_Import: return QString("Import plugins");
        case PT_ImageEdit: return QString("Image manipulation plugins");
        case PT_Analyze: return QString("Image structure analyzing plugins");
        case PT_OCR: return QString("OCR plugins");
        case PT_TextPostprocessing: return QString("Text postprocessing plugins");
        case PT_Export: return QString("Export plugins");
        default: return QString("Unknown");
    }
}
