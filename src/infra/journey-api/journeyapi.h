#ifndef JOURNEYAPI_H
#define JOURNEYAPI_H

#include <QObject>

#include "src/application/journey-dispatcher/ijourneyapi.h"

class JourneyAPI : public QObject, public IJourneyAPI
{
    Q_OBJECT
public:
    JourneyAPI(QObject* parent = nullptr);

    QFuture<bool> post(const QJsonObject&) override;
};

#endif // JOURNEYAPI_H
