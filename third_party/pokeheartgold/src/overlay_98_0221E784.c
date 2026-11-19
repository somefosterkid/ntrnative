#include "global.h"

#include "overlay_98.h"
#include "sprite_system.h"

void ov98_0221E784(SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *arg2, s32 vram) {
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_0_0_8, 0x4C, 0, vram, *(s32 *)arg2);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_0_0_8, 0x4B, 0, 1, vram, *(s32 *)((u8 *)arg2 + 4));
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_0_0_8, 0x4D, 0, *(s32 *)((u8 *)arg2 + 8));
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_0_0_8, 0x4E, 0, *(s32 *)((u8 *)arg2 + 0xC));
}
