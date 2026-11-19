#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_113.h"

extern u32 ov113_021E6B74;

extern u32 ov113_021E6BA4;

extern SpriteResourceCountsListUnion ov113_021E6BB8;

extern u32 ov113_021E6CB0;

void ov113_021E677C(UnownReportData *param_1);

void ov113_021E677C(UnownReportData *param_1) {
    NARC *pNVar3;
    u32 uVar4;
    u32 uVar5;
    SpriteResourceCountsListUnion SStack_30;

    GF_CreateVramTransferManager(0x20, param_1->heapID);
    param_1->spriteSystem = SpriteSystem_Alloc(param_1->heapID);
    SpriteSystem_Init(param_1->spriteSystem, (OamManagerParam *)&ov113_021E6CB0, (OamCharTransferParam *)&ov113_021E6BA4, 2);
    thunk_ClearMainOAM(param_1->heapID);
    thunk_ClearSubOAM(param_1->heapID);
    param_1->spriteManager1 = SpriteManager_New(param_1->spriteSystem);
    SpriteSystem_InitSprites(param_1->spriteSystem, param_1->spriteManager1, 0x11);
    sub_0200D2A4(param_1->spriteSystem, param_1->spriteManager1, (ResdatIdList *)&ov113_021E6B74, 0, 0);
    SStack_30 = ov113_021E6BB8;
    param_1->spriteManager2 = SpriteManager_New(param_1->spriteSystem);
    SpriteSystem_InitManagerWithCapacities(param_1->spriteSystem, param_1->spriteManager2, &SStack_30);
    SpriteManager_SetSpriteList(param_1->spriteManager2, SpriteManager_GetSpriteList(param_1->spriteManager1));
    pNVar3 = NARC_New(NARC_poketool_icongra_poke_icon, param_1->heapID);
    param_1->narc = pNVar3;
    uVar4 = sub_02074490();
    SpriteSystem_LoadPlttResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager2, param_1->narc, uVar4, 0, 1, 3, 1);
    uVar4 = sub_02074494();
    SpriteSystem_LoadCellResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager2, param_1->narc, uVar4, 0, 1);
    uVar4 = sub_020744A0();
    SpriteSystem_LoadAnimResObj(param_1->spriteSystem, param_1->spriteManager2, NARC_poketool_icongra_poke_icon, uVar4, 0, 1);
    uVar4 = 0;
    do {
        uVar5 = GetMonIconNaixEx(0xc9, 0, uVar4);
        SpriteSystem_LoadCharResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager2, param_1->narc, uVar5, 0, 2, uVar4 + 1);
        uVar4 = uVar4 + 1;
    } while ((s32)uVar4 < 7);
    uVar4 = 7;
    do {
        uVar5 = GetMonIconNaixEx(0xc9, 0, uVar4);
        SpriteSystem_LoadCharResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager2, param_1->narc, uVar5, 0, 1, uVar4 + 1);
        uVar4 = uVar4 + 1;
    } while ((s32)uVar4 < 0xe);
    return;
}
