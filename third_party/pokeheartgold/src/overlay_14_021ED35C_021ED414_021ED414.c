#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F11F8(void *, u32);
s32 ov14_021F0244(void *, u32);
s32 ov14_021F2270(void *, u32, u32);
void ov14_021F29E4(UnkStruct_ov14_boxsys *, u32, u32);

s32 ov14_021ED414(void *param_1) {
    u32 input;

    input = GridInputHandler_HandleInput_AllowHold(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C);
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 0);
    case 1:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 1);
    case 2:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 2);
    case 3:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 3);
    case 4:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 4);
    case 5:
        PlaySE(0x5dd);
        return ov14_021F1170(param_1, 5);
    case 6:
        PlaySE(0x5dc);
        return ov14_021F11F8(param_1, -1);
    case 7:
        PlaySE(0x5dc);
        return ov14_021F11F8(param_1, 1);
    case 8:
        ov14_021F29E4(((UnkStruct_ov14_boxapp *)param_1)->sys, 9, 8);
        if (((UnkStruct_ov14_boxapp *)param_1)->unk1F == ((UnkStruct_ov14_boxapp *)param_1)->unk25) {
            PlaySE(0x5f3);
            return ov14_021F2270(param_1, 0xc, 0x3d);
        }
        PlaySE(0x5dc);
        return ov14_021F2270(param_1, 0xc, 0xa2);
    case 9:
        ov14_021F29E4(((UnkStruct_ov14_boxapp *)param_1)->sys, 9, 8);
        PlaySE(0x5dd);
        return ov14_021F2270(param_1, 6, 0xa3);
    case 10:
        ov14_021F29E4(((UnkStruct_ov14_boxapp *)param_1)->sys, 9, 8);
        PlaySE(0x5dd);
        return ov14_021F2270(param_1, 7, 0xa4);
    case (u32)-3:
        PlaySE(0x5dc);
        if (((UnkStruct_ov14_boxapp *)param_1)->sys->unk_8 != 0) {
            PlaySE(0x5dc);
            return ov14_021F0244(param_1, 0x3e);
        }
        break;
    case (u32)-4:
        PlaySE(0x5dc);
        break;
    case 11:
        ov14_021F29E4(((UnkStruct_ov14_boxapp *)param_1)->sys, 9, 8);
    case (u32)-2:
        PlaySE(0x5dc);
        return ov14_021F2270(param_1, 10, 0x39);
    }
    return 0x3d;
}
