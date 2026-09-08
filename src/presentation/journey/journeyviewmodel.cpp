#include "journeyviewmodel.h"
#include "src/application/journey-tracker/journeytracker.h"
#include "src/infra/positioning/positioningservicefactory.h"

JourneyViewModel::JourneyViewModel(QObject *parent)
    : QObject{parent}
    , m_tracker(new JourneyTracker(createService(), this))
{
    connect(
        m_tracker, &JourneyTracker::journeyStateChanged,
        this, &JourneyViewModel::isActiveChanged
    );

    connect(
        m_tracker, &JourneyTracker::elapsedTimeChanged,
        this, &JourneyViewModel::elapsedTimeChanged
    );
}

void JourneyViewModel::start()
{
    m_tracker->startJourney();
}

void JourneyViewModel::finish()
{
    m_tracker->finishJourney();
}

void JourneyViewModel::registerEvent(int eventID)
{
    m_tracker->registerEvent(eventID);
}

bool JourneyViewModel::isActive() const
{
    return m_tracker->journeyIsActive();
}

int JourneyViewModel::elapsedTime() const
{
    return m_tracker->elapsedTime();
}