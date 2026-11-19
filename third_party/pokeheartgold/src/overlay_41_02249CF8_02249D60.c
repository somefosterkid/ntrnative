#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"

typedef struct UnkStruct_ov41_02249D60_v1 {
    void *unk_0;
    u8 unk_4[0x8];
    s32 unk_C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
    u8 unk_24[0x4];
    s32 unk_28;
} UnkStruct_ov41_02249D60_v1;

typedef struct UnkStruct_ov41_02249D60_v0 {
    u8 unk_0[0x4];
    enum NarcId unk_4;
    s32 unk_8;
} UnkStruct_ov41_02249D60_v0;

s32 ov41_02249F7C(BgConfig *, s32, NNSG2dScreenData *, s32, s32, s32, s32, s32, s32);

void ov41_02249D60(s8 *arg0);

void ov41_02249D60(s8 *arg0) {
    NNSG2dScreenData *ppScrnData;
    u32 v2;
    s8 *v3;
    UnkStruct_ov41_02249D60_v0 *v0;
    UnkStruct_ov41_02249D60_v1 *v1;
    void *ptr;

    v3 = arg0;
    v0 = *(void **)v3;
    ptr = GfGfxLoader_GetScrnData(v0->unk_4, v0->unk_8, 0, &ppScrnData, *(enum HeapID *)((u8 *)v3 + 0x14));
    v1 = *(void **)v3;
    ov41_02249F7C(v1->unk_0, v1->unk_1C, ppScrnData, v1->unk_14, v1->unk_18, v1->unk_C, v1->unk_10, v1->unk_28, v1->unk_20);
    Heap_Free(ptr);
    v2 = 0x18;
    do {
        *v3 = 0;
        v3 += 1;
        v2 -= 1;
    } while (v2 != 0);
}
