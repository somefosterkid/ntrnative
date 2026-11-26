#include "global.h"

#include "bg_window.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222AD20(PalPadGfx *, u32);

BOOL ov43_0222D508(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 heapID) {
    s32 iVar1;

    if (4 < details->scrollStep) {
        ScheduleSetBgPosText(gfx->bgConfig, 2, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(gfx->bgConfig, 3, BG_POS_OP_SET_ROT, 0);
        ov43_0222DAE8(details, shared, gfx, 0, details->page, heapID);
        ov43_0222AD20(gfx, 1);
        ov43_0222DB94(details, gfx, 0xff);
        return 1;
    }
    iVar1 = details->scrollStep * 0x100;
    iVar1 = (s32)(iVar1 + ((u32)(iVar1 >> 1) >> 0x1e)) >> 2;
    if (details->direction == 2) {
        iVar1 = -iVar1;
    }
    ScheduleSetBgPosText(gfx->bgConfig, 3, BG_POS_OP_SET_ROT, iVar1);
    details->scrollStep = details->scrollStep + 1;
    return 0;
}
