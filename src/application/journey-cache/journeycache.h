#ifndef JOURNEYCACHE_H
#define JOURNEYCACHE_H

#include "src/domain/journey/event.h"
#include "src/domain/journey/position.h"
#include "src/domain/journey/journey.h"

#include <optional>

namespace cache::journey {

namespace active {

bool check();
void init();
void clear();

/* Write operations */
void putPosition(const Position& pos);
void putEvent(const Event& event);

/* Read operations */
std::optional<Journey> load();

}

}

#endif // JOURNEYCACHE_H
