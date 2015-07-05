#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include <QDialog>
#include "cfplugininterface.h"

namespace Ui {
class PluginsWindow;
}

class CFPluginInterface;

class PluginsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginsWindow(QWidget *parent = 0);
    ~PluginsWindow();
    
private slots:
    void onLoadPlugin();
    void onRegisterPluginsFromDir();
    void onAboutPlugin();
    void showPluginsList();

private:
    void showPluginAboutDialog(CFPluginInterface *plugin);
    const QString pluginTypeToString(const PluginType pt) const;

    Ui::PluginsWindow *ui;
};

#endif // PLUGINSWINDOW_H
