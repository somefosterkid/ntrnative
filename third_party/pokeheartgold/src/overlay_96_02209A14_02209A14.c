#include "global.h"

#include "filesystem_files_def.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

void ov96_02209A14(Ov96_0220Data *param_1);

void ov96_02209A14(Ov96_0220Data *param_1) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    NNS_G2D_VRAM_TYPE vram;

    spriteManager = param_1->spriteManager;
    spriteSystem = param_1->spriteSystem;
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 8, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 7, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 9, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 10, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xc, TRUE, vram, 0x2711);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xb, FALSE, 3, vram, 0x2711);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xd, TRUE, 0x2711);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xe, TRUE, 0x2711);
    param_1->unk_214 = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, sub_02074490(), FALSE, 3, vram, 0x2712);
}

void ov96_02209B04(SpriteSystem *spriteSystem, SpriteManager *spriteManager);

void ov96_02209B04(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    s32 i;
    NNS_G2D_VRAM_TYPE vram;

    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0x10, TRUE, vram, 0x2712);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xF, FALSE, 1, vram, 0x2713);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    for (i = 0; i < 2; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0x10, TRUE, vram, i + 0x2713);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0xF, FALSE, 1, vram, i + 0x2714);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0x11, TRUE, 0x2712);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 0x12, TRUE, 0x2712);
}
