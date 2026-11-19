#include "global.h"
struct BattleCastleOpponentApp;

#include "frontier/overlay_80_022340E8.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_0224465C(struct BattleCastleOpponentApp *arg0, enum GFBgLayer arg1);

void ov83_0224465C(BattleCastleOpponentApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x30, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    if (ov80_02237D8C(arg0->challengeType) == 0) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x2C, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
        return;
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x2D, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_022446D0(void);

void ov83_022446D0(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9D, &ppPlttData, HEAP_ID_107);
    DC_FlushRange(ppPlttData->pRawData, 0x80U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0x80U);
    Heap_Free(ptr);
}

void ov83_02244704(struct BattleCastleOpponentApp *arg0, enum GFBgLayer arg1);

void ov83_02244704(BattleCastleOpponentApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x30, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x2E, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
}

void ov83_0224474C(void);

void ov83_0224474C(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0x9D, &ppPlttData, HEAP_ID_107);
    DC_FlushRange(ppPlttData->pRawData, 0x80U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0x80U);
    Heap_Free(ptr);
}

void ov83_02244780(struct BattleCastleOpponentApp *arg0, enum GFBgLayer arg1);

void ov83_02244780(BattleCastleOpponentApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x28, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x93, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_107);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0xBE, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_107);
}
