#include "src/application/journey-cache/journeycache.h"

#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

// Percursos finalizados ficam num banco SQLite: metadados na tabela journey e
// rota/eventos em tabelas filhas, apagadas em cascata junto com o percurso.
namespace cache::journey::finished {

namespace {

const QString connectionName = "finished-journeys";

const char* const schema[] = {
    "CREATE TABLE IF NOT EXISTS journey ("
    "    id                     INTEGER PRIMARY KEY AUTOINCREMENT,"
    "    init_timestamp_s       INTEGER NOT NULL,"
    "    duration_s             INTEGER NOT NULL,"
    "    dispatched             INTEGER NOT NULL DEFAULT 0,"
    "    dispatched_resource_id TEXT"
    ")",
    "CREATE TABLE IF NOT EXISTS position ("
    "    journey_id INTEGER NOT NULL REFERENCES journey(id) ON DELETE CASCADE,"
    "    seq        INTEGER NOT NULL,"
    "    latitude   REAL    NOT NULL,"
    "    longitude  REAL    NOT NULL,"
    "    timestamp  INTEGER NOT NULL,"
    "    PRIMARY KEY (journey_id, seq)"
    ")",
    "CREATE TABLE IF NOT EXISTS event ("
    "    journey_id INTEGER NOT NULL REFERENCES journey(id) ON DELETE CASCADE,"
    "    seq        INTEGER NOT NULL,"
    "    event_id   INTEGER NOT NULL,"
    "    latitude   REAL    NOT NULL,"
    "    longitude  REAL    NOT NULL,"
    "    timestamp  INTEGER NOT NULL,"
    "    PRIMARY KEY (journey_id, seq)"
    ")"
};

bool exec(QSqlQuery& query)
{
    if(query.exec())
        return true;
    qWarning() << "Failed to execute query:" << query.lastQuery() << query.lastError().text();
    return false;
}

bool exec(QSqlQuery& query, const QString& sql)
{
    if(query.exec(sql))
        return true;
    qWarning() << "Failed to execute query:" << sql << query.lastError().text();
    return false;
}

// A conexao e buscada pelo nome a cada uso (e nao guardada em variavel estatica)
// para nao sobrar referencia viva no encerramento do app. Enquanto a abertura
// falhar, cada chamada tenta abrir de novo.
QSqlDatabase database()
{
    if(!QSqlDatabase::contains(connectionName))
    {
        const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(path);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(path + QDir::separator() + "journeys.sqlite");
    }

    QSqlDatabase db = QSqlDatabase::database(connectionName, false);
    if(db.isOpen())
        return db;

    if(!db.open())
    {
        qWarning() << "Failed to open database:" << db.databaseName() << db.lastError().text();
        return db;
    }

    // foreign_keys vale por conexao e vem desligado por padrao no SQLite.
    QSqlQuery query(db);
    exec(query, "PRAGMA foreign_keys = ON");
    for(const char* statement: schema)
        exec(query, statement);

    return db;
}

}

qint64 put(const Journey& journey)
{
    QSqlDatabase db = database();
    if(!db.transaction())
    {
        qWarning() << "Failed to start transaction:" << db.lastError().text();
        return -1;
    }

    const auto fail = [&db]() -> qint64 {
        db.rollback();
        return -1;
    };

    QSqlQuery journeyQuery(db);
    journeyQuery.prepare("INSERT INTO journey (init_timestamp_s, duration_s) VALUES (?, ?)");
    journeyQuery.bindValue(0, QDateTime::currentSecsSinceEpoch() - journey.elapsedTime);
    journeyQuery.bindValue(1, journey.elapsedTime);
    if(!exec(journeyQuery))
        return fail();

    const qint64 id = journeyQuery.lastInsertId().toLongLong();

    QSqlQuery positionQuery(db);
    positionQuery.prepare(
        "INSERT INTO position (journey_id, seq, latitude, longitude, timestamp) "
        "VALUES (?, ?, ?, ?, ?)"
    );
    for(int seq = 0; seq < static_cast<int>(journey.route.size()); ++seq)
    {
        const Position& pos = journey.route.at(seq);
        positionQuery.bindValue(0, id);
        positionQuery.bindValue(1, seq);
        positionQuery.bindValue(2, pos.latitude);
        positionQuery.bindValue(3, pos.longitude);
        positionQuery.bindValue(4, pos.timestamp);
        if(!exec(positionQuery))
            return fail();
    }

    QSqlQuery eventQuery(db);
    eventQuery.prepare(
        "INSERT INTO event (journey_id, seq, event_id, latitude, longitude, timestamp) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );
    for(int seq = 0; seq < static_cast<int>(journey.events.size()); ++seq)
    {
        const Event& event = journey.events.at(seq);
        eventQuery.bindValue(0, id);
        eventQuery.bindValue(1, seq);
        eventQuery.bindValue(2, event.eventID);
        eventQuery.bindValue(3, event.position.latitude);
        eventQuery.bindValue(4, event.position.longitude);
        eventQuery.bindValue(5, event.position.timestamp);
        if(!exec(eventQuery))
            return fail();
    }

    if(!db.commit())
    {
        qWarning() << "Failed to commit transaction:" << db.lastError().text();
        return fail();
    }

    return id;
}

void remove(qint64 id)
{
    QSqlQuery query(database());
    query.prepare("DELETE FROM journey WHERE id = ?");
    query.bindValue(0, id);
    exec(query);
}

void setDispatched(qint64 id, const QString& resourceId)
{
    QSqlQuery query(database());
    query.prepare("UPDATE journey SET dispatched = 1, dispatched_resource_id = ? WHERE id = ?");
    query.bindValue(0, resourceId);
    query.bindValue(1, id);
    exec(query);
}

QFuture<QVector<JourneyMetadata>> get()
{
    QVector<JourneyMetadata> metadata;

    QSqlQuery query(database());
    query.setForwardOnly(true);
    if(exec(query, "SELECT id, init_timestamp_s, duration_s, dispatched, dispatched_resource_id "
                   "FROM journey ORDER BY id"))
    {
        while(query.next())
        {
            metadata.append(JourneyMetadata{
                query.value(0).toLongLong(),
                query.value(1).toLongLong(),
                query.value(2).toInt(),
                query.value(3).toBool(),
                query.isNull(4) ? std::nullopt : std::optional<QString>(query.value(4).toString())
            });
        }
    }

    return QtFuture::makeReadyValueFuture(metadata);
}

QFuture<std::optional<JourneyEntry>> get(qint64 id)
{
    const QSqlDatabase db = database();

    QSqlQuery journeyQuery(db);
    journeyQuery.prepare(
        "SELECT init_timestamp_s, duration_s, dispatched, dispatched_resource_id "
        "FROM journey WHERE id = ?"
    );
    journeyQuery.bindValue(0, id);
    if(!exec(journeyQuery) || !journeyQuery.next())
        return QtFuture::makeReadyValueFuture(std::optional<JourneyEntry>());

    JourneyEntry entry{
        JourneyMetadata{
            id,
            journeyQuery.value(0).toLongLong(),
            journeyQuery.value(1).toInt(),
            journeyQuery.value(2).toBool(),
            journeyQuery.isNull(3)
                ? std::nullopt
                : std::optional<QString>(journeyQuery.value(3).toString())
        },
        Journey{}
    };
    entry.journey.elapsedTime = entry.metadata.duration_s;

    QSqlQuery positionQuery(db);
    positionQuery.setForwardOnly(true);
    positionQuery.prepare(
        "SELECT latitude, longitude, timestamp FROM position "
        "WHERE journey_id = ? ORDER BY seq"
    );
    positionQuery.bindValue(0, id);
    if(!exec(positionQuery))
        return QtFuture::makeReadyValueFuture(std::optional<JourneyEntry>());

    while(positionQuery.next())
    {
        entry.journey.route.push_back(Position{
            positionQuery.value(0).toDouble(),
            positionQuery.value(1).toDouble(),
            static_cast<timestamp_t>(positionQuery.value(2).toLongLong())
        });
    }

    QSqlQuery eventQuery(db);
    eventQuery.setForwardOnly(true);
    eventQuery.prepare(
        "SELECT event_id, latitude, longitude, timestamp FROM event "
        "WHERE journey_id = ? ORDER BY seq"
    );
    eventQuery.bindValue(0, id);
    if(!exec(eventQuery))
        return QtFuture::makeReadyValueFuture(std::optional<JourneyEntry>());

    while(eventQuery.next())
    {
        entry.journey.events.push_back(Event{
            eventQuery.value(0).toInt(),
            Position{
                eventQuery.value(1).toDouble(),
                eventQuery.value(2).toDouble(),
                static_cast<timestamp_t>(eventQuery.value(3).toLongLong())
            }
        });
    }

    return QtFuture::makeReadyValueFuture(std::optional<JourneyEntry>(entry));
}

}
