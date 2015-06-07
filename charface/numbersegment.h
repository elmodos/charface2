#ifndef NUMBERSEGMENT_H
#define NUMBERSEGMENT_H

#include <QList>
#include <QString>
#include "typedefs.h"

class NumberSegment
{
public:
    static IntList getIndexesList(const QString &str, bool &error);

protected:
    static bool addIndexes(const QString &segment, IntList &list);
};

#endif // NUMBERSEGMENT_H
