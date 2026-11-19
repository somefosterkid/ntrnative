#include "global.h"
struct BattleParty;

#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

void ov08_0221D0F4(BattleParty *battleParty);

void ov08_0221D0F4(BattleParty *battleParty) {
    battleParty->messageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 6, battleParty->context->heapID);
    battleParty->unused3 = MessagePrinter_New(0xFU, 0xEU, 0U, battleParty->context->heapID);
    battleParty->stringTemplate = MessageFormat_New(battleParty->context->heapID);
    battleParty->string = String_New(0x200U, battleParty->context->heapID);
}

void ov08_0221D14C(struct BattleParty *arg0);

void ov08_0221D14C(BattleParty *arg0) {
    DestroyMsgData(*((MsgData * *)&arg0->messageLoader));
    MessagePrinter_Delete(*((MessagePrinter * *)&arg0->unused3));
    MessageFormat_Delete(*((MessageFormat * *)&arg0->stringTemplate));
    String_Delete(arg0->string);
}
