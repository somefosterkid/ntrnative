#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_31.h"
#include "pokeathlon/pokeathlon_save.h"



void ov31_0225E0E4(MartSubScreen *arg0, s32 arg1) {
    NNSG2dScreenData *ppScrnData;
    void *ptr;

    ptr = GfGfxLoader_GetScrnData(NARC_graphic_shop_gra, 0x12, 0, &ppScrnData, HEAP_ID_8);
    BG_LoadScreenTilemapData(arg0->bgConfig, 5U, ppScrnData->rawData, ppScrnData->szByte);
    ov31_0225E060(arg0->bgConfig, 5, arg1);
    ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 5U);
    Heap_Free(ptr);
}

s32 ov31_0225E12C(MartData *arg0, s32 idx, s32 arg2) {
    u8 v0;

    v0 = arg0->martType;
    if (v0 == 3) {
        if (PokeathlonSave_CheckReceivedDailyItemSlot(arg0->pokeathlonSave, idx) == 0) {
            return 1;
        }
        return 0;
    }
    if (v0 == 4) {
        if (PokeathlonSave_CheckReceivedDataCard(arg0->pokeathlonSave, idx + (6 * ((s32)(arg2 - 0x1F9) / 6))) == 0) {
            return 1;
        }
        return 0;
    }
    return 1;
}
