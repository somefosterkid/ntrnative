#include "global.h"
struct BattleHallApp;
#include "gf_gfx_loader.h"
#include "heap.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "overlay_82.h"
#include "overlay_82_battle_hall_app.h"

s32 ov80_0223792C(u8);

void ov82_0223ED94(struct BattleHallApp *arg0);

void ov82_0223EDF0(struct BattleHallApp *arg0, enum GFBgLayer arg1);
void ov82_0223EE38(void);
void ov82_0223EE6C(struct BattleHallApp *arg0, enum GFBgLayer arg1);

void ov82_0223ED94(BattleHallApp *arg0) {
    s32 new_var;
    if (ov80_0223792C(arg0->challengeType) != 0) {
        reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFF1FFF);
        reg_G2_WININ = (u16)((reg_G2_WININ & (~0x3F)) | 0x1F);
        new_var = 0x1F | (reg_G2_WINOUT & (~0x3F));
        reg_G2_WINOUT = (u16)(0x20 | new_var);
        reg_G2_WIN0H = 0xF000;
        reg_G2_WIN0V = 0x10;
        reg_GX_DISPCNT = (s32)(0x2000 | (reg_GX_DISPCNT & 0xFFFF1FFF));
    }
}

void ov82_0223EDF0(BattleHallApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x17, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x18, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_105);
}

void ov82_0223EE38(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x99, &ppPlttData, HEAP_ID_105);
    DC_FlushRange(ppPlttData->pRawData, 0xC0U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0xC0U);
    Heap_Free(ptr);
}

void ov82_0223EE6C(BattleHallApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x81, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_105);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x82, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_105);
}

void ov82_0223EECC(s32 param_1, GFBgLayer param_2) {
    BG_ClearCharDataRange((u8)param_2, 0x20, 0, HEAP_ID_105);
    GfGfxLoader_LoadCharDataFromOpenNarc(*(NARC **)(param_1 + 0x220), 0x85, *(BgConfig **)(param_1 + 0x48), param_2, 1, 0, 1, HEAP_ID_105);
    GfGfxLoader_GXLoadPalFromOpenNarc(*(NARC **)(param_1 + 0x220), 0xc0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, 0x20, HEAP_ID_105);
    return;
}
