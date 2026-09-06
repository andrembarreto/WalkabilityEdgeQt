#include "appdata.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QGuiApplication>

namespace appdata {

QList<JourneyDimensionViewModel*> loadData()
{
    QFile file(":/data/dimensions-and-events.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "could not open dimensions-and-events.json for reading";
        return {};
    }

    const QByteArray data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data);

    if(document.isNull())
    {
        qWarning() << "dimensions-and-events.json: bad format";
        return {};
    }

    QJsonObject object = document.object();
    QJsonValue dimensions = object.value("dimensions");
    if(dimensions.isArray())
    {
        QList<JourneyDimensionViewModel*> dimensionList;
        for(const auto& dimensionRef: dimensions.toArray())
        {
            QJsonObject dimension = dimensionRef.toObject();
            int id = dimension.value("id").toInt();
            QString name = dimension.value("name").toString();
            QString icon = dimension.value("icon").toString();
            QJsonValue events = dimension.value("events");
            if(events.isArray())
            {
                QList<JourneyEventViewModel*> eventList;
                for(const auto& eventRef: events.toArray())
                {
                    QJsonObject event = eventRef.toObject();
                    int id = event.value("id").toInt();
                    QString name = event.value("name").toString();
                    eventList.append(new JourneyEventViewModel(id, name, qApp));
                }
                dimensionList.append(new JourneyDimensionViewModel(id, icon, name, eventList, qApp));
            }
        }
        return dimensionList;
    }

    qWarning() << "dimensions-and-events.json: bad format";
    return {};
}

}