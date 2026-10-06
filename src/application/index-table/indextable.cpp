#include "indextable.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPromise>
#include <QSaveFile>
#include <QStandardPaths>
#include <memory>

namespace {

constexpr const char* kBundledFile = ":/data/dimensions-and-events.json";

QString cacheFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/dimensions-and-events.json";
}

bool isValid(const QJsonObject& object)
{
    return object.value("dimensions").isArray();
}

void saveLocal(const QJsonObject& object)
{
    const QString path = cacheFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "could not open" << path << "for writing";
        return;
    }
    file.write(QJsonDocument(object).toJson(QJsonDocument::Compact));
    if(!file.commit())
        qWarning() << "could not write" << path;
}

QJsonObject fetchLocal(const QString& dataFile)
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

QJsonObject loadFallback()
{
    QJsonObject object;
    const QString cache = cacheFilePath();
    if(QFile::exists(cache))
        object = fetchLocal(cache);
    if(!isValid(object))
        object = fetchLocal(kBundledFile);
    return object;
}

QVector<DimensionItem> parse(const QJsonObject& object)
{
    QJsonValue dimensions = object.value("dimensions");
    if(dimensions.isUndefined() || !dimensions.isArray())
    {
        qWarning() << "missing or malformed dimensions in data file";
        return {};
    }

    QVector<DimensionItem> dimensionList;
    for(const auto& dimensionRef: dimensions.toArray())
    {
        QJsonObject dimension = dimensionRef.toObject();
        QJsonValue events = dimension.value("events");
        if(events.isUndefined() || !events.isArray())
            continue;

        QVector<EventItem> eventList;
        for(const auto& eventRef: events.toArray())
        {
            QJsonObject event = eventRef.toObject();
            eventList.append({
                event.value("id").toInt(),
                event.value("description").toString()
            });
        }
        dimensionList.append({
            dimension.value("id").toInt(),
            dimension.value("name").toString(),
            eventList
        });
    }
    return dimensionList;
}

}

IndexTable& IndexTable::instance()
{
    static IndexTable table;
    return table;
}

IndexTable::IndexTable(QObject *parent)
    : QObject{parent}
{}

void IndexTable::setApi(IIndexAPI* api)
{
    m_api = api;
}

QFuture<QVector<DimensionItem>> IndexTable::get()
{
    auto promise = std::make_shared<QPromise<QVector<DimensionItem>>>();
    auto future = promise->future();
    promise->start();

    auto finish = [promise](QJsonObject object) {
        if(isValid(object))
            saveLocal(object);
        else
        {
            qWarning() << "remote data unavailable, using local data";
            object = loadFallback();
        }
        promise->addResult(parse(object));
        promise->finish();
    };

    if(!m_api)
    {
        finish({});
        return future;
    }

    m_api->fetchIndexTable().then(this, finish);
    return future;
}
