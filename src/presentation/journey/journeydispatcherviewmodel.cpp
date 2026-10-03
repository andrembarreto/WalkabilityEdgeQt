#include "journeydispatcherviewmodel.h"
#include "src/application/journey-dispatcher/journeydispatcher.h"
#include "src/infra/api/journey/journeyapi.h"

JourneyDispatcherViewModel::JourneyDispatcherViewModel(QObject *parent)
    : QObject{parent}
    , m_isExecuting(false)
{}

void JourneyDispatcherViewModel::execute(const Journey& journey)
{
    auto api = new JourneyAPI(this);
    auto dispatcher = new JourneyDispatcher(api, this);
    m_isExecuting = true;
    emit isExecutingChanged();
    dispatcher->execute(journey).then(this, [=](JourneyDispatchResult res){
        dispatcher->deleteLater();
        api->deleteLater();
        if(res.success)
            emit success();
        else
            emit fail(res.details);
        m_isExecuting = false;
        emit isExecutingChanged();
    });
}