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
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QList<JourneyEventViewModel*> events READ events CONSTANT)

public:
    enum class Type {
        T1, T2
    };
    Q_ENUM(Type)
    Q_PROPERTY(Type type READ type CONSTANT)

    explicit JourneyDimensionViewModel(
        const QString& icon,
        const QString& name,
        const QList<JourneyEventViewModel*>& events,
        Type type,
        QObject *parent = nullptr
    )
        : QObject{parent}
        , m_icon(icon)
        , m_name(name)
        , m_type(type)
    {}

    QString icon() const { return m_icon; }
    QString name() const { return m_name; }
    QList<JourneyEventViewModel*> events() const { return m_events; }
    Type type() const { return m_type; }

private:
    const QString m_icon;
    const QString m_name;
    const QList<JourneyEventViewModel*> m_events;
    const Type m_type;
};

#endif // JOURNEYDIMENSIONVIEWMODEL_H
