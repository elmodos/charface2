#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "cuneiformbackend.h"
#include "../charface/cfplugin.h"

class CFPluginOCRCuneiform : public CFPluginOCR
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginOCRCuneiform();
    ~CFPluginOCRCuneiform();

    QString name() const { return "OCR Cuneiform support plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.0"; }

    QString author() const { return "Modo Ltunzher"; }

    QIcon icon() const { return QIcon::fromTheme("gtk-find-and-replace"); }

    QString actionTitle() const { return "Cuneiform"; }

    bool doOCR(QImage &img, QString &result);

    void showSettingsDialog();

protected:
    CuneiformBackend *mCuneiform;
};

#endif // MYPLUGIN_H
