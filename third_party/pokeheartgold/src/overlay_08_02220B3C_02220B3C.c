#include "global.h"

#include "battle/battle_system.h"

#include "sprite_system.h"
#include "unk_02077678.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *managedSprite, u16 moveCategory);

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *managedSprite, u16 moveCategory) {
    enum NarcId narcId;
    SpriteSystem *spriteSystem;

    spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    narcId = GetMoveCategoryIconGfxNarcId();
    SpriteSystem_ReplaceCharResObj(spriteSystem, battleParty->spriteManager, narcId, GetMoveCategoryIconGfxCharFileId(moveCategory), 1, 0xB015);
    ManagedSprite_SetPaletteOverride(managedSprite, GetMoveCategoryIconGfxPlttOverride(moveCategory) + 4);
}

enum SummaryStatus {
    SUMMARY_CONDITION_POKERUS = 0,
    SUMMARY_CONDITION_PARALYSIS,
    SUMMARY_CONDITION_FREEZE,
    SUMMARY_CONDITION_SLEEP,
    SUMMARY_CONDITION_POISON,
    SUMMARY_CONDITION_BURN,
    SUMMARY_CONDITION_FAINTED,
    SUMMARY_CONDITION_NONE,
};

void ov08_02220A8C(ManagedSprite *sprite, const int x, const int y);

void ov08_02220B90(enum SummaryStatus summaryStatus, ManagedSprite *sprite, const int x, const int y);

void ov08_02220B90(enum SummaryStatus summaryStatus, ManagedSprite *sprite, const int x, const int y) {
    if (summaryStatus == SUMMARY_CONDITION_NONE) {
        return;
    }

    ManagedSprite_SetAnim(sprite, summaryStatus);
    ov08_02220A8C(sprite, x, y);
}

struct BattlePartyContext;

struct BattleSubMenuCursor;

struct MessagePrinter;

struct MsgData;

struct MessageFormat;

typedef struct SpritePosition {
    int x;
    int y;
} SpritePosition;

void ov08_02220AEC(BattleParty *battleParty, ManagedSprite *sprite, u32 resourceID, u8 type);

void ov08_02220BB4(BattleParty *battleParty, BattlePartyPokemon *pokemon, const SpritePosition *coordinates);

void ov08_02220BB4(BattleParty *battleParty, BattlePartyPokemon *pokemon, const SpritePosition *coordinates) {
    ov08_02220AEC(battleParty, battleParty->sprites[19], 45070, pokemon->type_1);
    ov08_02220A8C(battleParty->sprites[19], coordinates[0].x, coordinates[0].y);

    if (pokemon->type_1 != pokemon->type_2) {
        ov08_02220AEC(battleParty, battleParty->sprites[20], 45070 + 1, pokemon->type_2);
        ov08_02220A8C(battleParty->sprites[20], coordinates[1].x, coordinates[1].y);
    }
}
