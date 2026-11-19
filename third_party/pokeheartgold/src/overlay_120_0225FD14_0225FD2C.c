#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_120.h"

void ov120_0225FD2C(BgConfig *bgConfig, NARC *narc, s32 fileId, u16 bgId) {
    NNSG2dScreenData *screenData;
    void *screenFile;

    screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, fileId, 0, &screenData, HEAP_ID_FIELD1);
    LoadRectToBgTilemapRect(bgConfig, bgId, screenData->rawData, 0, 0, (u8)((u32)screenData->screenWidth >> 3), (u8)((u32)screenData->screenHeight >> 3));
    BgTilemapRectChangePalette(bgConfig, bgId, 0, 0, (u8)((u32)screenData->screenWidth >> 3), (u8)((u32)screenData->screenHeight >> 3), 0);
    Heap_Free(screenFile);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}
