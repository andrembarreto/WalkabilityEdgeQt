#ifndef JOURNEYTRACKER_H
#define JOURNEYTRACKER_H

#include <QObject>

#include "src/domain/journey/journey.h"
#include "src/domain/journey/position.h"
#include "ibackgroundtrackingservice.h"

class Stopwatch;
class IPositioningService;

class JourneyTracker : public QObject
{
    Q_OBJECT

public:
    explicit JourneyTracker(
        std::unique_ptr<IPositioningService> positioningService,
        std::unique_ptr<IBackgroundTrackingService> backgroundService,
        QObject *parent = nullptr
    );

    void startJourney();
    void finishJourney();
    void registerEvent(int eventID);

    bool journeyIsActive() const;
    int elapsedTime() const;
    const std::optional<Journey>& journey() const;

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
    std::unique_ptr<IPositioningService> m_positioningService;
    std::unique_ptr<IBackgroundTrackingService> m_backgroundService;
    int m_timeOffset_s;

    void startUpdates();
};

#endif // JOURNEYTRACKER_H
