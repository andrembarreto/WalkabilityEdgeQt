#ifndef JOURNEY_H
#define JOURNEY_H

#include <vector>

#include "position.h"
#include "event.h"

struct Journey
{
public:
    void setActive(bool active) { m_isActive = active; }
    void setElapsedTime(int elapsedTime_s) { m_elapsedTime = elapsedTime_s; }

    bool isActive() const { return m_isActive; }
    int elapsedTime() const { return m_elapsedTime; }
    void addToRoute(const Position& pos) { m_route.push_back(pos); }
    void addToEvents(const Event& event) { m_events.push_back(event); }

private:
    bool m_isActive = false;
    int m_elapsedTime = 0;
    std::vector<Position> m_route;
    std::vector<Event> m_events;
};

#endif // JOURNEY_H
