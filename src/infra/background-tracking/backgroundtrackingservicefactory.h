#ifndef BACKGROUNDTRACKINGSERVICEFACTORY_H
#define BACKGROUNDTRACKINGSERVICEFACTORY_H

#include <memory>

#include "src/application/journey-tracker/ibackgroundtrackingservice.h"

std::unique_ptr<IBackgroundTrackingService> createBackgroundTrackingService();

#endif // BACKGROUNDTRACKINGSERVICEFACTORY_H
