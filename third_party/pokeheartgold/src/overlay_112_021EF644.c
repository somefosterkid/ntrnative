#include "global.h"

#include "system.h"
#include "overlay_manager.h"

s32 ov112_021EF644(void *arg0);

struct OverlayManager;
BOOL ov112_021EF660(OverlayManager *appMan, int *state);

s32 ov112_021E9A78(u8 *, u8);
s32 ov112_021E9C10(u8 *, s32);
s32 ov112_021EF664(void *arg0);

s32 ov112_021EF644(void *arg0) {
    if (0x80 & gSystem.newKeys) {
        *(s32 *)((u8 *)arg0 + 4) = 3;
        return 3;
    }
    return 2;
}

BOOL ov112_021EF660(OverlayManager *appMan, int *state) {
    return 1;
}

s32 ov112_021EF664(void *arg0) {
    ov112_021E9C10(arg0, 1);
    ov112_021E9A78(arg0, 4);
    return 2;
}
