#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_printer.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021EE44C(PokedexAppData *param_1, s32 param_2, u32 param_3);

void ov18_021EE508(PokedexAppData *param_1, s32 param_2, u32 param_3);

void ov18_021EE520(PokedexAppData *arg0, s32 arg1, u32 arg2);

void ov18_021EE508(PokedexAppData *param_1, s32 param_2, u32 param_3) {
    ov18_021EE44C(param_1, param_2, param_3);
    ScheduleWindowCopyToVram(&param_1->windows[param_2]);
    return;
}

void ov18_021EE520(PokedexAppData *arg0, s32 arg1, u32 arg2) {
    u32 destX;
    void *src;
    void *ptr;
    u32 num;
    s32 v0;
    MessagePrinter *messagePrinter;
    Window *window;
    NNSG2dCharacterData *ppCharData;
    u32 v2;
    void *v1;

    v0 = arg1;
    num = arg2;
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(arg0->gfxNarc, 1, 1, &ppCharData, HEAP_ID_POKEDEX_APP);
    v2 = 0;
    v1 = ppCharData->pRawData;
    src = v1;
    destX = 0;
    window = &arg0->windows[v0];
    src = v1 + 0x20;
    do {
        BlitBitmapRect(window, v1 + 0x300, 0U, 0U, 8U, 8U, (u16)(s32)destX, 0U, 8U, 8U, 0xFFU);
        BlitBitmapRect(window, src, 0U, 0U, 8U, 8U, (u16)(s32)destX, 8U, 8U, 8U, 0xFFU);
        v2 += 1;
        destX += 8;
    } while (v2 < 3U);
    Heap_Free(ptr);
    messagePrinter = MessagePrinter_New(0xFU, 8U, 7U, HEAP_ID_POKEDEX_APP);
    PrintUIntOnWindow(messagePrinter, num, 3U, PRINTING_MODE_LEADING_ZEROS, window, 0U, 4U);
    MessagePrinter_Delete(messagePrinter);
}
