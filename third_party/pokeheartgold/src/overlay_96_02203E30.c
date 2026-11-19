#include "global.h"

#include "constants/heap.h"

#include "pokemon.h"
#include "sprite.h"

Sprite *ov96_021EB408(enum HeapID *param_1, s32 param_2, NNS_G2D_VRAM_TYPE param_3, u8 param_4, u8 param_5);

s16 *ov96_021EB4F4(void *param_1, u32 param_2, u32 param_3);

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

void ov96_021EB564(s32 param_1, s32 param_2);

void ov96_021EB588(s32 param_1, VecFx32 *param_2);

void ov96_021EB630(s32 param_1, u32 param_2);

void ov96_02203DCC(enum HeapID *param_1, u16 *param_2);

void ov96_02203F0C(s32 param_1);

void ov96_02203F50(void *param_1, void *param_2);

void ov96_02203E30(enum HeapID *param_1, enum HeapID *param_2, u16 *param_3);

void ov96_02203E30(enum HeapID *param_1, enum HeapID *param_2, u16 *param_3) {
    u8 bVar1;
    s16 *psVar2;
    s32 iVar3;
    enum HeapID *pHVar4;
    u16 *puVar5;
    s32 iStack_24;
    VecFx32 VStack_20;
    iVar3 = 0;
    do {
        ov96_021EB408(param_1, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0x65, 2);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 3);
    iVar3 = 0;
    iStack_24 = 0x10;
    puVar5 = param_3;
    pHVar4 = param_2;
    do {
        psVar2 = ov96_021EB4F4((u8 *)param_1, 0x65, 2);
        pHVar4[0x15] = (enum HeapID)psVar2;
        VStack_20.z = 0;
        VStack_20.x = (iStack_24 & 0xFFFFFFFFu) << 0xc;
        bVar1 = GetMonPicHeightBySpeciesGenderForm(*puVar5, *((u8 *)(((u8 *)puVar5) + 7)), 0, (u8)puVar5[1], *((u32 *)(puVar5 + 6)));
        VStack_20.y = (bVar1 + 0x178) * 0x1000;
        ov96_021EB588(pHVar4[0x15], &VStack_20);
        ov96_021EB564(pHVar4[0x15], iVar3 + 1);
        ov96_021EB52C((s16 *)pHVar4[0x15], 1, 1);
        ov96_021EB630(pHVar4[0x15], 0x13 - iVar3);
        psVar2 = ov96_021EB4F4((u8 *)param_1, 0x65, 5);
        pHVar4[0x18] = (enum HeapID)psVar2;
        ov96_021EB564((s32)psVar2, 0);
        VStack_20.y = VStack_20.y + (-0x10000);
        ov96_021EB588(pHVar4[0x18], &VStack_20);
        ov96_021EB630(pHVar4[0x18], 4);
        iVar3 = iVar3 + 1;
        puVar5 = puVar5 + 8;
        iStack_24 = iStack_24 + 0x32;
        pHVar4 = pHVar4 + 1;
    } while (iVar3 < 3);
    ov96_02203DCC(param_2, param_3);
    ov96_02203F50((u8 *)param_1, param_2);
    ov96_02203F0C((s32)param_2);
    return;
}

void ov96_02203F0C(s32 param_1) {
    u32 uVar1;
    s32 iVar2;
    s32 iVar3;

    uVar1 = 0;
    do {
        iVar2 = param_1 + uVar1 * 8;
        DC_FlushRange(*(void **)(iVar2 + 0x24), 0xc80);
        iVar3 = param_1 + uVar1 * 4;
        GXS_LoadOBJ(*(void **)(iVar2 + 0x24), *(u32 *)(iVar3 + 0x3c), 0xc80);
        DC_FlushRange(*(void **)(iVar2 + 0x28), 0x20);
        GXS_LoadOBJPltt(*(void **)(iVar2 + 0x28), *(u32 *)(iVar3 + 0x48), 0x20);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 3);
    return;
}
