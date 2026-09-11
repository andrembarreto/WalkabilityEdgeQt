#ifndef JOURNEYDISPATCHER_H
#define JOURNEYDISPATCHER_H

#include <QObject>
#include <QFuture>

#include "journeydispatchresult.h"
#include "src/domain/journey/journey.h"

class JourneyDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit JourneyDispatcher(QObject *parent = nullptr);

    QFuture<JourneyDispatchResult> execute(const Journey& journey);
};

#endif // JOURNEYDISPATCHER_H
