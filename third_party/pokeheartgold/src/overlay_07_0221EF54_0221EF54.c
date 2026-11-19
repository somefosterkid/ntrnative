#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "overlay_07_battle_anim_system.h"

s32 ov07_0221DDB0(void *, void *, s32);

void *ov07_0221DF1C(void *);

s32 ov07_0221FB7C(s32, s32);

void ov07_0221EF54(void *arg0);

void ov07_0221EF54(void *arg0) {
    s32 resIdx;
    s32 scrnIdx;
    void *data;

    data = ov07_0221DF1C(arg0);
    *((s32 **)&((BattleAnimSystem *)arg0)->scriptPtr) += 1;
    resIdx = *((s32 *)((BattleAnimSystem *)arg0)->scriptPtr);
    *((s32 **)&((BattleAnimSystem *)arg0)->scriptPtr) += 1;
    ToggleBgLayer(3, 0);
    SetBgControlParam(((BattleAnimSystem *)arg0)->bgConfig, 3, GF_BG_CNT_SET_COLOR_MODE, 0);
    GfGfxLoader_LoadCharData(NARC_a_0_0_7, ov07_0221FB7C(resIdx, 0), ((BattleAnimSystem *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, *(enum HeapID *)arg0);
    PaletteData_LoadNarc(((BattleAnimSystem *)arg0)->paletteData, NARC_a_0_0_7, ov07_0221FB7C(resIdx, 1), *(enum HeapID *)arg0, PLTTBUF_MAIN_BG, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(((BattleAnimSystem *)arg0)->bgConfig, 3);
    scrnIdx = 2;
    if (ov07_0221DDB0(data, arg0, 7) == 1) {
        scrnIdx = 3;
    }
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, ov07_0221FB7C(resIdx, scrnIdx), ((BattleAnimSystem *)arg0)->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 1, *(enum HeapID *)arg0);
    ToggleBgLayer(3, 1);
    Heap_Free(data);
}
