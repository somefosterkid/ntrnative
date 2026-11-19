#include "global.h"

#include "system.h"
#include "overlay_manager.h"

s32 ov112_021EE608(void *arg0);

struct OverlayManager;
BOOL ov112_021EE624(OverlayManager *appMan, int *state);

s32 ov112_021EE608(void *arg0) {
    if (0x80 & gSystem.newKeys) {
        *(s32 *)((u8 *)arg0 + 4) = 6;
        return 3;
    }
    return 2;
}

BOOL ov112_021EE624(OverlayManager *appMan, int *state) {
    return 1;
}
