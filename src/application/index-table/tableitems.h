#ifndef TABLEITEMS_H
#define TABLEITEMS_H

#include <QString>
#include <QVector>

struct EventItem
{
    int id;
    QString description;
};

struct DimensionItem
{
    int id;
    QString name;
    QVector<EventItem> events;
};

#endif // TABLEITEMS_H
