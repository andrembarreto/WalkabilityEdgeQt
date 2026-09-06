#ifndef JOURNEYDIMENSIONVIEWMODEL_H
#define JOURNEYDIMENSIONVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>

#include "journeyeventviewmodel.h"

class JourneyDimensionViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("JourneyDimensionViewModel só pode ser instanciada por C++")
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QList<JourneyEventViewModel*> events READ events CONSTANT)

public:
    explicit JourneyDimensionViewModel(
        int id,
        const QString& icon,
        const QString& name,
        const QList<JourneyEventViewModel*>& events,
        QObject *parent = nullptr
    )
        : QObject{parent}
        , m_id(id)
        , m_icon(icon)
        , m_name(name)
        , m_events(std::move(events))
    {}

    int id() const { return m_id; }
    const QString& icon() const { return m_icon; }
    const QString& name() const { return m_name; }
    const QList<JourneyEventViewModel*>& events() const { return m_events; }

private:
    const int m_id;
    const QString m_icon;
    const QString m_name;
    const QList<JourneyEventViewModel*> m_events;
};

#endif // JOURNEYDIMENSIONVIEWMODEL_H
