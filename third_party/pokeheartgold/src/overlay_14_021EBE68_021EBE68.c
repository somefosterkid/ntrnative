#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E6094(void *param_1, u32 param_2, s32 param_3, void *param_4);
void ov14_021E7E10(UnkStruct_ov14_boxapp *param_1, u32 param_2);

void ov14_021E895C(void *param_1, u32 param_2);
u32 ov14_021F0244(void *param_1, u32 param_2);

u32 ov14_021EBE68(void *param_1) {
    u8 sp0;
    u32 input;

    input = GridInputHandler_HandleInput_AllowHold(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_2C);
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 0);
        break;
    case 1:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 1);
        break;
    case 2:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 2);
        break;
    case 3:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 3);
        break;
    case 4:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 4);
        break;
    case 5:
        PlaySE(0x5dd);
        ov14_021E7E10(param_1, 5);
        break;
    case 6:
        sp0 = *((u32 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_40C0);
        ov14_021E6094(param_1, ((UnkStruct_ov14_boxapp *)param_1)->unk21, 0xb, &sp0);
        ov14_021E895C(param_1, sp0);
        PlaySE(0x5dd);
        return ov14_021F23F0(param_1, 0, 0x9a);
    case 7:
    case (u32)-2:
        PlaySE(0x5dc);
        return ov14_021F23F0(param_1, 1, 0x9a);
    case (u32)-3:
        PlaySE(0x5dc);
        return ov14_021F0244(param_1, 0x18);
    case (u32)-4:
        PlaySE(0x5dc);
        break;
    }
    return 0x16;
}
