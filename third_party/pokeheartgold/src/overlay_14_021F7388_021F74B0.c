#include "global.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
s32 ov14_021F74B0(FieldSystem *arg0);

s32 ov14_021F74B0(FieldSystem *arg0) {
    s32 v0;
    s32 nextInput;

    nextInput = GridInputHandler_GetNextInput(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    v0 = GridInputHandler_HandleInput_AllowHold(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C));
    if (ov14_021E8544(((UnkStruct_ov14_boxsys *)arg0->areaDataManager)->unk_2F0) == 0) {
        if (v0 == 0x24) {
            GridInputHandler_SetNextInput(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
    } else {
        if ((u32)(v0 - 0x21) <= 1U) {
            GridInputHandler_SetNextInput(*(GridInputHandler **)((u8 *)(*((void * *)&arg0->areaDataManager)) + 0x2C), nextInput);
            ov14_021F6B28(arg0, nextInput);
            return -1;
        }
        if ((v0 == 0x23) || (v0 == -2)) {
            return 0x25;
        }
    }
    return v0;
}
