#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include <QDialog>

namespace Ui {
class PluginsWindow;
}

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
    Ui::PluginsWindow *ui;
};

#endif // PLUGINSWINDOW_H
