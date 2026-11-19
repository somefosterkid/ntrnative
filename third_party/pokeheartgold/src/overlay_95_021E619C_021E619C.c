#include "global.h"
#include "particle_system.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_95_egg_hatch_app.h"
#include "bg_window.h"
#include "filesystem.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite_system.h"

void ov95_021E5C44(BgConfig *bgConfig, Window *window, enum GFBgLayer bgLayer, int tilemapTop, int tilemapLeft, int width, int height, int baseTile, int palette);

ParticleSystem *NewParticleSystem(enum HeapID heapID);

ParticleSystem *CreateParticleSystem(enum HeapID heapID, int narcID, int narcMemberIdx);

void ov95_021E619C(EggHatchGraphics *graphics);

void ov95_021E619C(EggHatchGraphics *graphics) {
    graphics->spriteSys = SpriteSystem_Alloc(HEAP_ID_70);
    const OamManagerParam renderOamTemplate = {
        .fromOBJmain = 0,
        .numOBJmain = 128,
        .fromAffineMain = 0,
        .numAffineMain = 32,
        .fromOBJsub = 0,
        .numOBJsub = 128,
        .fromAffineSub = 0,
        .numAffineSub = 32,
    };
    const OamCharTransferParam charTransferTemplate = {
        .maxTasks = 96,
        .sizeMain = 1024 * 64,
        .sizeSub = 512 * 32,
        .charModeMain = GX_OBJVRAMMODE_CHAR_1D_64K,
        .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K
    };

    SpriteSystem_Init(graphics->spriteSys, &renderOamTemplate, &charTransferTemplate, 32);

    const SpriteResourceCountsListUnion capacities = {
        .numChar = 96,
        .numPltt = 32,
        .numCell = 64,
        .numAnim = 64,
        .numMcel = 16,
        .numManm = 16,
    };

    graphics->spriteMan = SpriteManager_New(graphics->spriteSys);

    BOOL success = SpriteSystem_InitSprites(graphics->spriteSys, graphics->spriteMan, 255);
    GF_ASSERT(success);

    success = SpriteSystem_InitManagerWithCapacities(graphics->spriteSys, graphics->spriteMan, &capacities);
    GF_ASSERT(success);
}
