#include "indexapi.h"

#include "src/infra/api/common/apiutils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QPromise>
#include <memory>

IndexAPI::IndexAPI(QObject* parent)
    : QObject{parent}
    , m_manager(new QNetworkAccessManager(this))
{}

QFuture<QJsonObject> IndexAPI::fetchIndexTable()
{
    auto promise = std::make_shared<QPromise<QJsonObject>>();
    auto future = promise->future();
    promise->start();

    QNetworkRequest request(QUrl(ApiUtils::getBaseUrl() + "/index"));
    request.setTransferTimeout(5000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [promise, reply]{
        reply->deleteLater();
        auto json = ApiUtils::jsonReply(reply);
        if(json.isEmpty() || !json.isObject())
        {
            promise->addResult({});
            promise->finish();
            return;
        }
        promise->addResult(json.object());
        promise->finish();
    });

    return future;
}