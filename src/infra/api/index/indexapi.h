#ifndef INDEXAPI_H
#define INDEXAPI_H

#include <QObject>
#include <QFuture>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include "src/application/index-table/iindexapi.h"

class IndexAPI : public QObject, public IIndexAPI
{
    Q_OBJECT

public:
    explicit IndexAPI(QObject* parent = nullptr);

    QFuture<QJsonObject> fetchIndexTable() override;

private:
    QNetworkAccessManager* const m_manager;
};

#endif // INDEXAPI_H
