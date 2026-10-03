#ifndef INDEXPARAMETERSVIEWMODELS_H
#define INDEXPARAMETERSVIEWMODELS_H

#include <qqml.h>
#include <QGuiApplication>
#include <QList>
#include <QVector>

#include "src/application/index-table/tableitems.h"

struct journeyEventViewModel
{
    Q_GADGET
    QML_ELEMENT
    Q_PROPERTY(int id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)

public:
    int id;
    QString name;
};

struct journeyDimensionViewModel
{
    Q_GADGET
    QML_ELEMENT
    Q_PROPERTY(int id MEMBER id CONSTANT)
    Q_PROPERTY(QString icon MEMBER icon CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QList<journeyEventViewModel> events MEMBER events CONSTANT)

public:
    int id;
    QString icon;
    QString name;
    QList<journeyEventViewModel> events;
};

inline QString iconForDimension(int id)
{
    switch(id)
    {
    case 0:
        return "qrc:/resources/icons/shield.svg";
    case 1:
        return "qrc:/resources/icons/route.svg";
    default:
        return QString();
    }
}

inline QList<journeyDimensionViewModel> toViewModels(const QVector<DimensionItem>& dimensions)
{
    QList<journeyDimensionViewModel> dimensionList;
    for(const auto& dimension: dimensions)
    {
        QList<journeyEventViewModel> eventList;
        for(const auto& event: dimension.events)
        {
            eventList.append({event.id, event.description});
        }

        dimensionList.append({
            dimension.id, iconForDimension(dimension.id), dimension.name, eventList
        });
    }
    return dimensionList;
}

#endif // INDEXPARAMETERSVIEWMODELS_H
