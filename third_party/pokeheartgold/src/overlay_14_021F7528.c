#include "global.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);
void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
s32 ov14_021F7AC4(void *);
void ov14_021F7594(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

void ov14_021F7528(void *arg0, u8 arg1, s32 arg2) {
    u32 v0;
    u8 v1;
    v1 = arg1;
    if (arg2 != (-1)) {
        if (ov14_021E8544(((UnkStruct_ov14_boxsys *)((FieldSystem *)arg0)->areaDataManager)->unk_2F0) == 0) {
            v0 = v1 - 0x1E;
            switch (v0) {
            case 0:

            case 3:

            case 4:

            case 5:
                break;

            case 1:

            case 2:
                v1 = 0x1E;
                break;

            default:
                v1 = *((u8 *)(((u8 *)arg0) + 0x21));
                if (((u32)v1) >= 0x1EU) {
                    v1 = 0;
                }
                break;
            }

        } else if (v1 != 0x24) {
            v1 = 0x24;
        }
    }
    arg1 = 0xFFFFu;
    GridInputHandler_SetNextInput(*((GridInputHandler **)(((u8 *)(((void *)((FieldSystem *)arg0)->areaDataManager))) + 0x2C)), (s32)(v1 & arg1));
    ov14_021F6B28(arg0, v1);
}

void ov14_021F7594(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F7AC4(arg0->sys);
    ov14_021E5A50(arg0->sys, ov14_021E9F20);
}

void ov14_021F75AC(void *arg0, u8 arg1) {
    s32 new_var;
    new_var = (s32)(((((((((arg1 & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu) & 0xFFu);
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, new_var);
    ov14_021F6B28(arg0, arg1);
}
