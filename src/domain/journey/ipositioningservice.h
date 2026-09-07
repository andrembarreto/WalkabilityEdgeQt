#ifndef IPOSITIONINGSERVICE_H
#define IPOSITIONINGSERVICE_H

#include <QObject>

#include "position.h"

Q_DECLARE_METATYPE(Position)

class IPositioningService : public QObject
{
    Q_OBJECT
public:
    explicit IPositioningService(QObject* parent = nullptr) : QObject{parent} {}
    virtual ~IPositioningService() = default;

    virtual void startUpdates(int updateInterval_ms) = 0;
    virtual void stopUpdates() = 0;

signals:
    void updated(const Position& pos);
};

#endif // IPOSITIONINGSERVICE_H
