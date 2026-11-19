#include "global.h"

#include "overlay_manager.h"

struct OverlayManager;

BOOL ov96_021EEEBC(OverlayManager *appMan, int *state);

s32 ov96_021EF05C();
s32 ov96_021EEEC0(void);

BOOL ov96_021EEEBC(OverlayManager *appMan, int *state) {
    return TRUE;
}

s32 ov96_021EEEC0(void) {
    ov96_021EF05C();
    return 1;
}
