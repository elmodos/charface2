#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "../charface/cfplugin.h"
#include "leptonicabackend.h"

class CFPluginAnalyzeLeptonica : public CFPluginAnalyze
{
    Q_OBJECT
    Q_INTERFACES(CFPlugin)

public:
    CFPluginAnalyzeLeptonica();
    ~CFPluginAnalyzeLeptonica();

    QString name() const { return "Text zones analyzing plugin"; }

    QString pluginDescription() const;

    QString version() const { return "0.0.1"; }

    QString author() const { return "Serhiy Moroz"; }

    QIcon icon() const { return QIcon::fromTheme("empty"); }

    QString actionTitle() const { return "Analyze with leptonica"; }

    bool doAnalyze(QImage &img, ZoneList &result);

    //
    typedef struct {
        int a;
        bool b;
    } Settings;
    void showSettingsDialog();

protected:
    LeptonicaBackend *mBackend;
    Settings mSettings;
};

#endif // MYPLUGIN_H
