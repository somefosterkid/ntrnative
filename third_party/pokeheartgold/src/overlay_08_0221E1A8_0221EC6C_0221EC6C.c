#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

void ov08_0221EC6C(BattleParty *battleParty, s32 partyIndex);

void ov08_0221EC6C(BattleParty *battleParty, s32 partyIndex) {
    MsgData *msgData;
    String *string;
    s32 windowOffset;

    windowOffset = battleParty->useAltSummaryWindows * 22;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2D2, battleParty->context->heapID);
    string = NewString_ReadMsgData(msgData, battleParty->partyPokemon[partyIndex].ability);
    AddTextPrinterParameterizedWithColor(&battleParty->windows[windowOffset + 2], FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0x10200U, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    ScheduleWindowCopyToVram(&battleParty->windows[windowOffset + 2]);
}
