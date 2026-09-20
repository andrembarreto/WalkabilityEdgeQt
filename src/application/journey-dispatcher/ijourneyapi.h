#ifndef IJOURNEYAPI_H
#define IJOURNEYAPI_H

#include <QFuture>

class IJourneyAPI
{
public:
    virtual ~IJourneyAPI() = default;
    virtual QFuture<QVariant> postJourney(const QJsonObject& journeyData) = 0;
    virtual QFuture<QJsonObject> getJourneyScore(const QVariant& journeyID) = 0;
};

#endif // IJOURNEYAPI_H
