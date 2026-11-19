#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_67_app.h"

extern OamCharTransferParam ov67_021E6D7C;

extern SpriteResourceCountsListUnion ov67_021E6D90;

extern OamManagerParam ov67_021E6E18;

extern ManagedSpriteTemplate ov67_021E6F50[4];

void ov67_021E6820(Ov67App *arg0) {
    OamManagerParam sp3C;
    OamCharTransferParam sp28;
    SpriteResourceCountsListUnion sp10;
    u32 i;
    NARC *temp_r0;

    arg0->spriteSys = SpriteSystem_Alloc(arg0->heapID);
    arg0->spriteMan = SpriteManager_New(arg0->spriteSys);
    sp3C = ov67_021E6E18;
    sp28 = ov67_021E6D7C;
    SpriteSystem_Init(arg0->spriteSys, &sp3C, &sp28, 0x20);
    SpriteSystem_InitSprites(arg0->spriteSys, arg0->spriteMan, 4);
    sp10 = ov67_021E6D90;
    SpriteSystem_InitManagerWithCapacities(arg0->spriteSys, arg0->spriteMan, &sp10);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSys), 0, FX32_CONST(512));
    temp_r0 = NARC_New(NARC_a_1_1_8, arg0->heapID);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->spriteSys, arg0->spriteMan, temp_r0, 0, 1, 1, 0xD158);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->spriteSys, arg0->spriteMan, temp_r0, 3, 0, 2, 1, 0xD158);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->spriteSys, arg0->spriteMan, temp_r0, 1, 1, 0xD158);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->spriteSys, arg0->spriteMan, temp_r0, 2, 1, 0xD158);
    NARC_Delete(temp_r0);
    for (i = 0; i < 4; i++) {
        arg0->sprites[i] = SpriteSystem_NewSprite(arg0->spriteSys, arg0->spriteMan, &ov67_021E6F50[i]);
    }
    if (arg0->isRankingsScreen == 0) {
        ManagedSprite_SetDrawFlag(arg0->sprites[2], 0);
        ManagedSprite_SetDrawFlag(arg0->sprites[3], 0);
    }
    GfGfx_EngineATogglePlanes(0x10U, 1U);
}
