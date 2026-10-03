#include "journeyviewmodel.h"
#include "journeydispatcherviewmodel.h"

#include "src/application/journey-tracker/journeytracker.h"
#include "src/infra/positioning/positioningservicefactory.h"
#include "src/infra/background-tracking/backgroundtrackingservicefactory.h"

JourneyViewModel::JourneyViewModel(QObject *parent)
    : QObject{parent}
    , m_tracker(new JourneyTracker(createService(), createBackgroundTrackingService(), this))
    , m_dispatcher(new JourneyDispatcherViewModel(this))
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

void JourneyViewModel::resume()
{
    if(!canResume())
        return;
    m_tracker->resumeJourney();
}

void JourneyViewModel::finish()
{
    m_tracker->finishJourney();
}

void JourneyViewModel::discard()
{
    m_tracker->discardJourney();
}

void JourneyViewModel::registerEvent(int eventID)
{
    m_tracker->registerEvent(eventID);
}

void JourneyViewModel::dispatch()
{
    if(m_tracker->journey().has_value())
        m_dispatcher->execute(m_tracker->journey().value());
}

bool JourneyViewModel::canResume() const
{
    return m_tracker->canResumeJourney();
}

bool JourneyViewModel::isActive() const
{
    return m_tracker->journeyIsActive();
}

int JourneyViewModel::elapsedTime() const
{
    return m_tracker->elapsedTime();
}

JourneyDispatcherViewModel* JourneyViewModel::dispatcher() const
{
    return m_dispatcher;
}