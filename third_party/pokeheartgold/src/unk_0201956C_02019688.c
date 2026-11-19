#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_0201956C.h"
#include "constants/heap.h"
#include "filesystem.h"

void sub_02019688(UnkStruct_0201956C *arg0, s32 arg1, enum NarcId arg2, s32 arg3, s32 arg4);

void sub_02019688(UnkStruct_0201956C *a0, s32 a1, enum NarcId narcId, s32 memberNo, s32 isCompressed) {
    NNSG2dScreenData *ppScrnData;
    void *ptr;

    ptr = GfGfxLoader_GetScrnData(narcId, memberNo, isCompressed, &ppScrnData, a0->heapId);
    sub_02019668(a0, a1, (u16 *)ppScrnData->rawData);
    Heap_Free(ptr);
}

void sub_020196B8(UnkStruct_0201956C *a0, int a1, NARC *narc, int fileId, int a4) {
    NNSG2dScreenData *ppScrnData;
    void *ptr;

    ptr = GfGfxLoader_GetScrnDataFromOpenNarc(narc, fileId, a4, &ppScrnData, a0->heapId);
    sub_02019668(a0, a1, (u16 *)ppScrnData->rawData);
    Heap_Free(ptr);
}
