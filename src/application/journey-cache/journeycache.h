#ifndef JOURNEYCACHE_H
#define JOURNEYCACHE_H

#include "src/domain/journey/event.h"
#include "src/domain/journey/position.h"
#include "src/domain/journey/journey.h"

#include <string>
#include <optional>

namespace cache {

/* Write operations */
void putSavedJourneyID(const std::string& ID);
void putPosition(const Position& pos);
void putEvent(const Event& event);
void putElapsedTime(int elapsedTime_s);
void cleanJourneyData();

/* Read operations */
std::optional<Journey> getJourney();
std::optional<std::string> getSavedJourneyID();

}

#endif // JOURNEYCACHE_H
