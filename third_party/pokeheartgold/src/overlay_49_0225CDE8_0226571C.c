#include "global.h"

#include "heap.h"

void ov49_0226571C(void *arg0);

void ov49_0226571C(void *arg0) {
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        Heap_Free(*(void **)((u8 *)v1 + 0x10550));
        v0 += 1;
        v1 += 0x10;
    } while (v0 < 0xF);
}

s32 ov49_02258830(void *, void *, s32, s32);

void ov49_02265738(u8 *arg0, u8 *arg1, s32 arg2);

void ov49_02265738(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 v0;
    u8 *v1;

    v0 = 0;
    v1 = arg0 + 0x10640;
    do {
        ov49_02258830(v1, arg1, v0 + 0x90, arg2);
        v0 += 1;
        v1 += 4;
    } while (v0 < 0x27);
}

void ov49_02265760(void *arg0);

void ov49_02265760(void *arg0) {
    u32 texKey;
    u32 tex4x4Key;
    NNSG3dResTex *pTex;
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        pTex = NNS_G3dGetTex(*(NNSG3dResFileHeader **)((u8 *)v1 + 0x10640));
        NNS_G3dTexReleaseTexKey(pTex, &texKey, &tex4x4Key);
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(pTex));
        Heap_Free(*(NNSG3dResFileHeader **)((u8 *)v1 + 0x10640));
        v0 += 1;
        v1 += 4;
    } while (v0 < 0x27);
}
