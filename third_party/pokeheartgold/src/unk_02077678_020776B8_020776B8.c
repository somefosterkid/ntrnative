#include "global.h"

#include "sprite.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"

struct SpriteManager;

struct SpriteSystem;

u32 TypeIconGfx_GetCharFileId(int param0);

enum NarcId TypeIconGfx_GetNarcId(void);

void SpriteSystem_LoadMoveTypeIconCharResObj(SpriteSystem *param0, SpriteManager *param1, NNS_G2D_VRAM_TYPE param2, int param3, u32 param4);

void SpriteSystem_LoadMoveTypeIconCharResObj(SpriteSystem *param0, SpriteManager *param1, NNS_G2D_VRAM_TYPE param2, int param3, u32 param4) {
    SpriteSystem_LoadCharResObj(param0, param1, TypeIconGfx_GetNarcId(), TypeIconGfx_GetCharFileId(param3), 1, param2, param4);
}

u32 GetMoveTypeAndCategoryIconPlttFileId(void);

void SpriteSystem_LoadMoveTypeAndCategoryIconsPltt(SpriteSystem *spriteSys, SpriteManager *spriteMan, NNS_G2D_VRAM_TYPE vramType, u32 resourceID);

void SpriteSystem_LoadMoveTypeAndCategoryIconsPltt(SpriteSystem *spriteSys, SpriteManager *spriteMan, NNS_G2D_VRAM_TYPE vramType, u32 resourceID) {
    SpriteSystem_LoadPlttResObj(spriteSys, spriteMan, TypeIconGfx_GetNarcId(), GetMoveTypeAndCategoryIconPlttFileId(), 0, 3, vramType, resourceID);
}
