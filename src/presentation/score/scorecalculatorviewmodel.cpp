#include "scorecalculatorviewmodel.h"
#include "src/application/score-calculator/scorecalculator.h"
#include "src/infra/api/journey/journeyapi.h"
#include "src/application/journey-cache/journeycache.h"
#include "src/infra/data/appdata.h"

namespace {

QString getDimensionNameByID(int id)
{
    const auto dimensions = appdata::loadData();
    for(auto dim: dimensions)
    {
        if(dim->id() == id)
            return dim->name();
    }
    return QString();
}

QList<dimensionScoreViewModel> extractDimensionScores(const QMap<QString, float>& scores)
{
    QList<dimensionScoreViewModel> res;
    for(auto it=scores.begin(); it!=scores.end(); ++it)
    {
        res.append({
            getDimensionNameByID(it.key().toInt()),
            it.value()
        });
    }
    return res;
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

    std::optional<std::string> savedID = cache::getSavedJourneyID();
    if(!savedID.has_value())
    {
        m_status = Status::Failed;
        emit statusChanged();
        return;
    }
    QString idToCalculate = QString::fromStdString(savedID.value());

    auto api = new JourneyAPI(this);
    auto calculator = new ScoreCalculator(api, this);

    calculator->execute(idToCalculate).then(this, [=](ScoreCalculationResult res) {
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