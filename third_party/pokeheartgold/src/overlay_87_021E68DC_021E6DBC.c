#include "global.h"
struct ScratchOffCardApp;

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E6DBC(struct ScratchOffCardApp *arg0, enum GFBgLayer arg1);

void ov87_021E6DBC(ScratchOffCardApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x23, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x25, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
}

void ov87_021E6E00(struct ScratchOffCardApp *arg0, enum GFBgLayer arg1);

void ov87_021E6E00(ScratchOffCardApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x26, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x28, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
}

void ov87_021E6E44(void);

void ov87_021E6E44(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_2_0_5, 0x22, &ppPlttData, HEAP_ID_122);
    DC_FlushRange(ppPlttData->pRawData, 0xA0U);
    GXS_LoadBGPltt(ppPlttData->pRawData, 0U, 0xA0U);
    Heap_Free(ptr);
}

void ov87_021E6E78(void);

void ov87_021E6E78(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_2_0_5, 0, &ppPlttData, HEAP_ID_122);
    DC_FlushRange(ppPlttData->pRawData, 0x180U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0x180U);
    Heap_Free(ptr);
}

void ov87_021E6EB0(struct ScratchOffCardApp *arg0, enum GFBgLayer arg1);

void ov87_021E6EB0(ScratchOffCardApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 4, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 6, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
}

void ov87_021E6EF4(struct ScratchOffCardApp *arg0, enum GFBgLayer arg1);

void ov87_021E6EF4(ScratchOffCardApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 7, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 9, arg0->bgConfig, layer, 0U, 0U, 0, HEAP_ID_122);
}
