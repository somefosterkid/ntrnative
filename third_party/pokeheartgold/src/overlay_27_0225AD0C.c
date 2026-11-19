#include "global.h"

#include "bag.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_27.h"
#include "player_data.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225AEA8(void *, void *, void *, s32, s32, u32, Bag *, u32);

void ov27_0225AD0C(Ov27Task *arg0) {
    NNSG2dPaletteData *ppPlttData;
    s32 i;
    u32 gender;
    void *ptr;

    arg0->unk_18 = G2dRenderer_Init(0x10, &arg0->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&arg0->renderer, 0, 0x100000);
    for (i = 0; i < 4; i++) {
        arg0->unk_144[i] = Create2DGfxResObjMan(11, (GfGfxResType)i, HEAP_ID_8);
    }
    for (i = 0; i < 11; i++) {
        gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(arg0->fieldSystem->saveData));
        ov27_0225AEA8(arg0->fieldSystem, arg0->unk_144, arg0->unk_154[i], i, i + 100, gender, Save_Bag_Get(arg0->fieldSystem->saveData), arg0->unk_51C_1);
    }
    arg0->unk_154[0][2] = AddCellOrAnimResObjFromNarc(arg0->unk_144[2], NARC_a_0_1_4, 0x10, 1, 0x64, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    arg0->unk_154[0][3] = AddCellOrAnimResObjFromNarc(arg0->unk_144[3], NARC_a_0_1_4, 0x11, 1, 0x64, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    arg0->unk_154[1][2] = AddCellOrAnimResObjFromNarc(arg0->unk_144[2], NARC_a_0_1_4, 0x44, 1, 0x65, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    arg0->unk_154[1][3] = AddCellOrAnimResObjFromNarc(arg0->unk_144[3], NARC_a_0_1_4, 0x45, 1, 0x65, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    arg0->unk_154[2][2] = AddCellOrAnimResObjFromNarc(arg0->unk_144[2], NARC_a_0_1_4, 0x36, 1, 0x66, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    arg0->unk_154[2][3] = AddCellOrAnimResObjFromNarc(arg0->unk_144[3], NARC_a_0_1_4, 0x37, 1, 0x66, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
    ptr = GfGfxLoader_GetPlttData(NARC_a_0_1_4, 0xE, &ppPlttData, HEAP_ID_8);
    DC_FlushRange(ppPlttData->pRawData, 0x40U);
    MIi_CpuCopyFast(ppPlttData->pRawData, arg0->unk_4CC, 0x40U);
    Heap_Free(ptr);
}

s32 ov27_0225AE8C(s32 arg0) {
    if (arg0 == 1) {
        return 5;
    }
    if (arg0 == 3) {
        return 0x1F4;
    }
    return 0x1F3;
}
