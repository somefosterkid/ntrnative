#include "global.h"

#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_43_palpad.h"

s32 ov43_0222BC78(PalPadRoster *, PalPadShared *, PalPadGfx *, s32);

void ov43_0222C844(PalPadRosterPage *, PalPadGfx *, s32);

extern TouchscreenHitbox ov43_0222EF60;

s32 ov43_0222BEEC(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s32 *arg3);

s32 ov43_0222BEEC(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, s32 *arg3) {
    s32 temp_r0;
    s8 var_r1;

    temp_r0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov43_0222EF60);
    var_r1 = -1;
    if (temp_r0 == -1) {
        return 0;
    }
    if ((u32)temp_r0 <= 8U) {
        *arg3 = 1;
        roster->cursor = temp_r0;
        PlaySE(0x5E5U);
        ov43_0222C844(roster->pages, gfx, roster->cursor);
        return ov43_0222BC78(roster, shared, gfx, 5);
    }
    if (temp_r0 != 9) {
        if (temp_r0 == 0xA) {
            var_r1 = 1;
        } else {
            var_r1 = (temp_r0 - 0xB) - roster->page;
        }
    }
    if (var_r1 == 0) {
        return 0;
    }
    *arg3 = 1;
    roster->prevPage = roster->page;
    roster->page = (s16)(roster->page + var_r1);
    if ((s32)var_r1 >= 0) {
        roster->page = (s16)(roster->page % 4);
        return ov43_0222BC78(roster, shared, gfx, 2);
    }
    if ((s32)(roster->page) < 0) {
        roster->page = (s16)((roster->page) + 4);
    }
    return ov43_0222BC78(roster, shared, gfx, 1);
}
