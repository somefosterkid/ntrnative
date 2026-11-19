#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F9648(Window *, MsgData *, u32, u32, u32, u32, u32, u32);

void ov18_021EE44C(void *arg0, s32 arg1, s32 arg2);

void ov18_021EE44C(void *arg0, s32 arg1, s32 arg2) {
    NNSG2dCharacterData *ppCharData;
    void *ptr;
    u32 v2;
    s32 v1;
    void *v0;
    int v4;
    u32 destX;
    unsigned int v3;
    void *src;
    v0 = arg0;
    v1 = arg2;
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(((PokedexAppData *)arg0)->gfxNarc, 1, 1, &ppCharData, HEAP_ID_POKEDEX_APP);
    v4 = arg1 * sizeof(Window);
    destX = 0;
    v2 = 0;
    arg0 = v0 + offsetof(PokedexAppData, windows);
    src = ppCharData->pRawData;
    src = src + 0x20;
    do {
        BlitBitmapRect(arg0 + v4, src, 0U, 0U, 8U, 8U, (u16)((s32)destX), 0U, 8U, 8U, 0xFFU);
        BlitBitmapRect(arg0 + v4, src, 0U, 0U, 8U, 8U, (u16)((s32)destX), 8U, 8U, 8U, 0xFFU);
        destX += 8;
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 0xCU);
    ov18_021F9648(arg0 + v4, ((PokedexAppData *)v0)->msgData, v1, 0x60, 0, 0, 0xF0800, 1);
    Heap_Free(ptr);
}
