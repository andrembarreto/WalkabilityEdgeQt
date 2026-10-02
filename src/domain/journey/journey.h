#ifndef JOURNEY_H
#define JOURNEY_H

#include <vector>

#include "position.h"
#include "event.h"

struct Journey
{
    bool isActive {false};
    int elapsedTime {0};
    std::vector<Position> route;
    std::vector<Event> events;
};

#endif // JOURNEY_H
