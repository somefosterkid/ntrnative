#include "global.h"

#include "battle/battle_system.h"

#include "sprite_system.h"
#include "unk_02077678.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

void ov08_02220AEC(BattleParty *battleParty, ManagedSprite *managedSprite, s32 resId, u8 moveType);

void ov08_02220AEC(BattleParty *battleParty, ManagedSprite *managedSprite, s32 resId, u8 moveType) {
    enum NarcId narcId;
    SpriteSystem *spriteSystem;
    SpriteSystem *v0;

    v0 = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    spriteSystem = v0;
    narcId = TypeIconGfx_GetNarcId();
    SpriteSystem_ReplaceCharResObj(v0, battleParty->spriteManager, narcId, TypeIconGfx_GetCharFileId(moveType), 1, resId);
    ManagedSprite_SetPaletteOverride(managedSprite, TypeIconGfx_GetPlttOverride((u16)moveType) + 4);
}
