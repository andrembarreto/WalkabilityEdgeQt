#include "scorecalculatorviewmodel.h"
#include "src/application/score-calculator/scorecalculator.h"
#include "src/infra/api/journey/journeyapi.h"
#include "src/application/index-table/indextable.h"
#include "src/application/utils/waitfor.h"

#include <QSettings>

namespace {

QString getDimensionNameByID(const QVector<DimensionItem>& dimensions, int id)
{
    for(const auto& dim: dimensions)
    {
        if(dim.id == id)
            return dim.name;
    }
    return QString();
}

QList<dimensionScoreViewModel> extractDimensionScores(const QMap<QString, float>& scores)
{
    const auto dimensions = waitFor(IndexTable::instance().get());
    QList<dimensionScoreViewModel> res;
    for(auto it=scores.begin(); it!=scores.end(); ++it)
    {
        res.append({
            getDimensionNameByID(dimensions, it.key().toInt()),
            it.value()
        });
    }
    return res;
}

QString getLastSavedJourneyID()
{
    QSettings settings;
    return settings.value("lastSavedJourneyId", "").toString();
}

}

ScoreCalculatorViewModel::ScoreCalculatorViewModel(QObject *parent)
    : QObject{parent}
    , m_status(Status::Idle)
    , m_score({0.0, {}})
{}

void ScoreCalculatorViewModel::calculate()
{
    m_status = Status::Calculating;
    emit statusChanged();

    const QString lastSavedJourneyId = getLastSavedJourneyID();
    if(lastSavedJourneyId.isEmpty())
    {
        m_status = Status::Failed;
        emit statusChanged();
        return;
    }

    auto api = new JourneyAPI(this);
    auto calculator = new ScoreCalculator(api, this);

    calculator->execute(lastSavedJourneyId).then(this, [=](ScoreCalculationResult res) {
        calculator->deleteLater();
        api->deleteLater();
        if(res.success)
        {
            m_score = scoreViewModel{
                res.globalScore,
                extractDimensionScores(res.dimensionScores)
            };
            emit scoreChanged();
            m_status = Status::Ready;
        }
        else
        {
            m_status = Status::Failed;
        }
        emit statusChanged();
    });
}