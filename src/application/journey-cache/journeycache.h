#ifndef JOURNEYCACHE_H
#define JOURNEYCACHE_H

#include "src/domain/journey/event.h"
#include "src/domain/journey/position.h"
#include "src/domain/journey/journey.h"

#include <optional>
#include <QFuture>
#include <QVector>

namespace cache::journey {

namespace active {

bool check();
void init();
void clear();

void putPosition(const Position& pos);
void putEvent(const Event& event);

std::optional<Journey> get();

}

namespace finished {

struct JourneyMetadata
{

};

struct JourneyEntry
{

};

void put(const Journey& journey);

QFuture<QVector<JourneyMetadata>> get();
QFuture<JourneyEntry> get(int id);

}

}

#endif // JOURNEYCACHE_H
