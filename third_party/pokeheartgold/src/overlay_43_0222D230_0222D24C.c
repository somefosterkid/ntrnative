#include "global.h"

#include "unk_02005D10.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222AD40(PalPadGfx *, u32, u32);

s32 ov43_0222D230(PalPadDetails *, s32);

u32 ov43_0222D24C(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 action) {
    s32 iVar1;

    switch (action) {
    case 0:
        iVar1 = ov43_0222D230(details, details->page);
        if (iVar1 != 0) {
            PlaySE(0x5dd);
            return 6;
        }
        break;
    case 1:
        PlaySE(0x5dd);
        return 6;
    case 2:
        ov43_0222DB28(details, gfx, details->page, action);
        ov43_0222DB94(details, gfx, details->page);
        details->direction = 2;
        return 3;
    case 3:
        ov43_0222DB28(details, gfx, details->page, action);
        ov43_0222DB94(details, gfx, details->page);
        details->direction = 3;
        return 3;
    case 4:
        details->direction = 0;
        details->friendChangeDelay = 4;
        ov43_0222AD40(gfx, 2, 1);
        return 5;
    case 5:
        details->direction = 1;
        details->friendChangeDelay = 4;
        ov43_0222AD40(gfx, 3, 1);
        return 5;
    }
    return 2;
}
