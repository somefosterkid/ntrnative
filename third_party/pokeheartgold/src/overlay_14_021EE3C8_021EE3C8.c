#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021F0244(void *param_1, u32 param_2);
u32 ov14_021F1504(void *param_1, s32 param_2);
u32 ov14_021F1534(void *param_1, s32 arg1);
u32 ov14_021F1540(void *param_1);
void ov14_021F29E4(UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);

u32 ov14_021EE3C8(void *param_1) {
    s32 input = GridInputHandler_HandleInput_AllowHold(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C);

    switch ((u32)input) {
    case 0:
        return ov14_021F1448(param_1, 0);
    case 1:
        return ov14_021F1448(param_1, 1);
    case 2:
        return ov14_021F1448(param_1, 2);
    case 3:
        return ov14_021F1448(param_1, 3);
    case 4:
        return ov14_021F1448(param_1, 4);
    case 5:
        return ov14_021F1448(param_1, 5);
    case 6:
        PlaySE(0x5dc);
        return ov14_021F1504(param_1, -1);
    case 7:
        PlaySE(0x5dc);
        return ov14_021F1504(param_1, 1);
    case 8:
        return ov14_021F1540(param_1);
    case -3:
        PlaySE(0x5dc);
        if (((UnkStruct_ov14_boxapp *)param_1)->sys->unk_8 != 0) {
            return ov14_021F0244(param_1, 0x71);
        }
        break;
    case 9:
        ov14_021F29E4(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), 9, 8);
    case -2:
        PlaySE(0x5dc);
        return ov14_021F1534(param_1, 0);
    }
    return 0x61;
}
