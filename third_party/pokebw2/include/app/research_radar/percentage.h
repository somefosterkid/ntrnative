#ifndef POKEBW2_APP_RESEARCH_RADAR_PERCENTAGE_H
#define POKEBW2_APP_RESEARCH_RADAR_PERCENTAGE_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// A percentage shown with cell actors, a percent sign and up to three digits, which the Research Radar's graph uses
// for its answers' shares (percentage.c)
// The names of these functions and types are ours

typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 surface;
    // The digits' animation, whose frames are 0 to 9, and the percent sign's
    u16 digitSequence;
    u16 percentSequence;
} PercentageResources;

Percentage *Percentage_Create(HeapID heapId, const PercentageResources *resources, ClActUnit *unit);
void Percentage_Delete(Percentage *percentage);
// The position of the percent sign; the digits are above it, to its right
void Percentage_SetPos(Percentage *percentage, int x, int y);
void Percentage_SetValue(Percentage *percentage, u32 value);
void Percentage_SetVisible(Percentage *percentage, BOOL visible);

#endif // POKEBW2_APP_RESEARCH_RADAR_PERCENTAGE_H
