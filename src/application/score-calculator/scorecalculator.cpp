#include "scorecalculator.h"

ScoreCalculator::ScoreCalculator(IJourneyAPI* api, QObject* parent)
    : QObject{parent}
    , m_api{api}
{}

QFuture<ScoreCalculationResult> ScoreCalculator::execute(const QString& journeyId)
{
    auto promise = std::make_shared<QPromise<ScoreCalculationResult>>();
    auto future = promise->future();

    // TODO: call API and handle result

    promise->setException(QException());
    promise->finish();
    return future;
}