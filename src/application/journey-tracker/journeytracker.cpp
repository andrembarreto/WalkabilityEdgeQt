#include "journeytracker.h"
#include "src/application/utils/stopwatch.h"
#include "src/domain/journey/ipositioningservice.h"
#include "src/domain/journey/event.h"
#include "src/application/journey-cache/journeycache.h"

JourneyTracker::JourneyTracker(
    std::unique_ptr<IPositioningService> positioningService,
    std::unique_ptr<IBackgroundTrackingService> backgroundService,
    QObject *parent
)
    : QObject{parent}
    , m_stopwatch(new Stopwatch(this))
    , m_positioningService(std::move(positioningService))
    , m_backgroundService(std::move(backgroundService))
    , m_timeOffset_s(0)
{
    connect(
        m_stopwatch, &Stopwatch::updated,
        this, &JourneyTracker::updateElapsedTime
    );

    connect(
        m_positioningService.get(), &IPositioningService::updated,
        this, &JourneyTracker::onPositionUpdated
    );

    if(cache::journey::active::check())
    {
        auto activeJourney = cache::journey::active::load();
        if(activeJourney.has_value())
        {
            m_timeOffset_s = activeJourney->elapsedTime;
            m_journey = activeJourney;
            m_journey->isActive = true;
            startUpdates();
        }
    }
}

void JourneyTracker::updateElapsedTime(int elapsed_ms)
{
    if(!journeyIsActive())
        return;

    int elapsed_s = (elapsed_ms / 1000) + m_timeOffset_s;
    m_journey->elapsedTime = elapsed_s;
    emit elapsedTimeChanged();
}

void JourneyTracker::onPositionUpdated(const Position& pos)
{
    if(!journeyIsActive() || m_lastKnownPosition == pos)
        return;

    m_lastKnownPosition = pos;
    m_journey->route.push_back(pos);
    cache::journey::active::putPosition(pos);
}

void JourneyTracker::startJourney()
{
    if(journeyIsActive())
        return;

    m_journey = Journey();
    m_journey->isActive = true;
    emit journeyStateChanged();
    cache::journey::active::init();
    startUpdates();
}

void JourneyTracker::startUpdates()
{
    updateElapsedTime(0);
    m_stopwatch->start();
    m_lastKnownPosition.reset();
    m_positioningService->startUpdates(1000);
    m_backgroundService->start();
}

void JourneyTracker::finishJourney()
{
    if(!journeyIsActive())
        return;

    m_journey->isActive = false;
    emit journeyStateChanged();
    m_timeOffset_s = 0;
    m_stopwatch->stop();
    m_positioningService->stopUpdates();
    m_backgroundService->stop();
    cache::journey::active::clear();
}

void JourneyTracker::registerEvent(int eventID)
{
    if(!journeyIsActive() || !m_lastKnownPosition.has_value())
        return;

    Event newEvent{eventID, m_lastKnownPosition.value()};
    m_journey->events.push_back(newEvent);
    cache::journey::active::putEvent(newEvent);
}

bool JourneyTracker::journeyIsActive() const
{
    return m_journey.has_value() && m_journey->isActive;
}

int JourneyTracker::elapsedTime() const
{
    return journeyIsActive() ? m_journey->elapsedTime : 0;
}

const std::optional<Journey>& JourneyTracker::journey() const
{
    return m_journey;
}