#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"
#include "bg_window.h"

void ov106_021E69F0(ArceusCinematic *arceus, enum GFBgLayer layer, s32 palette);

s32 ov106_021E6B50(LegendaryCinematic *arg0, s32 *arg1) {
    ArceusCinematic *v0;

    v0 = arg0->arceus;
    if (arg1[0] != 6) {
        GF_AssertFail();
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(v0->narc, arg1[2] + 0x44, v0->bgConfig, (enum GFBgLayer)arg1[1], 0U, 0U, 1, HEAP_ID_153);
    ov106_021E69F0(v0, (enum GFBgLayer)arg1[1], arg1[2]);
    GfGfxLoader_LoadScrnDataFromOpenNarc(v0->narc, arg1[2], v0->bgConfig, (enum GFBgLayer)arg1[1], 0U, 0U, 1, HEAP_ID_153);
    return 1;
}

u32 ov106_021E6BB0(LegendaryCinematic *param_1, s32 *param_2) {
    if (*param_2 != 7) {
        GF_AssertFail();
    }
    ToggleBgLayer((u8)param_2[1], (u8)param_2[2]);
    return 1;
}
