#include "global.h"

#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite_system.h"
#include "unk_02077678.h"

s32 GetMoveTypeAndCategoryIconPlttFileId(void);

void SpriteSystem_LoadMoveTypeAndCategoryIconsPlttToBuffer(PaletteData *paletteData, PaletteBufferId bufferId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, int vram, int plttTag) {
    SpriteManager *v0;
    enum NarcId narcId;

    v0 = spriteManager;
    narcId = TypeIconGfx_GetNarcId();
    SpriteSystem_LoadPaletteBuffer(paletteData, bufferId, spriteSystem, spriteManager, narcId, GetMoveTypeAndCategoryIconPlttFileId(), 0, 3, vram, plttTag);
}
