#include "global.h"

#include "filesystem_files_def.h"
#include "sprite_system.h"
#include "overlay_96_app_021ed1d0.h"

void ov96_021ECA18(Ov96App_021ED1D0 *param_1);

void ov96_021ECA18(Ov96App_021ED1D0 *param_1) {
    SpriteSystem *spriteSystem;
    int i;
    SpriteManager *spriteManager;
    NNS_G2D_VRAM_TYPE vram;

    spriteSystem = param_1->spriteSystem;
    spriteManager = param_1->spriteManager;
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    for (i = 0; i < 3; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x14, TRUE, vram, i + 0x6d);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x15, TRUE, 0x68);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0x16, TRUE, 0x68);
}
