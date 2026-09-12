#ifndef IJOURNEYAPI_H
#define IJOURNEYAPI_H

#include <QFuture>

class IJourneyAPI
{
public:
    virtual ~IJourneyAPI() = default;
    virtual QFuture<bool> post(const QJsonObject&) = 0;
};

#endif // IJOURNEYAPI_H
