#ifndef JOURNEYEVENTVIEWMODEL_H
#define JOURNEYEVENTVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>

class JourneyEventViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("JourneyEventViewModel só pode ser instanciada por C++")
    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:

    explicit JourneyEventViewModel(
        int id,
        const QString& name,
        QObject *parent = nullptr
    )
        : QObject{parent}
        , m_id(id)
        , m_name(name)
    {}

    int id() const { return m_id; }
    const QString& name() const { return m_name; }

private:
    const int m_id;
    const QString m_name;
};

#endif // JOURNEYEVENTVIEWMODEL_H
