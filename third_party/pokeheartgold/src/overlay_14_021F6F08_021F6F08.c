#include "global.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

void ov14_021F6F08(void *arg0, u8 arg1, s32 arg2) {
    u8 v0;
    v0 = arg1;
    if (arg2 != (-1)) {
        if (ov14_021E8544(((UnkStruct_ov14_boxsys *)((FieldSystem *)arg0)->areaDataManager)->unk_2F0) == 0) {
            if (((u32)(v0 - 0x1F)) <= 1U) {
                v0 = 0x1E;
            } else if (((v0 != 0x1E) && (v0 != 0x21)) && ((v0 = *((u8 *)(((u8 *)arg0) + 0x21)), v0 == 0xFF) || (((u32)v0) >= 0x1EU))) {
                v0 = 0;
            }
        } else if (((v0 != 0x23) && (v0 != 0x24)) && (v0 != 0x25)) {
            v0 = 0x22;
        }
    }
    GridInputHandler_SetNextInput(*((GridInputHandler **)(((u8 *)(((void *)((FieldSystem *)arg0)->areaDataManager))) + 0x2C)), ((s32)v0) & 0xFFu);
    ov14_021F6B28(arg0, v0);
}
