#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_86_frontier_records_app.h"
#include "text.h"

void ov86_021E5ECC(FrontierRecordsApp *arg0, enum GFBgLayer arg1);

void ov86_021E5FD8(Window *arg0, String *arg1, u32 arg2, u32 arg3, enum Font arg4, u32 arg5, s32 arg6);

void ov86_021E5ECC(FrontierRecordsApp *arg0, enum GFBgLayer layer) {
    NNSG2dScreenData *ppScrnData;
    NARC *narc;
    void *ptr;

    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8C, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60U, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8B, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_121);
    ptr = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0x8F, 1, &ppScrnData, HEAP_ID_121);
    MIi_CpuCopy16(ppScrnData->rawData, arg0->tilemapRects, 0x60U);
    Heap_Free(ptr);
    NARC_Delete(narc);
}

void ov86_021E5F58(FrontierRecordsApp *arg0, enum GFBgLayer arg1);

void ov86_021E5F58(FrontierRecordsApp *arg0, enum GFBgLayer layer) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8E, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8D, arg0->bgConfig, layer, 0U, 0U, 1, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20U, HEAP_ID_121);
    NARC_Delete(narc);
}

void ov86_021E5FBC(FrontierRecordsApp *arg0, u32 arg1, s32 arg2);

void ov86_021E5FBC(FrontierRecordsApp *arg0, u32 idx, s32 num) {
    BufferIntegerAsString(arg0->msgFmt, idx, num, 4U, PRINTING_MODE_RIGHT_ALIGN, 1);
}

void ov86_021E5FD8(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6) {
    u32 x;

    x = arg2;
    if (arg6 == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0U);
    } else if (arg6 == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0U) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFFU, color, NULL);
}
