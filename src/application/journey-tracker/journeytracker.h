#ifndef JOURNEYTRACKER_H
#define JOURNEYTRACKER_H

#include <QObject>

#include "src/domain/journey/journey.h"
#include "src/domain/journey/position.h"

class Stopwatch;
class IPositioningService;

class JourneyTracker : public QObject
{
    Q_OBJECT
public:
    explicit JourneyTracker(
        IPositioningService* positioningService,
        QObject *parent = nullptr
    );

    void startJourney();
    void finishJourney();

    bool journeyIsActive() const;
    int elapsedTime() const;

signals:
    void journeyStateChanged();
    void elapsedTimeChanged();

private slots:
    void updateElapsedTime(int elapsed_ms);
    void onPositionUpdated(const Position& pos);

private:
    std::optional<Journey> m_journey;
    std::optional<Position> m_lastKnownPosition;
    Stopwatch* const m_stopwatch;
    IPositioningService* const m_positioningService;
};

#endif // JOURNEYTRACKER_H
