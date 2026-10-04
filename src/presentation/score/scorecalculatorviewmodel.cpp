#include "scorecalculatorviewmodel.h"
#include "src/application/score-calculator/scorecalculator.h"
#include "src/infra/api/journey/journeyapi.h"
#include "src/application/index-table/indextable.h"
#include "src/application/utils/waitfor.h"
#include "src/application/journey-cache/journeycache.h"

#include <QSettings>

namespace {

namespace fcache = cache::journey::finished;

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

void ScoreCalculatorViewModel::calculate(int journeyID)
{
    m_status = Status::Calculating;
    emit statusChanged();

    if(journeyID < 0)
    {
        calculateFor(getLastSavedJourneyID());
        return;
    }

    fcache::get(journeyID).then(this, [this](std::optional<fcache::JourneyEntry> entry) {
        calculateFor(entry.has_value()
                         ? entry->metadata.dispatchedResourceId.value_or(QString())
                         : QString());
    });
}

void ScoreCalculatorViewModel::calculateFor(const QString& resourceId)
{
    if(resourceId.isEmpty())
    {
        m_status = Status::Failed;
        emit statusChanged();
        return;
    }

    auto api = new JourneyAPI(this);
    auto calculator = new ScoreCalculator(api, this);

    calculator->execute(resourceId).then(this, [=](ScoreCalculationResult res) {
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