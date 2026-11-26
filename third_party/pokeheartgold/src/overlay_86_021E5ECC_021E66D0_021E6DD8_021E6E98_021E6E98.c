#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "overlay_86_frontier_records_app.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"

extern const OamCharTransferParam ov86_021E7ED8;

extern const SpriteResourceCountsListUnion ov86_021E7EEC;

extern const OamManagerParam ov86_021E7F74;

extern const ManagedSpriteTemplate ov86_021E802C[2];

void ov86_021E6E98(FrontierRecordsApp *arg0) {
    const ManagedSpriteTemplate *var_r6;
    u32 var_r7;
    OamManagerParam sp3C;
    OamCharTransferParam sp28;
    SpriteResourceCountsListUnion sp10;
    NARC *temp_r0;
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    arg0->spriteSystem = SpriteSystem_Alloc(HEAP_ID_121);
    arg0->spriteManager = SpriteManager_New(arg0->spriteSystem);
    sp3C = ov86_021E7F74;
    sp28 = ov86_021E7ED8;
    SpriteSystem_Init(arg0->spriteSystem, &sp3C, &sp28, 0x20);
    sp10 = ov86_021E7EEC;
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 2);
    SpriteSystem_InitManagerWithCapacities(arg0->spriteSystem, arg0->spriteManager, &sp10);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSystem), 0, (fx32)((!(512 > 0)) ? ((512 * (1 << 12)) - 0.5f) : ((512 * (1 << 12)) + 0.5f)));
    temp_r0 = NARC_New(NARC_a_1_8_4, HEAP_ID_121);
    SpriteSystem_LoadCharResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, temp_r0, 0x33, 1, 1, 0xD8CC);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, temp_r0, 0x40, 0, 2, 1, 0xD8CC);
    SpriteSystem_LoadCellResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, temp_r0, 0x31, 1, 0xD8CC);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arg0->spriteSystem, arg0->spriteManager, temp_r0, 0x32, 1, 0xD8CC);
    NARC_Delete(temp_r0);
    var_r7 = 0;
    var_r6 = ov86_021E802C;
    do {
        arg0->sprites[var_r7] = SpriteSystem_NewSprite(arg0->spriteSystem, arg0->spriteManager, var_r6);
        var_r6++;
        var_r7 += 1;
    } while (var_r7 < 2U);
}
