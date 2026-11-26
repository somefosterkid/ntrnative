#include "global.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222DAE8(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, u32 slot, s32 page, u32 heapID) {
    if (slot == 0) {
        details->page = (s16)page;
        ov43_0222DB28(details, gfx, page, slot);
    }
    ov43_0222DF4C(&details->slots[slot], shared, gfx, page, &details->screens[1], details->hasFrontierPages, heapID);
}
