#ifndef JOURNEYAPI_H
#define JOURNEYAPI_H

#include <QObject>
#include <QNetworkAccessManager>

#include "src/application/journey-dispatcher/ijourneyapi.h"

class JourneyAPI : public QObject, public IJourneyAPI
{
    Q_OBJECT
public:
    JourneyAPI(QObject* parent = nullptr);

    QFuture<QVariant> postJourney(const QJsonObject& journeyData) override;
    QFuture<QJsonObject> getJourneyScore(const QVariant& journeyID) override;

private:
    QNetworkAccessManager* const m_manager;
};

#endif // JOURNEYAPI_H
