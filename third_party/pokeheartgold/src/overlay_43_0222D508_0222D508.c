#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad.h"

void ov43_0222AD20(PalPadGfx *, u32);

void ov43_0222DAE8(u16 *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5, u32 param_6);

void ov43_0222DB94(s16 *, PalPadGfx *, s16);

u32 ov43_0222D508(s16 *param_1, u32 param_2, PalPadGfx *param_3, u32 param_4);

u32 ov43_0222D508(s16 *param_1, u32 param_2, PalPadGfx *param_3, u32 param_4) {
    s32 iVar1;

    if (4 < *(s32 *)(param_1 + 2)) {
        ScheduleSetBgPosText(param_3->bgConfig, 2, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(param_3->bgConfig, 3, BG_POS_OP_SET_ROT, 0);
        ov43_0222DAE8(param_1, (u32)(param_2), (u32)(param_3), 0, (u32)((s32)*param_1), (u32)(param_4));
        ov43_0222AD20(param_3, 1);
        ov43_0222DB94(param_1, param_3, 0xff);
        return 1;
    }
    iVar1 = *(s32 *)(param_1 + 2) * 0x100;
    iVar1 = (s32)(iVar1 + ((u32)(iVar1 >> 1) >> 0x1e)) >> 2;
    if (*(s32 *)(param_1 + 4) == 2) {
        iVar1 = -iVar1;
    }
    ScheduleSetBgPosText(param_3->bgConfig, 3, BG_POS_OP_SET_ROT, iVar1);
    *(s32 *)(param_1 + 2) = *(s32 *)(param_1 + 2) + 1;
    return 0;
}
