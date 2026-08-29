#ifndef JOURNEYTRACKER_H
#define JOURNEYTRACKER_H

#include <QObject>

#include "src/domain/journey/journey.h"

class Stopwatch;

class JourneyTracker : public QObject
{
    Q_OBJECT
public:
    explicit JourneyTracker(QObject *parent = nullptr);

    void startJourney();
    void finishJourney();

    bool journeyIsActive() const;
    int elapsedTime() const;

signals:
    void journeyStateChanged();
    void elapsedTimeChanged();

private slots:
    void updateElapsedTime(int elapsed_ms);

private:
    std::optional<Journey> m_journey;
    Stopwatch* const m_stopwatch;
};

#endif // JOURNEYTRACKER_H
