#include "global.h"
struct BattleCastleSelfApp;

#include "frontier/overlay_80_022340E8.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0223FA00(struct BattleCastleSelfApp *arg0, enum GFBgLayer arg1);

void ov83_0223FA00(BattleCastleSelfApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x22, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x23, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
        return;
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x24, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_0223FA74(void);

void ov83_0223FA74(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9C, &ppPlttData, HEAP_ID_107);
    DC_FlushRange(ppPlttData->pRawData, 0xE0U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0xE0U);
    Heap_Free(ptr);
}

void ov83_0223FAA8(struct BattleCastleSelfApp *arg0, enum GFBgLayer arg1);

void ov83_0223FAA8(BattleCastleSelfApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x22, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x2A, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_0223FAF0(void);

void ov83_0223FAF0(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9C, &ppPlttData, HEAP_ID_107);
    DC_FlushRange(ppPlttData->pRawData, 0x80U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0x80U);
    Heap_Free(ptr);
}

void ov83_0223FB24(struct BattleCastleSelfApp *arg0, enum GFBgLayer arg1);

void ov83_0223FB24(BattleCastleSelfApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x22, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x2B, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_0223FB6C(struct BattleCastleSelfApp *arg0);

void ov83_0223FB6C(BattleCastleSelfApp *arg0) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x22, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x26, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x28, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x29, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_0223FBEC(struct BattleCastleSelfApp *arg0, enum GFBgLayer arg1);

void ov83_0223FBEC(BattleCastleSelfApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x28, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x93, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_107);
}
