#include "journeydispatcher.h"
#include "ijourneyapi.h"
#include "journeydispatchresult.h"
#include "src/application/journey-cache/journeycache.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QPromise>
#include <QSettings>

namespace {
    QJsonObject positionToJson(const Position& position) {
        return QJsonObject {
            { "lat", position.latitude },
            { "long", position.longitude },
            { "time", position.timestamp }
        };
    }

    QJsonObject eventToJson(const Event& event) {
        return QJsonObject {
            { "id", event.eventID },
            { "pos", positionToJson(event.position) }
        };
    }

    QJsonObject journeyToJson(const Journey& journey) {
        QJsonArray route;
        for(const auto& position: journey.route)
        {
            route.append(positionToJson(position));
        }

        QJsonArray events;
        for(const auto& event: journey.events)
        {
            events.append(eventToJson(event));
        }

        return QJsonObject {
            { "route", route },
            { "events", events }
        };
    }
}

JourneyDispatcher::JourneyDispatcher(IJourneyAPI* api, QObject *parent)
    : QObject{parent}
    , m_api(api)
{}

QFuture<JourneyDispatchResult> JourneyDispatcher::execute(const Journey& journey)
{
    return m_api->postJourney(journeyToJson(journey)).then(this, [](QVariant journeyID) {
        bool success = journeyID.isValid();
        if(success)
        {
            QSettings settings;
            settings.setValue("lastSavedJourneyId", journeyID.toString());
            settings.sync();
        }
        return JourneyDispatchResult{
            success,
            success ? "Enviado" : "Falha ao enviar"
        };
    });
}