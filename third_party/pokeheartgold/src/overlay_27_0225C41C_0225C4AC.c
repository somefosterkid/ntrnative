#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_27.h"

void ov27_0225C4AC(Ov27TaskData *data) {
    NNSG2dScreenData *ppScrnData;
    NARC *narc;
    void *ptr;

    narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);
    reg_G2S_DB_BLDCNT = 0;
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0xA0U, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, data->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, HEAP_ID_8);
    ptr = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, 0, &ppScrnData, HEAP_ID_8);
    BG_LoadScreenTilemapData(data->bgConfig, 6U, ppScrnData->rawData, ppScrnData->szByte);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, 6U);
    Heap_Free(ptr);
    NARC_Delete(narc);
}


void ov27_0225C540(Ov27TaskData *data) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xA, data->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, HEAP_ID_8);
    NARC_Delete(narc);
    AddWindowParameterized(data->bgConfig, &data->windowA, 5U, 0xCU, 8U, 8U, 2U, 4U, 0x80U);
    AddWindowParameterized(data->bgConfig, &data->windowB, 5U, 0xCU, 0xEU, 8U, 2U, 4U, 0x90U);
    ov27_0225C8D0(&data->windowA, data->msgData, 0x2E);
    ov27_0225C8D0(&data->windowB, data->msgData, 0x2F);
}

void ov27_0225C5E4(Ov27TaskData *data) {
    BgClearTilemapBufferAndCommit(data->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(data->bgConfig, 5U);
    if (data->windowA.pixelBuffer != NULL) {
        RemoveWindow(&data->windowA);
    }
    if (data->windowB.pixelBuffer != NULL) {
        RemoveWindow(&data->windowB);
    }
}
