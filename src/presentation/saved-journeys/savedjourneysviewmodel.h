#ifndef SAVEDJOURNEYSVIEWMODEL_H
#define SAVEDJOURNEYSVIEWMODEL_H

#include <QDateTime>
#include <QObject>
#include <qqml.h>

struct savedJourneyViewModel
{
    Q_GADGET
    QML_ELEMENT
    Q_PROPERTY(int id MEMBER id CONSTANT)
    Q_PROPERTY(QDateTime date MEMBER date CONSTANT)
    Q_PROPERTY(int duration_s MEMBER duration_s CONSTANT)
    Q_PROPERTY(bool dispatched MEMBER dispatched CONSTANT)

public:
    int id;
    QDateTime date;
    int duration_s;
    bool dispatched;
};

class SavedJourneysViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QList<savedJourneyViewModel> journeys READ journeys NOTIFY journeysChanged)

public:
    explicit SavedJourneysViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void fetch();
    QList<savedJourneyViewModel> journeys() const { return m_journeys; }

signals:
    void journeysChanged();

private:
    QList<savedJourneyViewModel> m_journeys;
};

#endif // SAVEDJOURNEYSVIEWMODEL_H
