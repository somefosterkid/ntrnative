#include "global.h"
struct UnkStruct_ov10_0221FB28_t;

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_05.h"
#include "party_menu.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

void ov05_0221DC34(struct UnkStruct_ov10_0221FB28_t *arg0);

void ov05_0221DC34(UnkStruct_ov10_0221FB28_t *arg0) {
    GfGfx_EngineATogglePlanes(0x1EU, 0U);
    FreeBgTilemapBuffer(arg0->unk_0C, 3U);
    FreeBgTilemapBuffer(arg0->unk_0C, 2U);
    FreeBgTilemapBuffer(arg0->unk_0C, 1U);
    Heap_Free(arg0->unk_0C);
}

void ov05_0221DC60(UnkStruct_ov10_0221FB28 *arg0) {
    NNSG2dPaletteData *ppPltData;
    NARC *narc;
    void *v0;

    narc = NARC_New(NARC_graphic_plist_gra, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xF, arg0->unk_0C, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x11, arg0->unk_0C, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    v0 = NARC_AllocAndReadWholeMember(narc, 0x10U, arg0->trainerIntroData->heapID);
    NNS_G2dGetUnpackedPaletteData(v0, &ppPltData);
    BG_LoadPlttData(3U, ppPltData->pRawData, (u16)ppPltData->szByte, 0U);
    memcpy(arg0->unk_4E4, ppPltData->pRawData + 0x60, 0x60U);
    Heap_Free(v0);
    sub_0207CAAC(arg0->trainerIntroData->heapID, arg0->unk_2A4, arg0->unk_364, arg0->unk_424);
    NARC_Delete(narc);
}
