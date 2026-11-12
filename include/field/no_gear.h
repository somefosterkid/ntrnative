// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/no_gear.h: the lines in use so far.
#ifndef BWPORT_FIELD_NO_GEAR_H
#define BWPORT_FIELD_NO_GEAR_H

#include "types.h"
#include "struct_decls.h"

typedef struct NoGearWork NoGearWork;

void func_ov030_021f00f8(NoGearWork *work);
void func_ov030_021f0118(NoGearWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The first two arguments are unused
NoGearWork *func_ov030_021f00c8(void *saveData, FieldSubscreen *subscreen, GameSystem *gsys);
void func_ov030_021f0114(NoGearWork *work);
void func_ov030_021f0058(NoGearWork *work);

#endif // BWPORT_FIELD_NO_GEAR_H
