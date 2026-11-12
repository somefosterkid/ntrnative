// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/c_gear_power_onoff.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_C_GEAR_POWER_ONOFF_H
#define BWPORT_FIELD_C_GEAR_POWER_ONOFF_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct CGearPowerOnOff CGearPowerOnOff;

// pokebw2's CGearPowerOnOff_Create
CGearPowerOnOff *func_ov035_021efec0(FieldSubscreen *subscreen, GameSystem *gsys, HeapID heapId);
// pokebw2's CGearPowerOnOff_Update
// active is unused
void func_ov035_021eff3c(CGearPowerOnOff *work, BOOL active);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CGearPowerOnOff_Free
void func_ov035_021eff2c(CGearPowerOnOff *work);

#endif // BWPORT_FIELD_C_GEAR_POWER_ONOFF_H
