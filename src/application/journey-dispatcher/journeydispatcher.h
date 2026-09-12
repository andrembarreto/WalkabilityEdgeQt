#ifndef JOURNEYDISPATCHER_H
#define JOURNEYDISPATCHER_H

#include <QObject>
#include <QFuture>

#include "journeydispatchresult.h"
#include "src/domain/journey/journey.h"

class JourneyAPI;

class JourneyDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit JourneyDispatcher(JourneyAPI* api, QObject *parent = nullptr);

    QFuture<JourneyDispatchResult> execute(const Journey& journey);

private:
    JourneyAPI* const m_api;
};

#endif // JOURNEYDISPATCHER_H
