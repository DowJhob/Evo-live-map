#ifndef MAPDEFINITION_H
#define MAPDEFINITION_H

#include "src/abstract-memory.h"
#include "src/map-decl/map.h"
typedef struct                                       // Содержимое таблицы
{
    Map *declMap;
    offsetMemory Map;
    offsetMemory X_axis;
    offsetMemory Y_axis;
} mapDefinition;

#endif // MAPDEFINITION_H
