#include "journeytracker.h"
#include "src/application/utils/stopwatch.h"
#include "src/domain/journey/ipositioningservice.h"

JourneyTracker::JourneyTracker(IPositioningService* positioningService, QObject *parent)
    : QObject{parent}
    , m_stopwatch(new Stopwatch(this))
    , m_positioningService(positioningService)
{
    connect(
        m_stopwatch, &Stopwatch::updated,
        this, &JourneyTracker::updateElapsedTime
    );

    connect(
        m_positioningService, &IPositioningService::updated,
        this, &JourneyTracker::onPositionUpdated
    );
}

void JourneyTracker::updateElapsedTime(int elapsed_ms)
{
    if(!journeyIsActive())
        return;

    m_journey->setElapsedTime(elapsed_ms / 1000);
    emit elapsedTimeChanged();
}

void JourneyTracker::onPositionUpdated(const Position& pos)
{
    if(!journeyIsActive() || m_lastKnownPosition == pos)
        return;

    m_lastKnownPosition = pos;
    m_journey->addToRoute(pos);
}

void JourneyTracker::startJourney()
{
    if(journeyIsActive())
        return;

    m_journey = Journey();
    m_journey->setActive(true);
    emit journeyStateChanged();
    updateElapsedTime(0);
    m_stopwatch->start();
    m_positioningService->startUpdates(1000);
}

void JourneyTracker::finishJourney()
{
    if(!journeyIsActive())
        return;

    m_journey->setActive(false);
    emit journeyStateChanged();
    m_stopwatch->stop();
    m_positioningService->stopUpdates();
}

bool JourneyTracker::journeyIsActive() const
{
    return m_journey.has_value() && m_journey->isActive();
}

int JourneyTracker::elapsedTime() const
{
    return m_journey->elapsedTime();
}