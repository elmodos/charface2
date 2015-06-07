#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets/QDialog>
#include <myplugin.h>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    SettingsDialog(CFPluginAnalyzeLeptonica::Settings &settings);
    ~SettingsDialog();
    
    //Plugin settings, handled by dialog
    CFPluginAnalyzeLeptonica::Settings settings() { return mSettings; }

private:
    Ui::SettingsDialog *ui;
    CFPluginAnalyzeLeptonica::Settings mSettings;
};

#endif // SETTINGSDIALOG_H
