#include "global.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"
#include "overlay_96_ov96_rd_work.h"

void ov96_022163AC(Ov96RdWork *work);

void ov96_022163AC(Ov96RdWork *work) {
    SpriteSystem *spriteSystem;
    s32 i;
    SpriteManager *spriteManager;
    NNS_G2D_VRAM_TYPE vram;

    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x10, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0xD, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0xF, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0xE, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x14, TRUE, vram, 10001);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x11, FALSE, 2, vram, 10001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x13, TRUE, 10001);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x12, TRUE, 10001);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    for (i = 0; i < 2; i++) {
        /* 10002U: the asm recomputes i + 10002 per call (no CSE): an unsigned id here, or a u32 resId in the TU's prototype */
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x18, TRUE, vram, i + 10002U);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x15, FALSE, 1, vram, i + 10002);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x17, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 0x16, TRUE, 10002);
}
