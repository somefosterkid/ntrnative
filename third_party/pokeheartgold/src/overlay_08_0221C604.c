#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "message_printer.h"
#include "palette.h"
#include "pm_string.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle.h"
#include "battle/battle_party_context.h"
#include "msgdata.h"
#include "battle/battle_cursor.h"
#include "overlay_55.h"

struct BattleSystem;


u32 ov08_02224C94(BattleSubMenuCursor *cursor);

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum BattleLearnMoveScreenButton {
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

void ov08_022220AC(BattleParty *battleParty, u8 button);

void ov08_022217C8(BattleParty *battleParty);

int ov08_0221D5D0(BattleParty *battleParty, const TouchscreenHitbox *rect);

extern const TouchscreenHitbox ov08_02224F3C[8];

u8 ov08_0221C604(BattleParty *battleParty);

s32 ov08_0221D840(void *, s8);
s32 ov08_0221C918(BattleParty *battleParty);
s32 ov08_0221C924(BattleParty *battleParty);
s32 ov08_0221C930(BattleParty *battleParty);

s32 ov08_0221C93C(BattleParty *battleParty);
s32 ov08_0221C948(BattleParty *battleParty);

u8 ov08_0221C604(BattleParty *battleParty) {
    int learnMoveScreenButtonPressed = ov08_0221D5D0(battleParty, ov08_02224F3C);

    if (learnMoveScreenButtonPressed == -1) {
        learnMoveScreenButtonPressed = ov08_02224C94(battleParty->cursor);

        if (learnMoveScreenButtonPressed == 0xfffffffe) {
            learnMoveScreenButtonPressed = BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL;
        }
    } else {
        ov08_022217C8(battleParty);
    }

    switch (learnMoveScreenButtonPressed) {
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_1:
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_2:
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_3:
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_4:
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_MOVE_TO_LEARN:
        battleParty->context->selectedMoveSlot = (u8)learnMoveScreenButtonPressed;
        battleParty->learnMovePreviousScreenButton = (u8)learnMoveScreenButtonPressed;
        PlaySE(1501);
        ov08_022220AC(battleParty, 23 + learnMoveScreenButtonPressed);
        battleParty->queuedState = 12;
        return 22;
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS:
        if (battleParty->hasVisitedContestHall == 0) {
            break;
        }

        battleParty->displayingContestStats ^= 1;
        battleParty->learnMovePreviousScreenButton = (u8)learnMoveScreenButtonPressed;
        PlaySE(1501);
        ov08_022220AC(battleParty, 18);
        battleParty->queuedState = 11;
        return 22;
    case BATTLE_LEARN_MOVE_SCREEN_BUTTON_CANCEL:
        battleParty->context->selectedMoveSlot = 4;
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->queuedState = 25;
        return 22;
    }

    return 19;
}

enum BattleConfirmLearnMoveScreenButton {
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONFIRM = 0,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS,
    BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL,
};

void ov08_0221F220(BattleParty *battleParty);

BOOL ov08_0221DB54(BattleParty *battleParty);

void ov08_0221DB7C(BattleParty *battleParty);

void ov08_0221DBB4(BattleParty *battleParty);

extern const TouchscreenHitbox ov08_02224E44[4];

u8 ov08_0221C6F8(BattleParty *battleParty);

u8 ov08_0221C6F8(BattleParty *battleParty) {
    int confirmLearnMovesScreenButtonPressed = ov08_0221D5D0(battleParty, ov08_02224E44);

    if (confirmLearnMovesScreenButtonPressed == -1) {
        confirmLearnMovesScreenButtonPressed = ov08_02224C94(battleParty->cursor);

        if (confirmLearnMovesScreenButtonPressed == 0xfffffffe) {
            confirmLearnMovesScreenButtonPressed = BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL;
        }
    } else {
        ov08_022217C8(battleParty);
    }

    switch (confirmLearnMovesScreenButtonPressed) {
    case BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONFIRM:
        PlaySE(1501);

        if (battleParty->displayingContestStats == 0) {
            ov08_022220AC(battleParty, 28);
        } else {
            ov08_022220AC(battleParty, 28 + 1);
        }

        if (ov08_0221DB54(battleParty) == 1) {
            ov08_0221F220(battleParty);

            if (battleParty->displayingContestStats == 0) {
                ov08_0221DB7C(battleParty);
            } else {
                ov08_0221DBB4(battleParty);
            }

            battleParty->queuedState = 20;
        } else {
            battleParty->queuedState = 25;
        }

        return 22;
    case BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CONTEST_STATS:
        if (battleParty->hasVisitedContestHall == 0) {
            break;
        }

        PlaySE(1501);
        ov08_022220AC(battleParty, 18);
        battleParty->displayingContestStats ^= 1;
        battleParty->confirmLearnMovePreviousScreenButton = (u8)confirmLearnMovesScreenButtonPressed;
        battleParty->queuedState = 12;
        return 22;
    case BATTLE_CONFIRM_LEARN_MOVE_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->confirmLearnMovePreviousScreenButton = 0;
        battleParty->queuedState = 11;
        return 22;
    }

    return 20;
}



enum BattleRestoreMovePPScreenButton {
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1 = 0,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4,
    BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_CANCEL,
};

BOOL BattleSystem_RecoverStatus(BattleSystem *battleSys, int battler, int partySlot, int moveSlot, int item);

void ov08_022201C0(BattleParty *battleParty);

enum BattlePartyUseAllMovePPRestorationItemState {
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_INTIALISING = 0,
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_RESTORING_PP,
    BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_FINISHING,
};

extern const TouchscreenHitbox ov08_02224E94[6];

u8 ov08_0221C814(BattleParty *battleParty);

u8 ov08_0221C814(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;
    int restoreMovePPScreenButtonPressed = ov08_0221D5D0(battleParty, ov08_02224E94);

    if (restoreMovePPScreenButtonPressed == -1) {
        restoreMovePPScreenButtonPressed = ov08_02224C94(battleParty->cursor);

        if (restoreMovePPScreenButtonPressed == 0xfffffffe) {
            restoreMovePPScreenButtonPressed = BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_CANCEL;
        }
    } else {
        ov08_022217C8(battleParty);
    }

    switch (restoreMovePPScreenButtonPressed) {
    case BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_1:
    case BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_2:
    case BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_3:
    case BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_MOVE_4:
        if (battleParty->partyPokemon[context->selectedPartyIndex].moves[restoreMovePPScreenButtonPressed].move == MOVE_NONE) {
            break;
        }

        battleParty->context->selectedMoveSlot = (u8)restoreMovePPScreenButtonPressed;
        PlaySE(1501);
        ov08_022220AC(battleParty, 19 + restoreMovePPScreenButtonPressed);

        if (BattleSystem_RecoverStatus(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex], restoreMovePPScreenButtonPressed, context->selectedBattleBagItem) == 1) {
            battleParty->useItemState = BATTLE_PARTY_USE_ALL_MOVE_PP_RESTORATION_ITEM_STATE_INTIALISING;
            battleParty->queuedState = 23;
            return 22;
        } else {
            ReadMsgDataIntoString(battleParty->messageLoader, 81, battleParty->string);
            ov08_022201C0(battleParty);
            battleParty->context->selectedPartyIndex = 6;
            battleParty->queuedState = 25;
            return 17;
        }
        break;
    case BATTLE_RESTORE_MOVE_PP_SCREEN_BUTTON_CANCEL:
        PlaySE(1501);
        ov08_022220AC(battleParty, 6);
        battleParty->queuedState = 6;
        return 22;
    }

    return 21;
}

s32 ov08_0221C918(BattleParty *battleParty) {
    ov08_0221D840(battleParty, 0);
    return 1;
}

s32 ov08_0221C924(BattleParty *battleParty) {
    ov08_0221D840(battleParty, 1);
    return 2;
}

s32 ov08_0221C930(BattleParty *battleParty) {
    ov08_0221D840(battleParty, 2);
    return 3;
}

s32 ov08_0221C93C(BattleParty *battleParty) {
    ov08_0221D840(battleParty, 3);
    return 4;
}

s32 ov08_0221C948(BattleParty *battleParty) {
    ov08_0221D840(battleParty, 4);
    return 5;
}
