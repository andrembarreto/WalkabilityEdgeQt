#include "appdata.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QGuiApplication>

namespace appdata {

static QJsonObject loadJson(const char* dataFile)
{
    QFile file(dataFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "could not open" << dataFile << "for reading";
        return QJsonObject();
    }

    const QByteArray data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data);

    if(document.isNull())
    {
        qWarning() << dataFile << ": not JSON";
        return QJsonObject();
    }

    return document.object();
}

QList<JourneyDimensionViewModel*> loadData()
{
    QJsonObject object = loadJson(":/data/dimensions-and-events.json");
    QJsonValue dimensions = object.value("dimensions");

    if(dimensions.isUndefined() || !dimensions.isArray())
    {
        qWarning() << "missing or malformed dimensions in data file";
        return {};
    }

    QList<JourneyDimensionViewModel*> dimensionList;
    for(const auto& dimensionRef: dimensions.toArray())
    {
        QJsonObject dimension = dimensionRef.toObject();
        int id = dimension.value("id").toInt();
        QString name = dimension.value("name").toString();
        QString icon = dimension.value("icon").toString();
        QJsonValue events = dimension.value("events");

        if(!events.isUndefined() && events.isArray())
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

}