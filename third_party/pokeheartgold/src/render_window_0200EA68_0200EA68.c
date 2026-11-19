#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"

void sub_0200EA24(void *srcPixels, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *destPixels, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 blitWidth, u16 blitHeight);

void sub_0200EA68(Window *window, s32 fileId, u8 tileStart, s8 count, u8 x, u8 y);

void sub_0200EA68(Window *window, s32 fileId, u8 tileStart, s8 count, u8 x, u8 y) {
    u8 h;
    u8 w;
    u8 i;
    void *rawData;
    NNSG2dCharacterData *charData;
    void *ptr;
    u8 *charPtr;
    void *data;
    u32 size;
    u8 bgId;
    enum HeapID heapID;

    heapID = BgConfig_GetHeapId(window->bgConfig);
    bgId = GetWindowBgId(window);
    size = count * 0x80;
    data = Heap_Alloc(heapID, size);
    charPtr = BgGetCharPtr(bgId);
    ptr = GfGfxLoader_GetCharData(NARC_a_0_3_8, fileId, 0, &charData, heapID);
    rawData = charData->pRawData;
    i = 0;
    if (count > 0) {
        do {
            memcpy((u8 *)data + i * 0x80, charPtr + (tileStart + 10) * 0x20, 0x20);
            memcpy((u8 *)data + (i * 0x80 + 0x20), charPtr + (tileStart + 11) * 0x20, 0x20);
            memcpy((u8 *)data + (i * 0x80 + 0x40), charPtr + (tileStart + 10) * 0x20, 0x20);
            memcpy((u8 *)data + (i * 0x80 + 0x60), charPtr + (tileStart + 11) * 0x20, 0x20);
            i++;
        } while (i < count);
    }
    h = (16 - y) * count;
    w = 16 - x;
    sub_0200EA24(rawData, x, y, w, h, data, w, h, 0, 0, w, h);
    BG_LoadCharTilesData(window->bgConfig, bgId, data, size, tileStart + 0x12);
    Heap_Free(ptr);
    Heap_Free(data);
}
