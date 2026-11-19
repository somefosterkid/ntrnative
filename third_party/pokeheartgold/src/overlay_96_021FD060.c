#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_ov96_rower_work.h"

void ov96_021FD060(Ov96RowerWork *arg0);

Sprite *ov96_021EA424(struct UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);
Sprite *ov96_021EB408(enum HeapID *param_1, s32 param_2, NNS_G2D_VRAM_TYPE param_3, u8 param_4, u8 param_5);
s16 *ov96_021EB4F4(void *param_1, u32 param_2, u32 param_3);
void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);
void ov96_021EB564(void *param_1, s32 param_2);
void ov96_021EB588(void *param_1, VecFx32 *param_2);
u32 ov96_021EB5E8(void *param_1);
void ov96_021FD128(Ov96RowerWork *param_1, enum HeapID *param_2);

void ov96_021FD060(Ov96RowerWork *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_1_7_8, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 3, arg0->bgConfig, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_7_8, 4, arg0->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_7_8, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heapID);
    ov96_02200068(arg0->unk3E0);
}

void ov96_021EB2BC(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5);

void ov96_021EB2F4(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5, u8 param_6);

void ov96_021EB334(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021EB36C(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021FD0E4(enum HeapID *param_1);

void ov96_021FD0E4(enum HeapID *param_1) {
    ov96_021EB2BC(param_1, NARC_a_1_7_8, 0x11, 0x68, 1);
    ov96_021EB2F4(param_1, NARC_a_1_7_8, 0xe, 0x68, 1, 2);
    ov96_021EB334(param_1, NARC_a_1_7_8, 0x10, 0x68);
    ov96_021EB36C(param_1, NARC_a_1_7_8, 0xf, 0x68);
    return;
}

void ov96_021FD128(Ov96RowerWork *param_1, enum HeapID *param_2) {
    s16 *psVar1;
    SpriteList *pSVar2;
    Sprite *pSVar3;
    s32 iVar4;
    s32 iVar5;
    VecFx32 VStack_24;

    iVar4 = 0;
    do {
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 6);
        iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1e);
    iVar4 = 0;
    do {
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 7);
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 9);
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 0xb);
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 0xd);
        iVar4 = iVar4 + 1;
    } while (iVar4 < 4);
    iVar4 = 0;
    do {
        ov96_021EB408(param_2, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 0xe);
        iVar4 = iVar4 + 1;
    } while (iVar4 < 4);
    ov96_021EB408(param_2, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 0xc);
    ov96_021EB408(param_2, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0x68, 0xf);
    iVar5 = 0;
    do {
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 6);
        param_1->obstacles[iVar5].unk4 = psVar1;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 0x1e);
    iVar5 = 0;
    do {
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 7);
        param_1->rowers[iVar5].unk74 = psVar1;
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 9);
        param_1->rowers[iVar5].unk6C = psVar1;
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 0xb);
        param_1->rowers[iVar5].unk78 = psVar1;
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 0xd);
        param_1->rowers[iVar5].unk70 = psVar1;
        ov96_021EB564(param_1->rowers[iVar5].unk74, 3);
        ov96_021EB564(param_1->rowers[iVar5].unk70, 8);
        iVar5 = iVar5 + 1;
    } while (iVar5 < 4);
    iVar5 = 0;
    do {
        psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 0xe);
        param_1->unk3CC[iVar5] = psVar1;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 4);
    psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 0xc);
    param_1->unk1C = psVar1;
    ov96_021EB564((u8 *)psVar1, 0xb);
    ov96_021EB52C(param_1->unk1C, 1, 1);
    VStack_24.x = 0x20000;
    VStack_24.y = 0x10000;
    VStack_24.z = 0;
    ov96_021EB588(param_1->unk1C, &VStack_24);
    psVar1 = ov96_021EB4F4((u8 *)param_2, 0x68, 0xf);
    param_1->score.unk08 = psVar1;
    ov96_021EB564((u8 *)psVar1, 10);
    pSVar2 = (SpriteList *)ov96_021EB5E8(param_1->unk18);
    pSVar3 = ov96_021EA424(param_1->unk3C4, pSVar2, 0, param_1->heapID);
    param_1->score.digit0 = pSVar3;
    pSVar2 = (SpriteList *)ov96_021EB5E8(param_1->unk18);
    pSVar3 = ov96_021EA424(param_1->unk3C4, pSVar2, 0, param_1->heapID);
    param_1->score.digit1 = pSVar3;
    return;
}
