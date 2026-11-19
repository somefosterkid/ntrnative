#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "battle/battle_cursor.h"
#include "battle/battle_system.h"

#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pokemon_types_def.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"

void ov08_02220A8C(ManagedSprite *sprite, const int x, const int y);

void ov08_022213E4(ManagedSprite *sprite, u8 animationID);

s16 ov08_022214BC(u32 statValue, s16 endCoord, s16 startCoord);

void ov08_02221500(BattleParty *battleParty);

void ov08_02221500(BattleParty *battleParty) {
    BattlePartyPokemon *pokemon = &battleParty->partyPokemon[battleParty->context->selectedPartyIndex];

    ov08_022213E4(battleParty->sprites[27], 0);
    ov08_022213E4(battleParty->sprites[28], 1);
    ov08_022213E4(battleParty->sprites[29], 3);
    ov08_022213E4(battleParty->sprites[30], 4);
    ov08_022213E4(battleParty->sprites[31], 2);

    ov08_02220A8C(battleParty->sprites[27], ov08_022214BC(pokemon->cool, 144, 144), ov08_022214BC(pokemon->cool, 2, 24));
    ov08_02220A8C(battleParty->sprites[28], ov08_022214BC(pokemon->beauty, 164, 144), ov08_022214BC(pokemon->beauty, 16, 24));
    ov08_02220A8C(battleParty->sprites[29], ov08_022214BC(pokemon->cute, 156, 144), ov08_022214BC(pokemon->cute, 41, 24));
    ov08_02220A8C(battleParty->sprites[30], ov08_022214BC(pokemon->smart, 131, 143), ov08_022214BC(pokemon->smart, 41, 24));
    ov08_02220A8C(battleParty->sprites[31], ov08_022214BC(pokemon->tough, 123, 143), ov08_022214BC(pokemon->tough, 16, 24));
}

s32 ov08_02224B94(void *, BattleCursor *);

void ov08_0222162C(BattleParty *battleParty);

void ov08_0222162C(BattleParty *battleParty) {
    SpriteSystem *renderer;

    renderer = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    BattleCursor_LoadResources(renderer, battleParty->spriteManager, battleParty->palette, battleParty->context->heapID, 0xB018U, 0xB00CU, 0xB00CU, 0xB00CU);
    ov08_02224B94(battleParty->cursor, BattleCursor_New(renderer, battleParty->spriteManager, battleParty->context->heapID, 0xB018U, 0xB00CU, 0xB00CU, 0xB00CU, 0U, 1U));
}

BattleCursor *ov08_02224B84(void *);

void ov08_02221698(BattleParty *arg0);

void ov08_02221698(BattleParty *arg0) {
    BattleCursor_Delete(ov08_02224B84(*((void **)&arg0->cursor)));
    BattleCursor_FreeResources(arg0->spriteManager, 0xB018U, 0xB00CU, 0xB00CU, 0xB00CU);
}

void ov08_02224BF8(BattleSubMenuCursor *cursor, u32 mask);

enum BattleLearnMoveScreenButton {
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

void ov08_022216CC(BattleParty *battleParty);

void ov08_022216CC(BattleParty *battleParty) {
    if (battleParty->hasVisitedContestHall == 0) {
        ov08_02224BF8(battleParty->cursor, (1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL));
    } else {
        ov08_02224BF8(battleParty->cursor, ((1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4 | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL) | 1 << BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS));
    }
}

enum BattleConfirmLearnMoveScreenButton {
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONFIRM = 0,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

void ov08_022216F4(BattleParty *battleParty);

void ov08_022216F4(BattleParty *battleParty) {
    if (battleParty->hasVisitedContestHall == 0) {
        ov08_02224BF8(battleParty->cursor, (1 << BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONFIRM | 1 << BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL));
    } else {
        ov08_02224BF8(battleParty->cursor, ((1 << BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONFIRM | 1 << BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL) | 1 << BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS));
    }
}
