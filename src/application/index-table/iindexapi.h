#ifndef IINDEXAPI_H
#define IINDEXAPI_H

#include <QFuture>
#include <QJsonObject>

class IIndexAPI
{
public:
    virtual ~IIndexAPI() = default;
    virtual QFuture<QJsonObject> fetchIndexTable() = 0;
};

#endif // IINDEXAPI_H
