#ifndef POSITIONINGSERVICEFACTORY_H
#define POSITIONINGSERVICEFACTORY_H

#include <memory>

#include "src/domain/journey/ipositioningservice.h"

std::unique_ptr<IPositioningService> createService();

#endif // POSITIONINGSERVICEFACTORY_H
