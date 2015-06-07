#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "../charface/cfplugin.h"

class CFPluginColorEdit : public CFPluginImageEdit
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginColorEdit();
    ~CFPluginColorEdit();

    QString name() const { return "Color manipulation plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher"; }

    QIcon icon() const { return QIcon::fromTheme("gtk-find-and-replace"); }

    QString actionTitle() const { return "Recognize with Cuneiform"; }

    QWidget *createWidgetInteraction();
    QWidget *createWidgetTool();
    QStringList doEdit();
};

#endif // MYPLUGIN_H
