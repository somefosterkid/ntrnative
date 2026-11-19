#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "overlay_96_ov96hurdle_work.h"

void ov96_02207DDC(struct Ov96HurdleHud *);

void ov96_02205048(Ov96HurdleWork *arg0);

void ov96_02205048(Ov96HurdleWork *arg0) {
    void *pNcgrFile;

    GfGfxLoader_LoadCharData(NARC_a_2_2_4, 1, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapId);
    pNcgrFile = GfGfxLoader_LoadFromNarc(NARC_a_2_2_4, 3, 0, arg0->heapId, 0);
    arg0->unk_18 = pNcgrFile;
    NNS_G2dGetUnpackedBGCharacterData(pNcgrFile, &arg0->unk_1C);
    GfGfxLoader_LoadScrnData(NARC_a_2_2_4, 2, arg0->bgConfig, GF_BG_LYR_MAIN_FIRST, 0U, 0U, 0, arg0->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_2_4, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, arg0->heapId);
    ov96_02207DDC(arg0->hud);
}

void ov96_021EB2BC(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5);

void ov96_021EB2F4(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5, u8 param_6);

void ov96_021EB334(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021EB36C(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_022050B4(enum HeapID *param_1);

void ov96_022050B4(enum HeapID *param_1) {
    ov96_021EB2BC(param_1, NARC_a_2_2_4, 0x11, 0x68, 1);
    ov96_021EB2F4(param_1, NARC_a_2_2_4, 0xe, 0x68, 1, 2);
    ov96_021EB334(param_1, NARC_a_2_2_4, 0x10, 0x68);
    ov96_021EB36C(param_1, NARC_a_2_2_4, 0xf, 0x68);
    return;
}

void *ov96_021EB3E4(enum HeapID *, s32, s32, u8, u8);
s32 ov96_021EB564(void *, s32);
s32 ov96_021EB630(void *, s32);

void ov96_022050F8(Ov96HurdleWork *arg0, void *arg1);

void ov96_022050F8(Ov96HurdleWork *arg0, void *arg1) {
    s32 i;
    u8 j;

    for (i = 0; i < 20; i++) {
        arg0->cells[i].sprite = ov96_021EB3E4(arg1, 2, 1, 0x68, 6);
        ov96_021EB564(arg0->cells[i].sprite, 1);
    }
    for (i = 0; i < 10; i++) {
        arg0->pairs[i].sprites[0] = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x11);
        ov96_021EB564(arg0->pairs[i].sprites[0], 6);
        arg0->pairs[i].sprites[1] = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x12);
        ov96_021EB564(arg0->pairs[i].sprites[1], 5);
    }
    for (i = 0; i < 4; i++) {
        arg0->runners[i].unk_48 = ov96_021EB3E4(arg1, 2, 1, 0x68, 9);
        ov96_021EB564(arg0->runners[i].unk_48, 2);
        j = 0;
        do {
            arg0->runners[i].unk_4C[j] = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x17);
            ov96_021EB564(arg0->runners[i].unk_4C[j], 3);
            ov96_021EB630(arg0->runners[i].unk_4C[j], 2);
            j++;
        } while (j < 2);
        arg0->runners[i].unk_54 = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x1B);
        ov96_021EB564(arg0->runners[i].unk_54, 8);
    }
    for (i = 0; i < 4; i++) {
        arg0->unk_34C[i] = ov96_021EB3E4(arg1, 2, 1, 0x68, 0xE);
        ov96_021EB564(arg0->unk_34C[i], 7);
        ov96_021EB630(arg0->unk_34C[i], 4);
    }
    for (i = 0; i < 4; i++) {
        arg0->unk_35C[i] = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x19);
        ov96_021EB564(arg0->unk_35C[i], 4);
    }
    for (i = 0; i < 5; i++) {
        arg0->slots[i].unk_8 = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x1A);
        arg0->slots[i].unk_4 = ov96_021EB3E4(arg1, 2, 1, 0x68, 0x10);
        ov96_021EB564(arg0->slots[i].unk_4, 0);
        ov96_021EB564(arg0->slots[i].unk_8, 4);
    }
}
