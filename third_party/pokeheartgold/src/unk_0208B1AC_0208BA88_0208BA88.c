#include "global.h"

#include "sprite_system.h"
#include "unk_02077678.h"
#include "pokemon_summary_app.h"

void sub_0208BA88(PokemonSummary *arg0, s32 arg1, s32 arg2, u8 arg3);

void sub_0208BA88(PokemonSummary *arg0, s32 arg1, s32 resId, u8 moveType) {
    enum NarcId narcId;

    narcId = TypeIconGfx_GetNarcId();
    SpriteSystem_ReplaceCharResObj(arg0->spriteSystem, arg0->spriteManager, narcId, TypeIconGfx_GetCharFileId(moveType), 1, resId);
    thunk_Sprite_SetPaletteOverride(arg0->sprites[arg1], TypeIconGfx_GetPlttOverride((u16)moveType) + 3);
}
