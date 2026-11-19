#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "overlay_106.h"
#include "overlay_106_arceus_cinematic.h"
#include "overlay_106_legendary_cinematic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"

extern OamCharTransferParam ov106_021E6F48;

extern SpriteResourceCountsListUnion ov106_021E6F5C;

extern OamManagerParam ov106_021E6FC8;

extern ManagedSpriteTemplate ov106_021E7010;

void ov106_021E6520(LegendaryCinematic *arg0);

void ov106_021E6520(LegendaryCinematic *arg0) {
    OamManagerParam sp3C;
    OamCharTransferParam sp28;
    SpriteResourceCountsListUnion sp10;
    ManagedSprite *temp_r0_2;
    SpriteSystem *temp_r0;
    ArceusCinematic *var_r4;
    LegendaryCinematic *var_r7;

    var_r7 = arg0;
    var_r4 = var_r7->arceus;
    temp_r0 = SpriteSystem_Alloc(HEAP_ID_153);
    var_r4->spriteSystem = temp_r0;
    var_r4->spriteManager = SpriteManager_New(temp_r0);
    sp3C = ov106_021E6FC8;
    sp28 = ov106_021E6F48;
    SpriteSystem_Init(var_r4->spriteSystem, &sp3C, &sp28, 0x20);
    sp10 = ov106_021E6F5C;
    SpriteSystem_InitSprites(var_r4->spriteSystem, var_r4->spriteManager, 1);
    SpriteSystem_InitManagerWithCapacities(var_r4->spriteSystem, var_r4->spriteManager, &sp10);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(var_r4->spriteSystem), 0, FX32_CONST(1024));
    SpriteSystem_LoadCellResObjFromOpenNarc(var_r4->spriteSystem, var_r4->spriteManager, var_r4->narc, 0xCD, 1, 0xC8E9);
    SpriteSystem_LoadAnimResObjFromOpenNarc(var_r4->spriteSystem, var_r4->spriteManager, var_r4->narc, 0xCE, 1, 0xC8E9);
    SpriteSystem_LoadPlttResObjFromOpenNarc(var_r4->spriteSystem, var_r4->spriteManager, var_r4->narc, 0xCF, 0, 1, 1, 0xC8E9);
    SpriteSystem_LoadCharResObjFromOpenNarc(var_r4->spriteSystem, var_r4->spriteManager, var_r4->narc, 0xCC, 1, 1, 0xC8E9);
    temp_r0_2 = SpriteSystem_NewSprite(var_r4->spriteSystem, var_r4->spriteManager, &ov106_021E7010);
    var_r4->sprites[0] = temp_r0_2;
    ManagedSprite_SetDrawFlag(temp_r0_2, 0);
    ov106_021E66FC(var_r7);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}
