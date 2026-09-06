#ifndef APPDATA_H
#define APPDATA_H

#include <QList>

#include "src/presentation/journey/journeydimensionviewmodel.h"

namespace appdata {

QList<JourneyDimensionViewModel*> loadData();

}

#endif // APPDATA_H
