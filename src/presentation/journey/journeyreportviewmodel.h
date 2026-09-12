#ifndef JOURNEYREPORTVIEWMODEL_H
#define JOURNEYREPORTVIEWMODEL_H

#include <QObject>
#include <QDateTime>
#include <qqml.h>

struct journeyReportViewModel
{
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(QDateTime startTime MEMBER startTime CONSTANT)
    Q_PROPERTY(QDateTime finishTime MEMBER finishTime CONSTANT)
    Q_PROPERTY(QTime duration MEMBER duration CONSTANT)
    Q_PROPERTY(float totalDistance MEMBER totalDistance CONSTANT)

public:
    QDateTime startTime;
    QDateTime finishTime;
    QTime duration;
    float totalDistance;
};

#endif // JOURNEYREPORTVIEWMODEL_H
