#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

void ov106_021E5E5C(MotionBlur *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov106_021E6064(MotionBlur *capture) {
    ov106_021E5E5C(capture, 1, 0, 1);
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GfGfx_EngineATogglePlanes(0xEU, 1U);
}
