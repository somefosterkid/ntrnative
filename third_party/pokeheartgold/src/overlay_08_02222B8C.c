#include "global.h"

#include "constants/battle.h"
#include "constants/items.h"
#include "constants/moves.h"

#include "battle/battle.h"
#include "battle/battle_bag_context.h"
#include "battle/battle_system.h"
#include "filesystem_files_def.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0040.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_08_battle_bag.h"

int ov08_02223374(BattleBag *battleBag);
void ov08_02223B20(BattleBag *battleBag);
void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);
u8 ov08_02222B8C(BattleBag *battleBag);
void ov08_02223300(BattleBag *battleBag, u8 screen);
u8 ov08_02222D78(BattleBag *battleBag);
u8 ov08_02222D84(BattleBag *battleBag);
u8 ov08_02222D90(BattleBag *battleBag);

u8 ov08_02222B8C(BattleBag *battleBag) {
    BattleBagContext *context = battleBag->context;

    if (battleBag->currentBattlePocket == BTLPOCKETLIST_BATTLE_ITEMS) {
        int partySlot = ov08_02223374(battleBag);
        u32 itemBattleUse = GetItemAttr(context->selectedBattleBagItem, ITEMATTR_BATTLEUSEFUNC, context->heapID);

        if (context->embargoRemainingTurns != 0 && context->selectedBattleBagItem != ITEM_GUARD_SPEC_ && itemBattleUse != 3) {
            Pokemon *mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, partySlot);
            String *string = NewString_ReadMsgData(battleBag->messageLoader, 46);

            BufferBoxMonNickname(battleBag->stringTemplate, 0, Mon_GetBoxMon(mon));
            BufferMoveName(battleBag->stringTemplate, 1, MOVE_EMBARGO);
            StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, string);
            String_Delete(string);

            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;

            return 9;
        }

        if (BattleSystem_RecoverStatus(context->battleSys, context->battler, partySlot, 0, context->selectedBattleBagItem) == 1) {
            ov08_02223390(context->battleSys, context->selectedBattleBagItem, battleBag->currentBattlePocket, context->heapID);
            return 13;
        } else if (itemBattleUse == 3) {
            if (!(BattleSystem_GetBattleType(context->battleSys) & BATTLE_TYPE_TRAINER)) {
                ov08_02223390(context->battleSys, context->selectedBattleBagItem, battleBag->currentBattlePocket, context->heapID);
                return 13;
            } else {
                MsgData *messageLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msg_0040, context->heapID);
                String *string = NewString_ReadMsgData(messageLoader, 37);
                BufferPlayersName(battleBag->stringTemplate, 0, context->trainerInfo);
                StringExpandPlaceholders(battleBag->stringTemplate, battleBag->string, string);
                String_Delete(string);
                DestroyMsgData(messageLoader);
                ov08_02223B20(battleBag);
                battleBag->queuedState = 8;
                return 9;
            }
        } else {
            ReadMsgDataIntoString(battleBag->messageLoader, 34, battleBag->string);
            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;
            return 9;
        }
    } else if (battleBag->currentBattlePocket == BTLPOCKETLIST_BALLS) {
        if (context->hasTwoOpponents == 1) {
            ReadMsgDataIntoString(battleBag->messageLoader, 44, battleBag->string);
            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;
            return 9;
        }

        if (context->opponentHidden == 1) {
            ReadMsgDataIntoString(battleBag->messageLoader, 47, battleBag->string);
            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;
            return 9;
        }

        if (context->opponentSubstituted == 1) {
            ReadMsgDataIntoString(battleBag->messageLoader, 48, battleBag->string);
            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;
            return 9;
        }

        Party *party = BattleSystem_GetParty(context->battleSys, context->battler);
        PCStorage *pcBoxes = BattleSystem_GetPcStorage(context->battleSys);

        if (Party_GetCount(party) == 6 && PCStorage_FindFirstBoxWithEmptySlot(pcBoxes) == 18) {
            ReadMsgDataIntoString(battleBag->messageLoader, 45, battleBag->string);
            ov08_02223B20(battleBag);
            battleBag->queuedState = 8;
            return 9;
        }
    }

    return 13;
}

u8 ov08_02222D78(BattleBag *battleBag) {
    ov08_02223300(battleBag, 0);
    return 1;
}

u8 ov08_02222D84(BattleBag *battleBag) {
    ov08_02223300(battleBag, 1);
    return 2;
}

u8 ov08_02222D90(BattleBag *battleBag) {
    ov08_02223300(battleBag, 2);
    return 3;
}
