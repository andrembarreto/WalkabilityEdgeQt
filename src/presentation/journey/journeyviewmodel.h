#ifndef JOURNEYVIEWMODEL_H
#define JOURNEYVIEWMODEL_H

#include <QObject>
#include <QtQml/qqml.h>

class JourneyTracker;

class JourneyViewModel : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(int elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)

public:
    explicit JourneyViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void start();
    Q_INVOKABLE void finish();
    Q_INVOKABLE void registerEvent(int eventID);

    bool isActive() const;
    int elapsedTime() const;

signals:
    void isActiveChanged();
    void elapsedTimeChanged();

private:
    JourneyTracker* const m_tracker;
};

#endif // JOURNEYVIEWMODEL_H
