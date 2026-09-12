#ifndef JOURNEYAPI_H
#define JOURNEYAPI_H

#include <QFuture>

class JourneyAPI
{
public:
    virtual ~JourneyAPI() = default;
    virtual QFuture<bool> post(const QJsonObject&) = 0;
};

#endif // JOURNEYAPI_H
