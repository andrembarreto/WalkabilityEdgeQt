#ifndef MOCKJOURNEYAPI_H
#define MOCKJOURNEYAPI_H

#include <QObject>
#include <QDebug>
#include <memory>
#include <QTimer>
#include <QPromise>
#include <QJsonObject>

#include "src/application/journey-dispatcher/ijourneyapi.h"

class MockJourneyAPI : public IJourneyAPI
{
public:
    QFuture<QVariant> postJourney(const QJsonObject& journeyData) override {
        qDebug() << journeyData;
        auto promise = std::make_shared<QPromise<QVariant>>();
        auto future = promise->future();
        promise->start();
        QTimer::singleShot(2000, [=](){
            promise->addResult(true);
            promise->finish();
        });
        return future;
    }

    QFuture<QJsonObject> getJourneyScore(const QVariant& journeyID) override {
        auto promise = std::make_shared<QPromise<QJsonObject>>();
        auto future = promise->future();
        promise->start();
        QTimer::singleShot(2000, [=](){
            promise->addResult({});
            promise->finish();
        });
        return future;
    }
};
#endif // MOCKJOURNEYAPI_H
