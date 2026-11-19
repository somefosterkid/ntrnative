#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

void ov14_021F6E68(void *arg0, s32 arg1) {
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (u8)arg1);
    if ((arg1 != 6) && (arg1 != 7)) {
        ov14_021F6B28(arg0, arg1);
    }
}
