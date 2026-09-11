#ifndef JOURNEYVIEWMODEL_H
#define JOURNEYVIEWMODEL_H

#include <QObject>
#include <QtQml/qqml.h>

class JourneyTracker;
class JourneyDispatcherViewModel;

class JourneyViewModel : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(int elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)
    Q_PROPERTY(JourneyDispatcherViewModel* dispatcher READ dispatcher CONSTANT)

public:
    explicit JourneyViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void start();
    Q_INVOKABLE void finish();
    Q_INVOKABLE void registerEvent(int eventID);
    Q_INVOKABLE void dispatch();

    bool isActive() const;
    int elapsedTime() const;
    JourneyDispatcherViewModel* dispatcher() const;

signals:
    void isActiveChanged();
    void elapsedTimeChanged();

private:
    JourneyTracker* const m_tracker;
    JourneyDispatcherViewModel* const m_dispatcher;
};

#endif // JOURNEYVIEWMODEL_H
