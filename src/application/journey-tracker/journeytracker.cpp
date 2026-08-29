#include "journeytracker.h"
#include "src/application/utils/stopwatch.h"

JourneyTracker::JourneyTracker(QObject *parent)
    : QObject{parent}
    , m_stopwatch(new Stopwatch(this))
{
    connect(m_stopwatch, &Stopwatch::updated, this, &JourneyTracker::updateElapsedTime);
}

void JourneyTracker::updateElapsedTime(int elapsed_ms)
{
    m_journey->setElapsedTime(elapsed_ms / 1000);
    emit elapsedTimeChanged();
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
}

void JourneyTracker::finishJourney()
{
    if(!journeyIsActive())
        return;

    m_journey->setActive(false);
    emit journeyStateChanged();
    m_stopwatch->stop();
}

bool JourneyTracker::journeyIsActive() const
{
    return m_journey->isActive();
}

int JourneyTracker::elapsedTime() const
{
    return m_journey->elapsedTime();
}