#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B28(void *, u32);

void ov14_021F6C3C(u8 *param_1, u32 param_2, s32 param_3) {
    s32 iVar1;
    u32 uVar2;

    if (param_3 != -1) {
        iVar1 = ov14_021E8544(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2F0);
        if (iVar1 == 0) {
            uVar2 = (u32) * (u8 *)(param_1 + 0x21);
            if (uVar2 != 0xff && uVar2 >= 0x1e) {
                param_2 = uVar2 - 0x1e;
            } else {
                param_2 = 0;
            }
        } else if (((param_2 != 8) && (param_2 != 9)) && (param_2 != 10)) {
            param_2 = 7;
        }
    }
    GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C, param_2 & 0xff);
    ov14_021F6B28(param_1, param_2);
    return;
}
