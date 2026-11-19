#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_14_irc_battle_match.h"
#include "overlay_64_hall_of_fame_showcase.h"

extern OamCharTransferParam ov64_021E6EB8;

extern SpriteResourceCountsListUnion ov64_021E6EE4;

extern OamManagerParam ov64_021E6F8C;

extern ManagedSpriteTemplate ov64_021E70C8;

extern ManagedSpriteTemplate ov64_021E70FC;

extern ManagedSpriteTemplate ov64_021E73A0;

extern ManagedSpriteTemplate ov64_021E73D4;

extern ManagedSpriteTemplate ov64_021E7408;

extern ManagedSpriteTemplate ov64_021E743C;

void ov64_021E5CD0(HallOfFameShowcase *arg0);

void ov64_021E5CD0(HallOfFameShowcase *arg0) {
    OamManagerParam sp3C;
    OamCharTransferParam sp28;
    SpriteResourceCountsListUnion sp10;
    NARC *temp_r0;
    NARC *temp_r0_2;
    s32 var_r4;

    *((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]) = SpriteSystem_Alloc(HEAP_ID_59);
    *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]) = SpriteManager_New(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]));

    sp3C = ov64_021E6F8C;
    sp28 = ov64_021E6EB8;
    SpriteSystem_Init(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), &sp3C, &sp28, 0x20);

    sp10 = ov64_021E6EE4;
    SpriteSystem_InitSprites(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), 0x12);
    SpriteSystem_InitManagerWithCapacities(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &sp10);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8])), 0, FX32_CONST(512));
    temp_r0 = NARC_New(NARC_a_0_0_8, HEAP_ID_59);
    SpriteSystem_LoadCharResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4C, 0, 2, 0xDCC0);
    SpriteSystem_LoadCharResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4C, 0, 2, 0xDCC1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4B, 0, 1, 2, 0xDCC0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4B, 0, 1, 2, 0xDCC1);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4D, 0, 0xDCC0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0, 0x4E, 0, 0xDCC0);
    NARC_Delete(temp_r0);
    temp_r0_2 = NARC_New(NARC_a_0_9_7, HEAP_ID_59);
    SpriteSystem_LoadCharResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 6, 1, 1, 0xDCCE);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 7, 1, 0xDCC3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 8, 1, 0xDCC3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 9, 0, 2, 1, 0xDCCE);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 0xB, 1, 0xDCC1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 0xC, 1, 0xDCC1);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 0xE, 1, 0xDCC2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 0xF, 1, 0xDCC2);
    var_r4 = 0xDCC2;
    do {
        SpriteSystem_LoadPlttResObjFromOpenNarc(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), temp_r0_2, 0x10, 0, 1, 1, var_r4);
        var_r4 += 1;
    } while ((u32)var_r4 <= 0xDCCDU);
    NARC_Delete(temp_r0_2);
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[8][12]) = SpriteSystem_NewSpriteWithYOffset(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E70C8, FX32_CONST(512));
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[8][14]) = SpriteSystem_NewSpriteWithYOffset(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E70FC, FX32_CONST(512));
    ManagedSprite_SetDrawFlag(*((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[8][12]), 0);
    ManagedSprite_SetDrawFlag(*((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[8][14]), 0);
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[10][4]) = SpriteSystem_NewSprite(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E73A0);
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[10][6]) = SpriteSystem_NewSprite(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E73D4);
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[10][8]) = SpriteSystem_NewSprite(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E7408);
    *((ManagedSprite **)&((IrcBattleMatch *)arg0)->palettes[10][10]) = SpriteSystem_NewSprite(*((SpriteSystem **)&((IrcBattleMatch *)arg0)->palettes[8][8]), *((SpriteManager **)&((IrcBattleMatch *)arg0)->palettes[8][10]), &ov64_021E743C);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
