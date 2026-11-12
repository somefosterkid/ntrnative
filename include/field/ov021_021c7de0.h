#ifndef BWPORT_FIELD_OV021_021C7DE0_H
#define BWPORT_FIELD_OV021_021C7DE0_H

#include "types.h"
#include "field/field_flash.h"

// pokebw2's FieldFlash_Free, _Update, _Draw, _RequestState and _GetState
void func_ov021_021c7de0(FieldFlash *flash);
void func_ov021_021c7dfc(FieldFlash *flash);
void func_ov021_021c7e30(FieldFlash *flash);
// Changes the darkness at the next update (FieldFlash's state)
void func_ov021_021c7e50(FieldFlash *flash, u16 nextState);
u32 func_ov021_021c7e54(FieldFlash *flash);
void FieldFlashAnimation_Init(FieldFlashAnimation *animation);

#endif // BWPORT_FIELD_OV021_021C7DE0_H
