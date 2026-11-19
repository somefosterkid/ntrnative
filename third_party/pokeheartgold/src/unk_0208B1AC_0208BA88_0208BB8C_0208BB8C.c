#include "global.h"
struct PokemonSummary;

#include "filesystem_files_def.h"
#include "move.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02077678.h"
#include "pokemon_summary_app.h"

void sub_0208BB8C(struct PokemonSummary *param_1, u16 param_2);

void sub_0208BB8C(PokemonSummary *param_1, u16 param_2) {
    u32 uVar1;
    NarcId narcId;
    s32 iVar2;

    uVar1 = GetMoveAttr(param_2, MOVEATTR_CLASS);
    narcId = GetMoveCategoryIconGfxNarcId();
    iVar2 = GetMoveCategoryIconGfxCharFileId(uVar1);
    SpriteSystem_ReplaceCharResObj(param_1->spriteSystem, param_1->spriteManager, narcId, iVar2, 1, 10);
    iVar2 = GetMoveCategoryIconGfxPlttOverride(uVar1);
    thunk_Sprite_SetPaletteOverride(param_1->sprites[18], iVar2 + 3);
    return;
}
