#include "global.h"
#include "sys_task.h"
#include "sprite.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite_system.h"

struct SpriteManager;

struct SpriteSystem;

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

u32 GetMoveTypeAndCategoryIconCellFileId(void);

u32 GetMoveTypeAndCategoryIconAnimFileId(void);

enum NarcId TypeIconGfx_GetNarcId(void);

void SpriteSystem_LoadMoveTypeAndCategoryIconsCellAndAnim(SpriteSystem *param0, SpriteManager *param1, u32 param2, u32 param3);

struct ManagedSprite;
BOOL SpriteManager_UnloadCharObjById(SpriteManager *spriteManager, u32 character);
BOOL SpriteManager_UnloadPlttObjById(SpriteManager *spriteManager, u32 pal);
ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);
u8 TypeIconGfx_GetPlttOverride(int param0);
void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);
BOOL SpriteManager_UnloadMoveTypeIconCharResObjByTag(SpriteManager *spriteManager, u32 character);
BOOL SpriteManager_UnloadMoveTypeAndCategoryIconPlttResObjByTag(SpriteManager *spriteManager, u32 pal);
void SpriteManager_UnloadMoveTypeAndCategoryIconCellAndAnimResObjsByTags(SpriteManager *param0, u32 param1, u32 param2);
ManagedSprite *SpriteSystem_CreateMoveTypeIconSprite(SpriteSystem *param0, SpriteManager *param1, int param2, const ManagedSpriteTemplate *param3);
void MoveTypeIconSprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void SpriteSystem_LoadMoveTypeAndCategoryIconsCellAndAnim(SpriteSystem *param0, SpriteManager *param1, u32 param2, u32 param3) {
    SpriteSystem_LoadCellResObj(param0, param1, TypeIconGfx_GetNarcId(), GetMoveTypeAndCategoryIconCellFileId(), 1, param2);
    SpriteSystem_LoadAnimResObj(param0, param1, TypeIconGfx_GetNarcId(), GetMoveTypeAndCategoryIconAnimFileId(), 1, param3);
}

BOOL SpriteManager_UnloadMoveTypeIconCharResObjByTag(SpriteManager *spriteManager, u32 character) {
    return SpriteManager_UnloadCharObjById(spriteManager, character);
}

BOOL SpriteManager_UnloadMoveTypeAndCategoryIconPlttResObjByTag(SpriteManager *spriteManager, u32 pal) {
    return SpriteManager_UnloadPlttObjById(spriteManager, pal);
}

void SpriteManager_UnloadMoveTypeAndCategoryIconCellAndAnimResObjsByTags(SpriteManager *param0, u32 param1, u32 param2) {
    SpriteManager_UnloadCellObjById(param0, param1);
    SpriteManager_UnloadAnimObjById(param0, param2);
}

ManagedSprite *SpriteSystem_CreateMoveTypeIconSprite(SpriteSystem *param0, SpriteManager *param1, int param2, const ManagedSpriteTemplate *param3) {
    ManagedSprite *v0;
    ManagedSpriteTemplate v1;

    v1 = *param3;
    v1.pal = TypeIconGfx_GetPlttOverride(param2);
    v0 = SpriteSystem_NewSprite(param0, param1, &v1);

    return v0;
}

void MoveTypeIconSprite_DeleteAndFreeResources(ManagedSprite *managedSprite) {
    Sprite_DeleteAndFreeResources(managedSprite);
}
