#include "global.h"

#include "unk_02019BA4.h"
#include "field_system.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F6B28(void *, s32);

void ov14_021F73E4(void *arg0, s32 arg1, s32 arg2) {
    s32 var_r4;
    void *var_r5;

    var_r5 = arg0;
    var_r4 = arg1;
    if (arg2 != -1) {
        if (ov14_021E8544(((UnkStruct_ov14_boxsys *)((FieldSystem *)var_r5)->areaDataManager)->unk_2F0) == 0) {
            if (var_r4 >= 8) {
                var_r4 = (s32) * (u8 *)((u8 *)var_r5 + 0x21);
                if ((u32)var_r4 >= 0x1EU) {
                    var_r4 -= 0x1E;
                } else {
                    var_r4 = 0;
                }
            }
        } else if ((var_r4 != 8) || (var_r4 != 9) || (var_r4 != 0xA) || (var_r4 != 0xB) || (var_r4 != 0xC)) {
            var_r4 = 8;
        }
    }
    GridInputHandler_SetNextInput(*(GridInputHandler **)((u8 *)(((void *)((FieldSystem *)var_r5)->areaDataManager)) + 0x2C), (s32)(u8)var_r4);
    ov14_021F6B28(var_r5, var_r4);
}
