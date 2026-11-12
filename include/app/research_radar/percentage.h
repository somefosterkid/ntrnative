// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/percentage.h: the lines
// in use so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_PERCENTAGE_H
#define BWPORT_APP_RESEARCH_RADAR_PERCENTAGE_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

struct PercentageResources {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u16 surface;
    // The digits' animation, whose frames are 0 to 9, and the percent sign's
    u16 digitSequence;
    u16 percentSequence;
};

// pokebw2's Percentage_Create
Percentage *func_ov217_021c26e8(HeapID heapId, const PercentageResources *resources, ClActUnit *unit);
// pokebw2's Percentage_SetValue
void func_ov217_021c272c(Percentage *percentage, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Percentage_Delete
void func_ov217_021c270c(Percentage *percentage);
// pokebw2's Percentage_SetPos
// The position of the percent sign; the digits are above it, to its right
void func_ov217_021c271c(Percentage *percentage, int x, int y);
// pokebw2's Percentage_SetVisible
void func_ov217_021c2740(Percentage *percentage, BOOL visible);

#endif // BWPORT_APP_RESEARCH_RADAR_PERCENTAGE_H
