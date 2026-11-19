#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "overlay_07_battle_anim_system.h"

s32 ov07_0221BFC0(void *);

s32 ov07_0221DDB0(void *, void *, s32);

s32 ov07_0221FB7C(s32, s32);

void ov07_0221DE04(void *arg0, void *arg1, enum GFBgLayer arg2, s32 arg3);

BOOL ov07_0221DEB0(int flags, int flag);

void ov07_0221DE04(void *arg0, void *arg1, enum GFBgLayer layer, s32 arg3) {
    s32 v0;
    s32 v1;

    v0 = arg3;
    GfGfxLoader_LoadCharData(NARC_a_0_0_7, ov07_0221FB7C(arg3, 0), ((BattleAnimSystem *)arg1)->bgConfig, layer, 0U, 0U, 1, *(enum HeapID *)arg1);
    PaletteData_LoadNarc(((BattleAnimSystem *)arg1)->paletteData, NARC_a_0_0_7, ov07_0221FB7C(v0, 1), *(enum HeapID *)arg1, PLTTBUF_MAIN_BG, 0x20U, 0x90U);
    BgClearTilemapBufferAndCommit(((BattleAnimSystem *)arg1)->bgConfig, (u8)layer);
    v1 = 2;
    if (ov07_0221BFC0(arg1) == 1) {
        v1 = 4;
    } else if (ov07_0221DDB0(arg0, arg1, 7) == 1) {
        v1 = 3;
    }
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, ov07_0221FB7C(v0, v1), ((BattleAnimSystem *)arg1)->bgConfig, layer, 0U, 0U, 1, *(enum HeapID *)arg1);
}

BOOL ov07_0221DEB0(int flags, int flag) {
    return ((flags << 16) & flag) == flag;
}
