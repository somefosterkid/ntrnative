#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "move.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

void ov08_0221DDCC(BattleParty *battleParty, s32 param_2, enum Font param_3, u16 param_4, u8 param_5, u8 param_6);

void ov08_0221E244(BattleParty *battleParty, u32 param_2, s32 param_3, s32 param_4, u16 param_5, u16 param_6, u32 param_7);

void ov08_0221E2E8(BattleParty *battleParty, u16 param_2, u8 param_3, u8 param_4);

void ov08_0221F07C(BattleParty *battleParty, s32 param_2, s32 param_3, s32 param_4);

void ov08_0221F1B0(BattleParty *battleParty, s32 param_2);

extern s32 ov08_02224FE0[];

void ov08_02220084(BattleParty *battleParty);

void ov08_02220084(BattleParty *battleParty) {
    String *string;
    u32 pp;
    BattlePartyContext *context;
    u32 moveSlot;

    FillWindowPixelBuffer(&battleParty->windows[0], 0);
    FillWindowPixelBuffer(&battleParty->windows[2], 0);
    FillWindowPixelBuffer(&battleParty->windows[3], 0);
    FillWindowPixelBuffer(&battleParty->windows[1], 0);
    FillWindowPixelBuffer(&battleParty->windows[4], 0);
    FillWindowPixelBuffer(&battleParty->windows[5], 0);
    FillWindowPixelBuffer(&battleParty->windows[6], 0);
    ov08_0221DDCC(battleParty, 0, FONT_SYSTEM, battleParty->context->selectedPartyIndex, 0, 0);
    ov08_0221E2E8(battleParty, 2, 0, 0);
    string = NewString_ReadMsgData(battleParty->messageLoader, 0x39);
    AddTextPrinterParameterizedWithColor(&battleParty->windows[4], FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(&battleParty->windows[4]);
    context = battleParty->context;
    moveSlot = context->selectedMoveSlot;
    if (moveSlot < 4) {
        BattlePartyPokemonMove *move = &battleParty->partyPokemon[context->selectedPartyIndex].moves[moveSlot];
        ov08_0221E244(battleParty, move->move, 1, ov08_02224FE0[moveSlot], 0, 0, 0xf0e00);
        ov08_0221F07C(battleParty, 3, move->currentPP, move->maxPP);
    } else {
        pp = GetMoveAttr(context->moveToLearn, MOVEATTR_PP);
        ov08_0221E244(battleParty, battleParty->context->moveToLearn, 1, 0x49, 0, 0, 0xf0e00);
        ov08_0221F07C(battleParty, 3, pp, pp);
    }
    ov08_0221F1B0(battleParty, 6);
}
