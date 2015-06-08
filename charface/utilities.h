#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QString>
#include <QIcon>

#define LOAD_ICON(x) (Utilities::loadIcon(x))

class Utilities : public QObject
{
public:
    static QString getCurrentIconsThemeName();
    static QIcon loadIcon(const QString &name);
};

#endif // UTILITIES_H
