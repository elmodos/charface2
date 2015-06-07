#include <QStringList>
#include <QDebug>

#include "numbersegment.h"

IntList NumberSegment::getIndexesList(const QString &str, bool &error)
{
    //
    QString astr = str;
    astr.remove(" ");
    QStringList segments = astr.split(",");

    //
    IntList indexes;

    //
    error = false;

    //
    foreach (QString segment, segments)
    {
        bool res =  addIndexes(segment, indexes);

        if (!res)
        {
            error = true;
            return indexes;
        }
    }
    //
    return indexes;
}

bool NumberSegment::addIndexes(const QString &segment, IntList &list)
{
    QStringList indexes = segment.split("-");


    //only one number
    if(indexes.size() == 1)
    {
        bool ok = true;
        int val = indexes.at(0).toInt(&ok);
        if (ok) list.append(val);
        return ok;
    }

    //two numbers
    if (indexes.size() == 2)
    {
        bool ok = true;
        int val1 = indexes.at(0).toInt(&ok);
        if (!ok) return false;

        int val2 = indexes.at(1).toInt(&ok);
        if (!ok) return false;

        if (val1 >= val2) return false;

        for (int i = val1; i <= val2; i++)

            list.append(i);

        return true;
    }

    return false;
}
