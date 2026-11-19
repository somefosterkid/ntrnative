#include "global.h"

#include "filesystem.h"
#include "overlay_05.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

void ov05_0221E2D8(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;
    u32 v0;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, (s32)sub_02074490(), 0, 3, 1, 0xB808);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, (s32)sub_02074498(), 0, 0xB809);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, (s32)sub_020744A4(), 0, 0xB809);
    v0 = 0;
    do {
        SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, arg0->unk_214[v0].unk_00, 0, 1, v0 + 0xB809);
        v0 += 1;
    } while (v0 < 6U);
    NARC_Delete(narc);
}

void ov05_0221E390(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_graphic_plist_gra, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x14, 0, 1, 0xB80F);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x15, 0, 1, 1, 0xB809);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x13, 0, 0xB80A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x12, 0, 0xB80A);
    NARC_Delete(narc);
}

void ov05_0221E42C(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_0_3_9, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x40, 0, 1, 0xB810);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x41, 0, 1, 1, 0xB80A);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x3F, 0, 0xB80B);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0x3E, 0, 0xB80B);
    NARC_Delete(narc);
}

void ov05_0221E4C8(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_graphic_plist_gra, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 2, 0, 1, 0xB811);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 8, 0, 2, 1, 0xB80B);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 1, 0, 0xB80C);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0, 0, 0xB80C);
    NARC_Delete(narc);
}
