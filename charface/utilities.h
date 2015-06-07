#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QString>
#include <QIcon>

class Utilities : public QObject
{
public:
    static QString getCurrentIconsThemeName();
    static QIcon loadIcon(const QString &name);
};

#define LOAD_ICON(x) (Utilities::loadIcon(x))

#endif // UTILITIES_H
