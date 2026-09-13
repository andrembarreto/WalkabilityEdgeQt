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
}

JourneyAPI::JourneyAPI(QObject* parent)
    : QObject{parent}
{}

QFuture<bool> JourneyAPI::post(const QJsonObject& obj)
{
    auto promise = std::make_shared<QPromise<bool>>();
    auto future = promise->future();
    promise->start();

    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply){
        bool success = reply->error() == QNetworkReply::NoError;
        promise->addResult(success);
        promise->finish();
        reply->deleteLater();
    });
    QNetworkRequest request(QUrl(getBaseUrl() + "/journeys"));
    request.setTransferTimeout(5000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    manager->post(request, data);

    return future;
}