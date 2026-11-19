#include "global.h"
#include "gf_gfx_loader.h"
#include "heap.h"

extern const u32 ov18_021FBE10[8];

u32 ov18_021F967C(u32 index);

enum NarcId ov18_021E5900(void);
s32 ov18_021E5904(s32);
void *ov18_021F9694(s32 arg0, enum HeapID arg1);

u32 ov18_021F967C(u32 index) {
    return ov18_021FBE10[index];
}

extern const u8 ov18_021FBDFC[9];

u8 ov18_021F9688(u8 spot);

u8 ov18_021F9688(u8 spot) {
    return ov18_021FBDFC[spot];
}

void *ov18_021F9694(s32 arg0, enum HeapID heapID) {
    NNSG2dCharacterData *ppCharData;
    enum NarcId narcId;
    void *v0;
    void *v1;
    void *ptr;

    narcId = ov18_021E5900();
    ptr = GfGfxLoader_GetCharData(narcId, ov18_021E5904(arg0), 1, &ppCharData, heapID);
    v1 = ppCharData->pRawData;
    v0 = Heap_AllocAtEnd(heapID, 0x80U);
    memset(v0, 0, 0x80U);
    memcpy(v0, v1 + 0x80, 0x40U);
    memcpy(v0 + 0x40, v1, 0x40U);
    Heap_Free(ptr);
    return v0;
}
