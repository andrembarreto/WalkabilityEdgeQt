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
    qint64 id;
    long long initTimestamp_s;
    int duration_s;
    bool dispatched;
    std::optional<QString> dispatchedResourceId;
};

struct JourneyEntry
{
    JourneyMetadata metadata;
    Journey journey;
};

void put(const Journey& journey);
void setDispatched(qint64 id, const QString& resourceId);

QFuture<QVector<JourneyMetadata>> get();
QFuture<std::optional<JourneyEntry>> get(qint64 id);

}

}

#endif // JOURNEYCACHE_H
