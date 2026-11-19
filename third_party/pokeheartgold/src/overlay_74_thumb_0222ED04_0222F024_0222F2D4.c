#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_74_wonder_cards_app2_data.h"

void ov74_0222F2D4(WonderCardsApp2Data *appData, u32 narcMemberIdx, u32 bgLayer, u32 size);

void ov74_0222F2D4(WonderCardsApp2Data *appData, u32 narcMemberIdx, u32 bgLayer, u32 size) {
    NNSG2dScreenData *screenData;
    void *nscr = GfGfxLoader_LoadFromNarc(NARC_a_1_1_3, narcMemberIdx, TRUE, appData->heapID, TRUE);

    NNS_G2dGetUnpackedScreenData(nscr, &screenData);
    BG_LoadScreenTilemapData(appData->bgConfig, bgLayer, screenData->rawData, size);
    Heap_Free(nscr);
}
