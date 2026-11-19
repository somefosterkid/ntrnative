#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96hurdle_hud.h"

void ov96_022082BC(Ov96HurdleHud *hud, u32 param_2, u32 param_3);

void ov96_022082BC(Ov96HurdleHud *hud, u32 param_2, u32 param_3) {
    if (param_2 >= 2) {
        GF_AssertFail();
    }
    if (param_3 >= 3) {
        GF_AssertFail();
    }
    DC_FlushRange(hud->picRes[param_3].charBuf, 0xc80);
    GXS_LoadOBJ(hud->picRes[param_3].charBuf, hud->charLoc[param_2], 0xc80);
    DC_FlushRange(hud->picRes[param_3].palBuf, 0x20);
    GXS_LoadOBJPltt(hud->picRes[param_3].palBuf, hud->palLoc[param_2], 0x20);
    return;
}
