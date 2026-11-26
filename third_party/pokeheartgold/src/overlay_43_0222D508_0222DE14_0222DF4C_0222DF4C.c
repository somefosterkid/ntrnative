#include "global.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222DF90(PalPadDetailsSlot *, PalPadGfx *);

extern void (*_0222F0E0[3])(PalPadDetailsSlot *, PalPadShared *, PalPadGfx *, u32);

extern void (*ov43_0222F12C[8])(PalPadDetailsSlot *, PalPadShared *, PalPadGfx *, u32);

void ov43_0222DF4C(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, s32 page, PalPadDetailsScreen *screens, BOOL hasFrontierPages, u32 heapID) {
    ov43_0222DF90(slot, gfx);
    if (hasFrontierPages != 0) {
        ov43_0222F12C[page](slot, shared, gfx, heapID);
        return;
    }
    _0222F0E0[page](slot, shared, gfx, heapID);
    return;
}
