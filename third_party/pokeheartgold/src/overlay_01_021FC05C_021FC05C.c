#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_01_021FC05C.h"
#include "unk_02005D10.h"

void ov01_021FC14C(BgConfig *);

void ov01_021FC1A4(void *, u32, u32, u32);

void ov01_021FC05C(u32 param_1, s32 param_2, u8 *param_3) {
    ov01_021FC14C(*(void **)(param_2 + 8));
    ov01_021FC1A4(param_3, 0, 10, 0x13);
    *(u32 *)(param_3 + 0x14) = 0;
    G2_SetBlendAlpha(4, 0x29, 0, 0x10);
    SetBgPriority(2, 0);
    GfGfx_EngineATogglePlanes(4, 1);
    PlaySE(0x647);
    return;
}
