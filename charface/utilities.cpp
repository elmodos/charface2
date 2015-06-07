#include "utilities.h"


QString Utilities::getCurrentIconsThemeName()
{
    return QIcon::themeName();//"elementary";
}

QIcon Utilities::loadIcon(const QString &name)
{
    QIcon icon = QIcon::fromTheme(name);

    if (icon.isNull())
        icon = QIcon(QString(":/icons/%1").arg(name));

    return icon;
}
