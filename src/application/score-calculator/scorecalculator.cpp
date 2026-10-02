#include "scorecalculator.h"
#include "src/application/journey-dispatcher/ijourneyapi.h"

#include <QJsonObject>

ScoreCalculator::ScoreCalculator(IJourneyAPI* api, QObject* parent)
    : QObject{parent}
    , m_api{api}
{}

QFuture<ScoreCalculationResult> ScoreCalculator::execute(const QString& journeyId)
{
    auto promise = std::make_shared<QPromise<ScoreCalculationResult>>();
    auto future = promise->future();

    m_api->getJourneyScore(journeyId).then(this, [=](QJsonObject scoreData) {
        if(scoreData.isEmpty())
        {
            promise->addResult({false, 0, {}});
            promise->finish();
            return;
        }
        float globalScore = scoreData.value("global_score").toDouble();
        QJsonObject dimensionScoresObj = scoreData.value("dimension_scores").toObject();
        QMap<QString, float> dimensionScores;
        for(const QString& key : dimensionScoresObj.keys())
        {
            dimensionScores.insert(key, dimensionScoresObj.value(key).toDouble());
        }
        promise->addResult(ScoreCalculationResult{true, globalScore, dimensionScores});
        promise->finish();
    });

    return future;
}