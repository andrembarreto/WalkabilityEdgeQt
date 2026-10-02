#include "scorecalculatorviewmodel.h"
#include "src/application/score-calculator/scorecalculator.h"
#include "src/infra/journey-api/journeyapi.h"
#include "src/application/journey-cache/journeycache.h"

ScoreCalculatorViewModel::ScoreCalculatorViewModel(QObject *parent)
    : QObject{parent}
    , m_status(Status::Idle)
{}

void ScoreCalculatorViewModel::calculate()
{
    m_status = Status::Calculating;
    emit statusChanged();

    auto api = new JourneyAPI(this);
    auto calculator = new ScoreCalculator(api, this);

    std::optional<std::string> savedID = cache::getSavedJourneyID();
    if(!savedID.has_value())
    {
        m_status = Status::Failed;
        emit statusChanged();
    }
    QString idToCalculate = QString::fromStdString(savedID.value());

    calculator->execute(idToCalculate).then(this, [=](ScoreCalculationResult res){
        calculator->deleteLater();
        api->deleteLater();
        if(res.success)
        {
            // TODO: build score
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
