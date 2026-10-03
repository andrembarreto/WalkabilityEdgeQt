#ifndef APIUTILS_H
#define APIUTILS_H

#include <QString>
#include <QJsonDocument>

class QNetworkReply;

namespace ApiUtils {
    QString getBaseUrl();
    QJsonDocument jsonReply(QNetworkReply* reply);
}

#endif // APIUTILS_H
