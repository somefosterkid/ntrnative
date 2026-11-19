#include "global.h"
struct BattleFactoryApp;

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02240698(struct BattleFactoryApp *arg0, enum GFBgLayer arg1);

void ov81_02240698(BattleFactoryApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 0x81, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 0x82, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
}

void ov81_022406E0(BattleFactoryApp *app, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 0x92, app->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 0x91, app->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
}

void ov81_02240728(BattleFactoryApp *app, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(app->narc, 4, app->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(app->narc, 8, app->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
}

void ov81_02240770(void);

void ov81_02240770(void) {
    NNSG2dPaletteData *ppPlttData;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 0xC1, &ppPlttData, HEAP_ID_100);
    DC_FlushRange(ppPlttData->pRawData, 0x160U);
    GX_LoadBGPltt(ppPlttData->pRawData, 0U, 0x160U);
    Heap_Free(ptr);
}

void ov81_022407A8(struct BattleFactoryApp *arg0, enum GFBgLayer arg1);

void ov81_022407A8(BattleFactoryApp *arg0, enum GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(arg0->narc, 4, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg0->narc, 7, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg0->narc, 0x95, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x160U, HEAP_ID_100);
}
