#include "global.h"
struct EvolutionData;

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "evolution_data.h"

ManagedSprite *sub_020755E8(SpriteSystem *, SpriteManager *, s32, s32, u8);

typedef struct Words2 {
    u32 w[2];
} Words2;

extern const OamCharTransferParam _020FFEE8;

extern SpriteResourceCountsListUnion _020FFEFC;

extern OamManagerParam _020FFF14;

void sub_02075630(struct EvolutionData *arg0);

void sub_02075630(EvolutionData *arg0) {
    OamManagerParam sp3C;
    OamCharTransferParam sp28;
    SpriteResourceCountsListUnion sp10;
    SpriteSystem *temp_r6_2;
    SpriteManager *temp_r4_5;
    NARC *temp_r0;

    arg0->unk_AC = SpriteSystem_Alloc(arg0->heapID);
    arg0->unk_B0 = SpriteManager_New(arg0->unk_AC);
    sp3C = _020FFF14;
    sp28 = _020FFEE8;
    sp28.maxTasks = 0x10;
    SpriteSystem_Init(arg0->unk_AC, &sp3C, &sp28, 0x20);
    SpriteSystem_InitSprites(arg0->unk_AC, arg0->unk_B0, 0x10);
    sp10 = _020FFEFC;
    SpriteSystem_InitManagerWithCapacities(arg0->unk_AC, arg0->unk_B0, &sp10);
    temp_r6_2 = arg0->unk_AC;
    temp_r4_5 = arg0->unk_B0;
    temp_r0 = NARC_New(NARC_a_2_3_7, arg0->heapID);
    SpriteSystem_LoadPlttResObjFromOpenNarc(temp_r6_2, temp_r4_5, temp_r0, 0xB, 0, 1, 2, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(temp_r6_2, temp_r4_5, temp_r0, 0xC, 0, 2, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(temp_r6_2, temp_r4_5, temp_r0, 0xD, 0, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(temp_r6_2, temp_r4_5, temp_r0, 0xE, 0, 0);
    NARC_Delete(temp_r0);
    arg0->unk_B4 = sub_020755E8(arg0->unk_AC, arg0->unk_B0, 0x80, 0x48, 1);
    ManagedSprite_SetDrawFlag(arg0->unk_B4, 0);
    arg0->unk_B8 = TRUE;
}
