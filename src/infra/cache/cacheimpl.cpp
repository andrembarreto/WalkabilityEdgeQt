#include "src/application/journey-cache/journeycache.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

namespace cache {

namespace {

QString cacheDirPath()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    return path;
}

void appendLine(const QString& fileName, const QString& line)
{
    QFile file(cacheDirPath() + QDir::separator() + fileName);
    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        qWarning() << "Failed to open file for appending:" << file.fileName();
        return;
    }
    QTextStream stream(&file);
    stream << line << '\n';
}

QStringList readLines(const QString& fileName)
{
    QFile file(cacheDirPath() + QDir::separator() + fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QStringList lines;
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        const QString line = stream.readLine();
        if(!line.isEmpty())
            lines.append(line);
    }
    return lines;
}

}

void putSavedJourneyID(const std::string& ID)
{
    QSettings settings;
    settings.setValue("lastSavedJourneyId", QString::fromStdString(ID));
    settings.sync();
}

void putPosition(const Position& pos)
{
    const QString line = QString("%1,%2,%3")
                              .arg(pos.timestamp)
                              .arg(pos.latitude, 0, 'g', 17)
                              .arg(pos.longitude, 0, 'g', 17);
    appendLine("route.csv", line);
}

void putEvent(const Event& event)
{
    const QString line = QString("%1,%2,%3,%4")
                              .arg(event.eventID)
                              .arg(event.position.timestamp)
                              .arg(event.position.latitude, 0, 'g', 17)
                              .arg(event.position.longitude, 0, 'g', 17);
    appendLine("events.csv", line);
}

void resetJourney()
{
    cleanJourney();

    QSettings settings;
    settings.setValue("activeJourney", true);
    settings.setValue("journeyStartTime", QDateTime::currentSecsSinceEpoch());
    settings.sync();
}

void cleanJourney()
{
    QSettings settings;
    settings.setValue("activeJourney", false);
    settings.remove("journeyStartTime");
    settings.sync();

    QFile::remove(cacheDirPath() + QDir::separator() + "route.csv");
    QFile::remove(cacheDirPath() + QDir::separator() + "events.csv");
}

std::optional<Journey> getJourney()
{
    QSettings settings;

    const QVariant isActive = settings.value("activeJourney", false);
    if(!isActive.isValid() || !isActive.toBool())
        return std::nullopt;

    Journey journey;
    const QVariant journeyStartTime = settings.value("journeyStartTime");
    journey.elapsedTime = static_cast<int>(
        QDateTime::currentSecsSinceEpoch() - journeyStartTime.toLongLong()
    );

    for(const QString& line : readLines("route.csv"))
    {
        const QStringList fields = line.split(',');
        if(fields.size() != 3)
            continue;

        journey.route.push_back(Position{
            fields.at(1).toDouble(),
            fields.at(2).toDouble(),
            static_cast<timestamp_t>(fields.at(0).toLongLong())
        });
    }

    for(const QString& line : readLines("events.csv"))
    {
        const QStringList fields = line.split(',');
        if(fields.size() != 4)
            continue;

        journey.events.push_back(Event{
            fields.at(0).toInt(),
            Position{
                fields.at(2).toDouble(),
                fields.at(3).toDouble(),
                static_cast<timestamp_t>(fields.at(1).toLongLong())
            }
        });
    }

    return journey;
}

std::optional<std::string> getSavedJourneyID()
{
    QSettings settings;
    const QVariant value = settings.value("lastSavedJourneyId");
    if(value.isValid() && value.canConvert<QString>())
    {
        return value.toString().toStdString();
    }
    return std::nullopt;
}

}
