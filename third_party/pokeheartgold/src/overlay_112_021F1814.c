#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sound.h"
#include "overlay_112_sprite_app.h"

extern u8 ov112_021FF4C0;

void ov112_021F1814(Ov112SpriteApp *arg0);

extern s32 ov112_021FF9B8;
void ov112_021F194C(s32 arg0, u32 arg1);

void ov112_021F1814(Ov112SpriteApp *arg0) {
    s32 v0;
    u8 *v1;

    v1 = &ov112_021FF4C0;
    v0 = 0;
    do {
        FreeBgTilemapBuffer(arg0->bgConfig, *v1);
        v0 += 1;
        v1 += 1;
    } while (v0 < 5);
    Heap_Free(arg0->bgConfig);
}

void ov112_021F1838(Ov112SpriteApp *arg0);

void ov112_021F1838(Ov112SpriteApp *arg0) {
    if (arg0->bgConfig == NULL) {
        GF_AssertFail();
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 1, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 3, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 9, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0xA, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0xB, arg0->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 1, arg0->heapID);
}

void ov112_021F1904(Ov112SpriteApp *arg0, u32 arg1, s32 arg2);

void ov112_021F1904(Ov112SpriteApp *arg0, u32 arg1, s32 arg2) {
    u8 v0;

    if ((arg1 >= 6U) && (arg1 < 0x10U)) {
        v0 = 0;
    } else if ((arg1 >= 0x10U) && (arg1 < 0x13U)) {
        v0 = 1;
    } else {
        v0 = 2;
    }
    if (arg2 == 5) {
        v0 += 3;
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, v0 + 3, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 1, arg0->heapID);
}

void ov112_021F194C(s32 arg0, u32 arg1) {
    if (arg1 >= 8U) {
        GF_AssertFail();
    }
    PlayBGM((u16) * (s32 *)(&ov112_021FF9B8 + arg1));
}
