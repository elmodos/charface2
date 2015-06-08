#ifndef PLUGINABOUTDIALOG_H
#define PLUGINABOUTDIALOG_H

#include <QDialog>

class CFPlugin;

namespace Ui {
class PluginAboutDialog;
}

class PluginAboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginAboutDialog(CFPlugin *plugin);
    ~PluginAboutDialog();
    int exec();
    void show();

private:
    Ui::PluginAboutDialog *ui;
};

#endif // PLUGINABOUTDIALOG_H
