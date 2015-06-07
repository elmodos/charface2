#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(CFPluginAnalyzeLeptonica::Settings &settings) :
    QDialog(0),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    mSettings = settings;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
