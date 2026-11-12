// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/medal.h: the lines in use so far.
#ifndef BWPORT_FIELD_MEDAL_H
#define BWPORT_FIELD_MEDAL_H

#include "types.h"

// Black 2's, not checked against Black
// A medal's entry in archive 0xeb
typedef struct {
    u8 unk00[9];
    // An index into MEDAL_TYPE_TO_CATEGORY, and the medal's field effect after 0x4e
    u8 type;
    // Whether Mr. Medal hints at the medal while it is unknown
    u8 hintable;
    u8 unk0B;
} MedalData;

#endif // BWPORT_FIELD_MEDAL_H
