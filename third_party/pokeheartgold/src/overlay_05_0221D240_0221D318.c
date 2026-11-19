#include "global.h"

#include "filesystem.h"
#include "overlay_05.h"
#include "sprite_system.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

void ov05_0221D318(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_0_0_8, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0xD0, 1, 1, 0xB807);
    SpriteSystem_LoadPlttResObj(arg0->unk_190, arg0->unk_194, NARC_a_1_0_4, 8, 0, 2, 1, 0xB807);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0xCF, 1, 0xB807);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 0xD1, 1, 0xB807);
    NARC_Delete(narc);
}
