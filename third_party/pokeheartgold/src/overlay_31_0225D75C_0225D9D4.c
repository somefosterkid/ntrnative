#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_31.h"

void ov31_0225D9D4(MartSubScreen *arg0, s32 arg1) {
    NNSG2dScreenData *ppScrnData;
    void *ptr;
    void *v0;
    void *v1;

    switch (arg1) {
    case 0:
        ptr = GfGfxLoader_GetScrnData(NARC_graphic_shop_gra, 0x11, 0, &ppScrnData, HEAP_ID_8);
        BG_LoadScreenTilemapData(arg0->bgConfig, 6U, ppScrnData->rawData, ppScrnData->szByte);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
        Heap_Free(ptr);
        GfGfx_EngineBTogglePlanes(2U, 1U);
        return;
    case 1:
        BgFillTilemapBufferAndSchedule(arg0->bgConfig, 5U, 0U);
        v0 = GfGfxLoader_GetScrnData(NARC_graphic_shop_gra, 0x13, 0, &ppScrnData, HEAP_ID_8);
        BG_LoadScreenTilemapData(arg0->bgConfig, 6U, ppScrnData->rawData, ppScrnData->szByte);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
        Heap_Free(v0);
        return;
    case 2:
        BgFillTilemapBufferAndSchedule(arg0->bgConfig, 5U, 0U);
        v1 = GfGfxLoader_GetScrnData(NARC_graphic_shop_gra, 0x14, 0, &ppScrnData, HEAP_ID_8);
        BG_LoadScreenTilemapData(arg0->bgConfig, 6U, ppScrnData->rawData, ppScrnData->szByte);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 6U);
        Heap_Free(v1);
        return;
    }
}
