#ifndef JOURNEYDISPATCHER_H
#define JOURNEYDISPATCHER_H

#include <QObject>
#include <QFuture>
#include <optional>

#include "journeydispatchresult.h"
#include "src/domain/journey/journey.h"

class IJourneyAPI;

class JourneyDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit JourneyDispatcher(IJourneyAPI* api, QObject *parent = nullptr);

    QFuture<JourneyDispatchResult> execute(
        const Journey& journey,
        std::optional<qint64> cachedId = std::nullopt
    );

private:
    IJourneyAPI* const m_api;
};

#endif // JOURNEYDISPATCHER_H
