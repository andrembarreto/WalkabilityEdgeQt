#include "apiutils.h"

#include <QNetworkReply>
#include <QJsonParseError>
#include <QSettings>
#include <QDebug>

QString ApiUtils::getBaseUrl() {
    QSettings settings(":/config/app.ini", QSettings::IniFormat);
    auto raw = settings.value("API/BaseUrl", "http://localhost:8000");
    return raw.toString();
}

QJsonDocument ApiUtils::jsonReply(QNetworkReply* reply) {
    if(reply->error() != QNetworkReply::NoError)
        return QJsonDocument();
    QByteArray rawData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        qWarning() << "Erro ao processar JSON" << parseError.errorString();
    return doc;
}
