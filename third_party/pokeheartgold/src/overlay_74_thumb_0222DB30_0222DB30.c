#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "gf_gfx_loader.h"

void ov74_0222DB30(WonderCardsAppData *appData, u32 narcMemberIdx, u32 bgLayer, u32 size);
void ov74_0222DB30(WonderCardsAppData *appData, u32 narcMemberIdx, u32 bgLayer, u32 size) {
    NNSG2dScreenData *screenData;
    void *nscr = GfGfxLoader_LoadFromNarc((NarcId)113, narcMemberIdx, TRUE, appData->heapID, TRUE);

    NNS_G2dGetUnpackedScreenData(nscr, &screenData);

    BG_LoadScreenTilemapData(appData->bgConfig, bgLayer, screenData->rawData, size);
    Heap_Free(nscr);
}
