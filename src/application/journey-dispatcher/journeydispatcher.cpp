#include "journeydispatcher.h"

JourneyDispatcher::JourneyDispatcher(QObject *parent)
    : QObject{parent}
{}

QFuture<JourneyDispatchResult> JourneyDispatcher::execute(const Journey& journey)
{

}