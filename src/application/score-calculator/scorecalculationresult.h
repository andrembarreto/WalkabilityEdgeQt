#ifndef SCORECALCULATIONRESULT_H
#define SCORECALCULATIONRESULT_H

#include <QMap>

struct ScoreCalculationResult
{
    bool success;
    float globalScore;
    QMap<QString, float> dimensionScores;
};

#endif // SCORECALCULATIONRESULT_H
