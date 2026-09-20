#include "journeyapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <memory>

namespace {
    QString getBaseUrl() {
        QSettings settings(":/config/app.ini", QSettings::IniFormat);
        auto raw = settings.value("API/BaseUrl", "http://localhost:8000");
        return raw.toString();
    }

    QJsonDocument jsonReply(QNetworkReply* reply) {
        if(reply->error() != QNetworkReply::NoError)
            return QJsonDocument();
        QByteArray rawData = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);
        if (parseError.error != QJsonParseError::NoError)
            qWarning() << "Erro ao processar JSON" << parseError.errorString();
        return doc;
    }
}

JourneyAPI::JourneyAPI(QObject* parent)
    : QObject{parent}
    , m_manager(new QNetworkAccessManager(this))
{}

QFuture<QVariant> JourneyAPI::postJourney(const QJsonObject& journeyData)
{
    auto promise = std::make_shared<QPromise<QVariant>>();
    auto future = promise->future();
    promise->start();

    QNetworkRequest request(QUrl(getBaseUrl() + "/journeys"));
    request.setTransferTimeout(5000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto data = QJsonDocument(journeyData).toJson(QJsonDocument::Compact);
    auto reply = m_manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [promise, reply]{
        reply->deleteLater();
        auto json = jsonReply(reply);
        if(json.isObject() && json.object().contains("journey_id"))
        {
            auto id = json.object().value("journey_id");
            promise->addResult(id);
        }
        else
            promise->addResult(QVariant());
        promise->finish();
    });

    return future;
}

QFuture<QJsonObject> JourneyAPI::getJourneyScore(const QVariant& journeyID)
{
    auto promise = std::make_shared<QPromise<QJsonObject>>();
    auto future = promise->future();
    promise->start();

    QNetworkRequest request(
        QUrl(getBaseUrl() + QString("/journeys/%1/score").arg(journeyID.toString()))
    );
    request.setTransferTimeout(5000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [promise, reply]{
        reply->deleteLater();
        auto json = jsonReply(reply);
        if(json.isEmpty())
        {
            promise->addResult({});
            promise->finish();
            return;
        }
        if (!json.isObject())
        {
            qWarning() << "O JSON retornado não é um objeto na raiz.";
            promise->addResult({});
            promise->finish();
            return;
        }
        promise->addResult(json.object());
        promise->finish();
    });

    return future;
}