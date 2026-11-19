#include "global.h"
#include "sys_task.h"
#include "sprite.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"

struct SpriteManager;

struct SpriteSystem;

u32 GetMoveCategoryIconGfxCharFileId(int param0);

enum NarcId GetMoveCategoryIconGfxNarcId(void);

void SpriteSystem_LoadMoveCategoryIconCharResObj(SpriteSystem *param0, SpriteManager *param1, NNS_G2D_VRAM_TYPE param2, int param3, u32 param4);

BOOL SpriteManager_UnloadCharObjById(SpriteManager *spriteManager, u32 character);
void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);
BOOL SpriteManager_UnloadMoveCategoryIconCharResObjByTag(SpriteManager *spriteManager, u32 character);
void MoveCategoryIconSprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void SpriteSystem_LoadMoveCategoryIconCharResObj(SpriteSystem *param0, SpriteManager *param1, NNS_G2D_VRAM_TYPE param2, int param3, u32 param4) {
    SpriteSystem_LoadCharResObj(param0, param1, GetMoveCategoryIconGfxNarcId(), GetMoveCategoryIconGfxCharFileId(param3), 1, param2, param4);
}

BOOL SpriteManager_UnloadMoveCategoryIconCharResObjByTag(SpriteManager *spriteManager, u32 character) {
    return SpriteManager_UnloadCharObjById(spriteManager, character);
}

void MoveCategoryIconSprite_DeleteAndFreeResources(ManagedSprite *managedSprite) {
    Sprite_DeleteAndFreeResources(managedSprite);
}
