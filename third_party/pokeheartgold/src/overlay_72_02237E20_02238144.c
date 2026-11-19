#include "global.h"

#include "overlay_manager.h"
#include "overlay_72_wfc_connect.h"

s32 ov72_022387D0(NintendoWfcConnect *, s32, s32);

void ov72_02238144(NintendoWfcConnect *arg0, OverlayManager *arg1);

void ov72_02238144(NintendoWfcConnect *arg0, OverlayManager *man) {
    arg0->args = OverlayManager_GetArgs(man);
    arg0->screen = 0;
    ov72_022387D0(arg0, 0, 0);
}

void ov72_02238160(void);

void ov72_02238160(void) {
}
