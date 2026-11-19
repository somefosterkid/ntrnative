#include "global.h"

#include "filesystem.h"
#include "overlay_05.h"
#include "sprite_system.h"
#include "sprite.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"


BOOL ov05_0221CCF4(void);

void ov05_0221D890(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_0_4, arg0->trainerIntroData->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 5, 0, 1, 0xB808);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 6, 0, 0xB808);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->unk_190, arg0->unk_194, narc, 7, 0, 0xB808);
    NARC_Delete(narc);
}

void ov05_0221D904(u8 **param_1) {
    BOOL BVar1;

    ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[0x72], 0);
    ManagedSprite_SetDrawFlag((ManagedSprite *)param_1[0x73], 0);
    if (*(u8 *)(*param_1 + 0x2a) == 3) {
        ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x72], 0x80, 0x48);
        ManagedSprite_SetAnim((ManagedSprite *)param_1[0x72], 2);
        return;
    }
    BVar1 = ov05_0221CCF4();
    if (BVar1 == 1) {
        if (*(u8 *)(*param_1 + 0x2a) == 1) {
            ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x72], 0x30, 0x48);
            ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x73], 0xd0, 0x48);
        } else {
            ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x72], 0xd0, 0x48);
            ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x73], 0x30, 0x48);
        }
    } else if (*(u8 *)(*param_1 + 0x2a) == 1) {
        ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x72], 0xd0, 0x48);
        ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x73], 0x30, 0x48);
    } else {
        ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x72], 0x30, 0x48);
        ManagedSprite_SetPositionXY((ManagedSprite *)param_1[0x73], 0xd0, 0x48);
    }
    ManagedSprite_SetAnim((ManagedSprite *)param_1[0x72], 0);
    ManagedSprite_SetAnim((ManagedSprite *)param_1[0x73], 1);
    return;
}
