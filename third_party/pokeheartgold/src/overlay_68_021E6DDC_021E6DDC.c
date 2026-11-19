#include "global.h"
#include "gf_gfx_planes.h"
#include "overlay_68_move_relearner.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

void ov68_021E6DDC(MoveRelearner *relearner);

extern const SpriteResourceCountsListUnion ov68_021E7C2C;

extern const OamManagerParam ov68_021E7CD0;

extern const OamCharTransferParam ov68_021E7C18;

void ov68_021E6DDC(MoveRelearner *relearner) {
    SpriteResourceCountsListUnion resourceCounts = ov68_021E7C2C;

    GF_CreateVramTransferManager(64, HEAP_ID_66);

    relearner->spriteSystem = SpriteSystem_Alloc(HEAP_ID_66);
    relearner->spriteManager = SpriteManager_New(relearner->spriteSystem);

    OamManagerParam oamManagerParam = ov68_021E7CD0;

    OamCharTransferParam oamCharTransferParam = ov68_021E7C18;

    SpriteSystem_Init(relearner->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    SpriteSystem_InitSprites(relearner->spriteSystem, relearner->spriteManager, MOVE_RELEARNER_NUM_SPRITES);
    SpriteSystem_InitManagerWithCapacities(relearner->spriteSystem, relearner->spriteManager, &resourceCounts);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}
