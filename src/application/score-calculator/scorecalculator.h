#ifndef SCORECALCULATOR_H
#define SCORECALCULATOR_H

#include <QObject>
#include <QFuture>

#include "scorecalculationresult.h"

class IJourneyAPI;

class ScoreCalculator : public QObject
{
    Q_OBJECT

public:
    ScoreCalculator(IJourneyAPI* api, QObject* parent = nullptr);

    QFuture<ScoreCalculationResult> execute(const QString& journeyId);

private:
    IJourneyAPI* const m_api;
};

#endif // SCORECALCULATOR_H
