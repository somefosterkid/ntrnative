// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/calender.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_FIELD_CALENDER_H
#define BWPORT_FIELD_CALENDER_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// A zone with weather by date, and where its year of weather starts in the archive's first file
typedef struct {
    u16 zoneId;
    u16 offset;
} CalendarZone;

struct Calendar {
    HeapID heapId;
    GameData *gameData;
    ArcTool *arc;
    CalendarZone zones[68];
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u16 Calendar_GetWeather(Calendar *calendar, u16 zoneId);
Calendar *Calendar_Create(GameData *gameData, HeapID heapId);
void Calendar_Free(Calendar *calendar);

#endif // BWPORT_FIELD_CALENDER_H
