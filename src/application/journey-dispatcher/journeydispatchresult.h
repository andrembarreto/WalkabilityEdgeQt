#ifndef JOURNEYDISPATCHRESULT_H
#define JOURNEYDISPATCHRESULT_H

#include <QMetaType>
#include <QString>

struct JourneyDispatchResult
{
    bool success;
    QString details;
};

Q_DECLARE_METATYPE(JourneyDispatchResult)

#endif // JOURNEYDISPATCHRESULT_H
