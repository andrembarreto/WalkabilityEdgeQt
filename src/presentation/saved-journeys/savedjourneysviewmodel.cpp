#include "savedjourneysviewmodel.h"
#include "src/application/journey-cache/journeycache.h"

namespace {

namespace fcache = cache::journey::finished;

}

SavedJourneysViewModel::SavedJourneysViewModel(QObject *parent)
    : QObject{parent}
{}

void SavedJourneysViewModel::fetch()
{

    m_journeys.clear();

    fcache::get().then(this, [this](QVector<fcache::JourneyMetadata> journeys) {
        for(const auto& journey: journeys)
        {
            savedJourneyViewModel viewModel {
                static_cast<int>(journey.id),
                QDateTime::fromSecsSinceEpoch(journey.initTimestamp_s),
                journey.duration_s,
                journey.dispatched
            };
            m_journeys.append(viewModel);
        }
        emit journeysChanged();
    });
}