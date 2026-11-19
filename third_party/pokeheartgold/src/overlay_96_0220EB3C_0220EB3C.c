#include "global.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void ov96_0220EB3C(UnkStruct_ov96_0220E3F0 *param_1);

void ov96_0220EB3C(UnkStruct_ov96_0220E3F0 *param_1) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    NNS_G2D_VRAM_TYPE vram;

    spriteSystem = param_1->spriteSystem;
    spriteManager = param_1->spriteManager;
    if (spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (spriteManager == NULL) {
        GF_AssertFail();
    }
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x16, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x13, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x15, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x14, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x1a, TRUE, vram, 0x2711);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x17, FALSE, 4, vram, 0x2711);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x19, TRUE, 0x2711);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x18, TRUE, 0x2711);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x12, TRUE, vram, 0x2712);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0xf, FALSE, 1, vram, 0x2712);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x12, TRUE, vram, 0x2713);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0xf, FALSE, 1, vram, 0x2713);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x11, TRUE, 0x2712);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 0x10, TRUE, 0x2712);
}
