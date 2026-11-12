// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov261/digitalcardcheck.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/digitalcardcheck.h"
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/event_fishing.h"

// The kinds of cards, which field 10 of the entry of the tournament block says
enum {
    CARD_TYPE_1 = 1,
    CARD_TYPE_2,
    CARD_TYPE_3,
    CARD_TYPE_4,
    CARD_TYPE_5,
};

// What the menu holds
enum {
    MENU_YES_NO,
    MENU_BACK,
    MENU_UNREGISTER_BACK,
};

// The three bytes of a digital player ID, the first in the high byte
static inline u32 DigitalCardCheck_PackBytes(u32 high, u32 middle, u32 low) {
    return (high << 24) | ((middle & 0xff) << 16) | ((low & 0xff) << 8);
}


void func_ov176_0217d458(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217d4fc(WifiBattleMatchTask *task, u32 *seq, void *work);
void *func_0200b204(void *block, u32 index);
Regulation *func_0200ae14(void *entry);
RecordSave *getRecordBlkAddress(SaveControl *save);
PlayerInfo *GetGameDataPlayerInfo(GameData *gameData);
ClActUnit *func_ov177_021e5548(WifiBattleMatchGraphic *net);
void *func_0200b074(void *entry);
void *func_0200ea00(RecordSave *record);
u16 func_0200dfd8(void *results, u32 index, u32 field);
u8 func_02008554(PlayerInfo *info);
u32 func_0200b078(void *entry, u32 field);
void *func_0200ea08(RecordSave *record);
u32 func_0200e918(void *results, u32 index);
u32 func_0200ae54(Regulation *regulation, u32 param);
SaveControl *GameData_GetSaveControl(GameData *gameData);
void *getTournamentBlockAddress(SaveControl *save);
BattleBoxSave *getBattleBox(SaveControl *save);
void *func_0200b1d4(void *block, u32 index);
void func_0200bbd8(BattleBoxSave *battleBox, u32 mask);
void func_0200b110(void *entry, u32 field, u32 value);
void func_02012dac(GameData *gameData);
int func_02012dd0(GameData *gameData);
void func_ov177_021e5ef8(WifiBattleMatchCard *card);
BOOL func_ov177_021e8844(WifiBattleMatchMsgBox *msgBox);
void func_ov176_0217d730(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217d964(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217da38(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217db0c(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217dbe0(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217dc20(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217dc28(DigitalCardCheck *check);
static void func_ov176_0217de60(DigitalCardCheck *check);
BOOL func_ov176_0217de8c(DigitalCardCheck *check);
BOOL func_ov176_0217de98(DigitalCardCheck *check);
void func_ov176_0217dea4(DigitalCardCheck *check, u32 page);
void func_ov176_0217dedc(DigitalCardCheck *check, u32 kind);
void func_ov176_0217dfa8(DigitalCardCheck *check);
s32 func_ov176_0217dfbc(DigitalCardCheck *check);
void func_ov176_0217dfd0(DigitalCardCheck *check, u32 msgId, u32 style);
BOOL func_ov176_0217dfe4(DigitalCardCheck *check);
void func_ov176_0217dff0(DigitalCardCheck *check, u32 enabled);
void func_ov176_0217e000(DigitalCardCheck *check);

// pokebw2's DigitalCardCheck_Update

void func_ov176_0217d410(DigitalCardCheck *check) {
    func_ov177_021e8a78(check->task);
    func_ov176_0217de60(check);
    GFL_WordSetSystemFree(check->wordSet);
    GFL_MsgDataFree(check->msgData);
    GFL_HeapFree(check);
}

void func_ov176_0217d434(DigitalCardCheck *check) {
    func_ov177_021e8a80(check->task);
    if (check->card != NULL) {
        func_ov177_021e64c0(check->card, check->param.printQueue);
    }
}

BOOL func_ov176_0217d44c(DigitalCardCheck *check) {
    return func_ov177_021e8a90(check->task);
}


// pokebw2's DigitalCardCheck_TaskStart
void func_ov176_0217d458(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    void *entry =
        func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(check->param.gameData)), check->param.mode);
    u32 field2 = func_0200b078(entry, 2);
    u32 cardType = func_0200b078(entry, 10);

    if (field2 != 0 && cardType == CARD_TYPE_1) {
        func_ov177_021e8aa0(task, func_ov176_0217d4fc);
    } else if (field2 != 0 && cardType == CARD_TYPE_2) {
        func_ov177_021e8aa0(task, func_ov176_0217d730);
    } else if (field2 != 0 && cardType == CARD_TYPE_3) {
        func_ov177_021e8aa0(task, func_ov176_0217d964);
    } else if (field2 != 0 && cardType == CARD_TYPE_4) {
        func_ov177_021e8aa0(task, func_ov176_0217da38);
    } else if (field2 != 0 && cardType == CARD_TYPE_5) {
        func_ov177_021e8aa0(task, func_ov176_0217db0c);
    } else {
        func_ov177_021e8aa0(task, func_ov176_0217dbe0);
    }
}

void func_ov176_0217d4fc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    SaveControl *save;
    void *entry;
    BattleBoxSave *box;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217dc28(check);
        func_ov176_0217dea4(check, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217de8c(check)) {
            *seq = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 3) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov176_0217e000(check);
        func_ov176_0217dff0(check, 1);
        func_ov176_0217dfd0(check, EventFishing_Text_103, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4:
        func_ov176_0217dedc(check, 2);
        *seq = 5;
        break;
    case 5:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 6;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 6:
        func_ov176_0217dfd0(check, EventFishing_Text_113, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 7);
        break;
    case 7:
        func_ov176_0217dedc(check, 0);
        *seq = 8;
        break;
    case 8:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 9;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 9:
        func_ov176_0217dfd0(check, EventFishing_Text_114, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 10);
        break;
    case 10:
        func_ov176_0217dedc(check, 0);
        *seq = 11;
        break;
    case 11:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 12;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 12:
        func_ov176_0217dfd0(check, EventFishing_Text_107, 2);
        *seq = 19;
        func_ov177_021e8ab4(task, 13);
        break;
    case 13:
        save = GameData_GetSaveControl(check->param.gameData);
        entry = func_0200b1d4(getTournamentBlockAddress(save), check->param.mode);
        box = getBattleBox(save);
        func_0200bbd8(box, check->param.mode == DIGITAL_CARD_CHECK_WIFI ? 1 : 2);
        func_0200b110(entry, 10, 4);
        *seq = 14;
        break;
    case 14:
        func_02012dac(check->param.gameData);
        *seq = 15;
        break;
    case 15:
        choice = func_02012dd0(check->param.gameData);
        if (choice == 2) {
            *seq = 16;
        } else if (choice == 3) {
            *seq = 16;
        }
        break;
    case 16:
        func_ov176_0217dfd0(check, EventFishing_Text_115, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 17);
        break;
    case 17:
        if (func_ov176_0217de98(check)) {
            *seq = 18;
        }
        break;
    case 18:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    case 19:
        if (func_ov176_0217dfe4(check)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

void func_ov176_0217d730(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    SaveControl *save;
    void *entry;
    BattleBoxSave *box;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217dc28(check);
        func_ov176_0217dea4(check, 0);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217de8c(check)) {
            *seq = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 3) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov176_0217e000(check);
        func_ov176_0217dff0(check, 1);
        func_ov176_0217dfd0(check, EventFishing_Text_104, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4:
        func_ov176_0217dedc(check, 2);
        *seq = 5;
        break;
    case 5:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 6;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 6:
        func_ov176_0217dfd0(check, EventFishing_Text_105, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 7);
        break;
    case 7:
        func_ov176_0217dedc(check, 0);
        *seq = 8;
        break;
    case 8:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 9;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 9:
        func_ov176_0217dfd0(check, EventFishing_Text_114, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 10);
        break;
    case 10:
        func_ov176_0217dedc(check, 0);
        *seq = 11;
        break;
    case 11:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 12;
        } else if (choice == 1) {
            *seq = 17;
        }
        break;
    case 12:
        func_ov176_0217dfd0(check, EventFishing_Text_107, 2);
        *seq = 19;
        func_ov177_021e8ab4(task, 13);
        break;
    case 13:
        save = GameData_GetSaveControl(check->param.gameData);
        entry = func_0200b1d4(getTournamentBlockAddress(save), check->param.mode);
        box = getBattleBox(save);
        func_0200bbd8(box, check->param.mode == DIGITAL_CARD_CHECK_WIFI ? 1 : 2);
        func_0200b110(entry, 10, 4);
        *seq = 14;
        break;
    case 14:
        func_02012dac(check->param.gameData);
        *seq = 15;
        break;
    case 15:
        choice = func_02012dd0(check->param.gameData);
        if (choice == 2) {
            *seq = 16;
        } else if (choice == 3) {
            *seq = 16;
        }
        break;
    case 16:
        func_ov176_0217dfd0(check, EventFishing_Text_108, 1);
        *seq = 19;
        func_ov177_021e8ab4(task, 17);
        break;
    case 17:
        if (func_ov176_0217de98(check)) {
            *seq = 18;
        }
        break;
    case 18:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    case 19:
        if (func_ov176_0217dfe4(check)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}



// pokebw2's DigitalCardCheck_TaskCardType3
void func_ov176_0217d964(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217dc28(check);
        func_ov176_0217dea4(check, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217de8c(check)) {
            *seq = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 3) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov176_0217e000(check);
        func_ov176_0217dff0(check, 1);
        func_ov176_0217dfd0(check, EventFishing_Text_109, 1);
        *seq = 4;
        break;
    case 4:
        if (func_ov176_0217dfe4(check)) {
            *seq = 5;
        }
        break;
    case 5:
        func_ov176_0217dedc(check, MENU_BACK);
        *seq = 6;
        break;
    case 6:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 7;
        }
        break;
    case 7:
        if (func_ov176_0217de98(check)) {
            *seq = 8;
        }
        break;
    case 8:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    }
}

// pokebw2's DigitalCardCheck_TaskCardType4
void func_ov176_0217da38(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217dc28(check);
        func_ov176_0217dea4(check, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217de8c(check)) {
            *seq = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 3) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov176_0217e000(check);
        func_ov176_0217dff0(check, 1);
        func_ov176_0217dfd0(check, EventFishing_Text_110, 1);
        *seq = 4;
        break;
    case 4:
        if (func_ov176_0217dfe4(check)) {
            *seq = 5;
        }
        break;
    case 5:
        func_ov176_0217dedc(check, MENU_BACK);
        *seq = 6;
        break;
    case 6:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 7;
        }
        break;
    case 7:
        if (func_ov176_0217de98(check)) {
            *seq = 8;
        }
        break;
    case 8:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    }
}

// pokebw2's DigitalCardCheck_TaskCardType5
void func_ov176_0217db0c(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;
    s32 choice;

    switch (*seq) {
    case 0:
        func_ov176_0217dc28(check);
        func_ov176_0217dea4(check, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217de8c(check)) {
            *seq = 2;
        }
        break;
    case 2:
        if (GCTX_HIDGetPressedKeys() & 3) {
            *seq = 3;
        }
        break;
    case 3:
        func_ov176_0217e000(check);
        func_ov176_0217dff0(check, 1);
        func_ov176_0217dfd0(check, EventFishing_Text_112, 1);
        *seq = 4;
        break;
    case 4:
        if (func_ov176_0217dfe4(check)) {
            *seq = 5;
        }
        break;
    case 5:
        func_ov176_0217dedc(check, MENU_BACK);
        *seq = 6;
        break;
    case 6:
        choice = func_ov176_0217dfbc(check);
        if (choice == -1) {
            break;
        }
        func_ov176_0217dfa8(check);
        if (choice == 0) {
            *seq = 7;
        }
        break;
    case 7:
        if (func_ov176_0217de98(check)) {
            *seq = 8;
        }
        break;
    case 8:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    }
}

// pokebw2's DigitalCardCheck_TaskNoCard
void func_ov176_0217dbe0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    DigitalCardCheck *check = work;

    switch (*seq) {
    case 0:
        func_ov176_0217dfd0(check, EventFishing_Text_111, 1);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217dfe4(check)) {
            *seq = 2;
        }
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov176_0217dc20);
        break;
    }
}

static void func_ov176_0217dc20(WifiBattleMatchTask *task, u32 *seq, void *work) {
    func_ov177_021e8aa8(task);
}

static void func_ov176_0217dc28(DigitalCardCheck *check) {
    SaveControl *save;
    void *tournament;
    void *entry;
    void *cardData;
    Regulation *regulation;
    RecordSave *record;
    PlayerInfo *playerInfo;
    ClActUnit *link;
    void *results;
    u32 f4, f5, f6, f7, f8, f9;

    if (check->card != NULL) {
        return;
    }
    save = GameData_GetSaveControl(check->param.gameData);
    tournament = getTournamentBlockAddress(GameData_GetSaveControl(check->param.gameData));
    entry = func_0200b1d4(tournament, check->param.mode);
    cardData = func_0200b204(tournament, check->param.mode);
    regulation = func_0200ae14(entry);
    record = getRecordBlkAddress(save);
    playerInfo = GetGameDataPlayerInfo(check->param.gameData);
    link = func_ov177_021e5548(check->param.net);
    if (check->param.mode == DIGITAL_CARD_CHECK_WIFI) {
        WifiBattleMatchWifiCardInfo info;

        MI_CpuFill8(&info, 0, sizeof(WifiBattleMatchWifiCardInfo));
        MI_CpuCopy8(func_0200b074(entry), &info, 0x92);
        results = func_0200ea00(record);
        info.unkA0 = func_0200dfd8(results, 10, 0);
        info.unkA4 = func_0200dfd8(results, 10, 1) + func_0200dfd8(results, 10, 2);
        info.look = func_02008554(playerInfo);
        f6 = func_0200b078(entry, 6);
        f5 = func_0200b078(entry, 5);
        f4 = func_0200b078(entry, 4);
        info.startDate = (f4 << 24) | ((f5 & 0xff) << 16) | ((f6 & 0xff) << 8);
        f9 = func_0200b078(entry, 9);
        f8 = func_0200b078(entry, 8);
        f7 = func_0200b078(entry, 7);
        info.endDate = (f7 << 24) | ((f8 & 0xff) << 16) | ((f9 & 0xff) << 8);
        info.variant = func_0200b078(entry, 11);
        check->card = func_ov177_021e5d68(&info, 0, playerInfo, link, check->param.seq, check->param.font, check->param.printQueue,
            check->msgData, check->wordSet, cardData, 1, check->heapId);
    } else {
        WifiBattleMatchLiveCardInfo info;

        MI_CpuFill8(&info, 0, sizeof(WifiBattleMatchLiveCardInfo));
        MI_CpuCopy8(func_0200b074(entry), &info, 0x92);
        results = func_0200ea08(record);
        info.unkA0 = func_0200e918(results, 1);
        info.unkA2 = func_0200e918(results, 2);
        info.unkA4 = func_0200e918(results, 0);
        info.unkA6 = func_0200ae54(regulation, 0x16);
        info.look = func_02008554(playerInfo);
        f6 = func_0200b078(entry, 6);
        f5 = func_0200b078(entry, 5);
        f4 = func_0200b078(entry, 4);
        info.startDate = (f4 << 24) | ((f5 & 0xff) << 16) | ((f6 & 0xff) << 8);
        f9 = func_0200b078(entry, 9);
        f8 = func_0200b078(entry, 8);
        f7 = func_0200b078(entry, 7);
        info.endDate = (f7 << 24) | ((f8 & 0xff) << 16) | ((f9 & 0xff) << 8);
        info.variant = func_0200b078(entry, 11);
        check->card = func_ov177_021e6118(&info, playerInfo, link, check->param.seq, check->param.font, check->param.printQueue,
            check->msgData, check->wordSet, cardData, 1, check->heapId);
    }
    func_ov176_0217dff0(check, 0);
}


static void func_ov176_0217de60(DigitalCardCheck *check) {
    if (check->card != NULL) {
        if (check->param.mode == DIGITAL_CARD_CHECK_WIFI) {
            func_ov177_021e5ef8(check->card);
        } else {
            func_ov177_021e62a0(check->card);
        }
        check->card = NULL;
    }
    GFL_BGSysClearScr(1);
    GFL_BGSysLoadScr(1);
}



// pokebw2's DigitalCardCheck_IsCardReady
BOOL func_ov176_0217de8c(DigitalCardCheck *check) {
    return func_ov177_021e64c8(check->card);
}

// pokebw2's DigitalCardCheck_IsCardDone
BOOL func_ov176_0217de98(DigitalCardCheck *check) {
    return func_ov177_021e65fc(check->card);
}

// pokebw2's DigitalCardCheck_ShowCard
void func_ov176_0217dea4(DigitalCardCheck *check, u32 page) {
    if (check->param.mode == DIGITAL_CARD_CHECK_WIFI) {
        func_ov177_021e5f24(check->card, page, check->msgData, check->param.printQueue, check->param.font,
                            check->heapId);
    } else {
        func_ov177_021e62cc(check->card, page, check->msgData, check->param.printQueue, check->param.font,
                            check->heapId);
    }
}

// pokebw2's DigitalCardCheck_OpenMenu
void func_ov176_0217dedc(DigitalCardCheck *check, u32 kind) {
    WifiBattleMatchMenuParam param;
    u32 layout;
    int x, y, width, height;

    if (check->menu != NULL) {
        return;
    }
    MI_CpuFill8(&param, 0, sizeof(WifiBattleMatchMenuParam));
    param.msgData = check->msgData;
    param.font = check->param.font;
    param.printQueue = check->param.printQueue;
    param.bg = 0;
    param.palette = 15;
    param.framePalette = 13;
    param.frameChar = 1;
    switch (kind) {
    case MENU_YES_NO:
        param.msgIds[0] = EventFishing_Text_118;
        param.msgIds[1] = EventFishing_Text_119;
        param.count = 2;
        param.canCancel = 1;
        param.cancelValue = 1;
        layout = 0;
        param.cursorRow = 1;
        break;
    case MENU_BACK:
        param.msgIds[0] = EventFishing_Text_116;
        param.count = 1;
        param.canCancel = 1;
        param.cancelValue = 0;
        layout = 0;
        break;
    case MENU_UNREGISTER_BACK:
        param.msgIds[0] = EventFishing_Text_117;
        param.msgIds[1] = EventFishing_Text_116;
        param.count = 2;
        param.canCancel = 1;
        param.cancelValue = 1;
        layout = 1;
        param.cursorRow = 1;
        break;
    }
    switch (layout) {
    case 0:
        width = 12;
        height = param.count * 2;
        x = 19;
        y = 17 - height;
        break;
    case 1:
        width = 15;
        height = param.count * 2;
        x = 16;
        y = 17 - height;
        break;
    }
    check->menu = func_ov177_021e88b0(&param, x, y, width, height, check->heapId);
}

// pokebw2's DigitalCardCheck_CloseMenu
void func_ov176_0217dfa8(DigitalCardCheck *check) {
    if (check->menu != NULL) {
        func_ov177_021e89d0(check->menu);
        check->menu = NULL;
    }
}

// pokebw2's DigitalCardCheck_GetMenuChoice
s32 func_ov176_0217dfbc(DigitalCardCheck *check) {
    if (check->menu != NULL) {
        return func_ov177_021e8a00(check->menu);
    }
    return -1;
}

// pokebw2's DigitalCardCheck_Print
void func_ov176_0217dfd0(DigitalCardCheck *check, u32 msgId, u32 style) {
    func_ov177_021e8738(check->param.msgBox, check->msgData, msgId, style);
}

BOOL func_ov176_0217dfe4(DigitalCardCheck *check) {
    return func_ov177_021e8844(check->param.msgBox);
}


// pokebw2's DigitalCardCheck_SetBGEnabled
void func_ov176_0217dff0(DigitalCardCheck *check, u32 enabled) {
    GFL_BGSysSetBGEnabled(0, enabled);
}

// pokebw2's DigitalCardCheck_ClearMessage
void func_ov176_0217e000(DigitalCardCheck *check) {
    func_ov177_021e8870(check->param.msgBox);
}
