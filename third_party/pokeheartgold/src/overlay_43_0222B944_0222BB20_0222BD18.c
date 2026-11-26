#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_palpad.h"

s32 ov43_0222AD00(PalPadGfx *, s32);

s32 ov43_0222BC78(PalPadRoster *, PalPadShared *, PalPadGfx *, s32);

void ov43_0222C844(PalPadRosterPage *, PalPadGfx *, s32);

s32 ov43_0222BD18(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx);

s32 ov43_0222BD18(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx) {
    s32 sp8;
    s32 sp4;
    s32 sp0;
    s16 temp_r0_3;
    s16 temp_r1;
    s32 var_r4;
    u8 temp_r0;

    if (0xCF3 & gSystem.newKeys) {
        shared->inputState = 0;
    }
    if (1 & gSystem.newKeys) {
        return ov43_0222BC78(roster, shared, gfx, 5);
    }
    if (gSystem.newKeys & 2) {
        roster->cursor = 8;
        ov43_0222C844(roster->pages, gfx, roster->cursor);
        return ov43_0222BC78(roster, shared, gfx, 0);
    }
    temp_r1 = roster->cursor;
    var_r4 = 0;
    sp8 = (s32)(u16)temp_r1;
    sp4 = 0;
    if (0x40 & gSystem.newAndRepeatedKeys) {
        if (temp_r1 == 8) {
            var_r4 = 1;
            roster->cursor = roster->prevCursor;
            ov43_0222AD00(gfx, 1);
        } else if ((s32)temp_r1 < 4) {
            if ((s32)temp_r1 > 0) {
                roster->cursor = (s16)(temp_r1 - 1);
                var_r4 = 1;
            }
        } else if ((s32)temp_r1 > 4) {
            roster->cursor = (s16)(temp_r1 - 1);
            var_r4 = 1;
        }
    } else if (0x80 & gSystem.newAndRepeatedKeys) {
        if ((temp_r1 == 3) || (temp_r1 == 7)) {
            roster->cursor = 8;
            var_r4 = 1;
        } else if ((s32)temp_r1 < 4) {
            if ((s32)temp_r1 < 3) {
                roster->cursor = (s16)(temp_r1 + 1);
                var_r4 = 1;
            }
        } else if ((s32)temp_r1 < 7) {
            roster->cursor = (s16)(temp_r1 + 1);
            var_r4 = 1;
        }
    } else if (0x20 & gSystem.newAndRepeatedKeys) {
        if (temp_r1 != 8) {
            if ((s32)temp_r1 < 4) {
                var_r4 = 1;
                roster->cursor = (s16)(temp_r1 + 4);
                sp4 = 1;
                sp0 = 0;
            } else {
                roster->cursor = (s16)(temp_r1 - 4);
                var_r4 = 1;
            }
        }
    } else if (gSystem.newAndRepeatedKeys & 0x10) {
        if (temp_r1 != 8) {
            if ((s32)temp_r1 < 4) {
                roster->cursor = (s16)(temp_r1 + 4);
                var_r4 = 1;
            } else {
                var_r4 = 1;
                roster->cursor = (s16)(temp_r1 - 4);
                sp4 = 1;
                sp0 = 1;
            }
        }
    } else {
        temp_r0 = shared->lrPaging;
        if (temp_r0 != 0) {
            if ((temp_r0 != 0) && (0x200 & gSystem.newAndRepeatedKeys)) {
                sp4 = 1;
                sp0 = 0;
            } else if ((temp_r0 != 0) && (0x100 & gSystem.newAndRepeatedKeys)) {
                sp4 = 1;
                sp0 = 1;
            }
        }
    }
    if (sp4 == 1) {
        if (sp0 != 0) {
            roster->prevPage = roster->page;
            roster->page = (s16)((roster->page + 1) % 4);
            return ov43_0222BC78(roster, shared, gfx, 2);
        }
        roster->prevPage = roster->page;
        roster->page = (s16)(roster->page - 1);
        temp_r0_3 = roster->page;
        if ((s32)temp_r0_3 < 0) {
            roster->page = (s16)(temp_r0_3 + 4);
        }
        return ov43_0222BC78(roster, shared, gfx, 1);
    }
    if (var_r4 == 1) {
        PlaySE(0x5E5U);
        ov43_0222C844(roster->pages, gfx, roster->cursor);
        roster->prevCursor = (s16)sp8;
    }
    return 0;
}
