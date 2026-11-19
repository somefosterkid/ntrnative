#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EF388(PokedexAppData *param_1, s32 param_2, u32 param_3);

void ov18_021EF388(PokedexAppData *param_1, s32 param_2, u32 param_3) {
    u8 bVar1;
    void *ptr;
    Window *window;
    s32 destX;
    void *pvVar4;
    u32 uStack_24;
    NNSG2dCharacterData *apNStack_1c;

    window = &param_1->windows[param_2];
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(param_1->gfxNarc, 4, 1, &apNStack_1c, HEAP_ID_POKEDEX_APP);
    destX = 0;
    pvVar4 = apNStack_1c->pRawData;
    uStack_24 = 0;
    do {
        BlitBitmapRect(window, (void *)(pvVar4 + 0xc40), 0, 0, 8, 8, destX, 0, 8, 8, 0xff);
        BlitBitmapRect(window, (void *)((u8 *)pvVar4 + 0xca0), 0, 0, 8, 8, destX, 8, 8, 8, 0xff);
        destX = destX + 8;
        uStack_24 = uStack_24 + 1;
    } while (uStack_24 < 8);
    Heap_Free(ptr);
    bVar1 = GetWindowWidth(window);
    ov18_021F9648(window, param_1->msgData, param_3, (bVar1 * 8) / 2, 0, 4, 0x20100, 2);
    CopyWindowPixelsToVram_TextMode(window);
    return;
}
