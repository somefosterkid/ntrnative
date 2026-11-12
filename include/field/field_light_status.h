// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_light_status.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_FIELD_LIGHT_STATUS_H
#define BWPORT_FIELD_FIELD_LIGHT_STATUS_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/gx.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's FieldLightStatus_Get
// The colors of a zone's light at a time of day and in a weather and season: the four lights' and then the materials'
// diffuse, ambient, specular and emission
void func_0201493c(u32 zoneId, u32 hour, u32 minute, u32 weather, u32 season, GXRgb *colors, HeapID heapId);

#endif // BWPORT_FIELD_FIELD_LIGHT_STATUS_H
