// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov261/livebattlematch_flow.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/version.h"
#include "app/livebattlematch_flow.h"
#include "app/livebattlematch_irc.h"
#include "app/digitalcardcheck.h"
#include "app/wifibattlematch_util.h"
#include "battle/regulation.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/pms.h"
#include "system/wordset.h"
#include "nitro/os.h"

// A heap that the team is read into for a moment
#define HEAPID_LIVEBATTLEMATCH_TEAM HEAPID_TAIL(0x56)

// The steps of a flow that wait for a message to be printed
#define STEP_MESSAGE_QUEUE_NUM 4

void func_ov176_0217d174(LiveBattleMatchFlow *wk);
void func_ov176_0217d224(LiveBattleMatchFlow *wk);
void func_ov177_021e7df0(WifiBattleMatchLinkIcon *linkIcon);
void func_ov176_0217e058(LiveBattleMatchIrc *irc);
void func_020351a0(u8 flags);
ClActUnit *func_ov177_021e5548(WifiBattleMatchGraphic *graphic);
WifiBattleMatchTeamList *func_ov177_021e7e3c(BattleBoxSave *battleBox, ClActUnit *link, WifiBattleMatchRes *seq, Font *font, PrintQueue *printQueue, MsgData *msgCard, HeapID heapId);
u32 func_0200b078(void *entry, u32 field);
void func_ov176_0217d458(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217d160(LiveBattleMatchFlow *wk);
void func_ov176_0217d1a0(LiveBattleMatchFlow *wk, u32 arg);
BOOL func_ov176_0217d194(LiveBattleMatchFlow *wk);
u32 func_0200ba38(u8 flag);
void func_0200b110(void *entry, u32 field, u32 value);
void func_0200bbd8(BattleBoxSave *battleBox, u32 mask);
BOOL func_ov176_0217d188(LiveBattleMatchFlow *wk);
u32 func_ov176_0217d2a4(LiveBattleMatchFlow *wk);
BOOL func_ov176_0217d2b0(LiveBattleMatchFlow *wk);
void func_ov176_0217d290(LiveBattleMatchFlow *wk);
void func_ov176_0217d2bc(LiveBattleMatchFlow *wk, PokeParty *party);
u32 func_0201a090(Regulation *regulation, PokeParty *party, u32 *flags);
void func_0200b2a4(void *records, PokeParty *party);
void func_0200bbc8(BattleBoxSave *battleBox, u32 mask);
void func_ov176_0217d2f0(LiveBattleMatchPlayerData *playerData, LiveBattleMatchFlow *wk);
void func_ov176_0217e318(LiveBattleMatchIrc *irc, void *buffer);
int func_ov176_0217e380(LiveBattleMatchIrc *irc);
void func_ov176_0217e40c(LiveBattleMatchIrc *irc);
BOOL func_ov176_0217e414(LiveBattleMatchIrc *irc);
BOOL func_0200b1a8(void *entry);
void func_0200e904(void *record);
void func_0200b1f8(void *records);
BOOL func_0200bbfc(BattleBoxSave *battleBox, u16 flags);
void clearVSPlayer(void);
Regulation *func_0200ae14(void *entry);
void *func_0200b204(void *block, u32 index);
u32 func_0200e918(void *results, u32 index);
u32 func_0200ae54(Regulation *regulation, u32 param);
u8 func_02008554(PlayerInfo *info);
void *func_0200b074(void *entry);
void *getTournamentBlockAddress(SaveControl *save);
PlayerInfo *GetGameDataPlayerInfo(GameData *gameData);

struct LiveBattleMatchFlow {
    WifiBattleMatchTask *task;
    WifiBattleMatchMenu *menu;
    u32 menuKind;
    LiveBattleMatchIrc *irc;
    WifiBattleMatchCard *playerCard;
    WifiBattleMatchPartnerCard *partnerCard;
    WifiBattleMatchTeamList *teamList;
    WifiBattleMatchLinkIcon *linkIcon;
    MsgData *msgMain;
    MsgData *msgCard;
    Font *font;
    WordSet *wordSet;
    PokeParty *party;
    void *record;
    void *entry;
    // The competition data that the infrared delivery brings
    u8 delivered[0x160];
    u32 result;
    LiveBattleMatchFlowParam param;
    HeapID heapId;
    u32 frames;
    BOOL hasVideo;
    BOOL readySent;
    BOOL readyReceived;
    u32 pickFlags;
    u32 pickIndex;
};

void func_ov176_0217bb58(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217bbd4(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217bbdc(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217bff8(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217c2ec(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217c628(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217cae0(WifiBattleMatchTask *task, u32 *seq, void *work);
static void func_ov176_0217cc78(WifiBattleMatchTask *task, u32 *seq, void *work);
void func_ov176_0217ceac(LiveBattleMatchFlow *wk, u32 kind);
void func_ov176_0217cfd0(LiveBattleMatchFlow *wk);
s32 func_ov176_0217cfdc(LiveBattleMatchFlow *wk);
void func_ov176_0217cfe8(LiveBattleMatchFlow *wk, u32 msgId, u32 type);
BOOL func_ov176_0217d000(LiveBattleMatchFlow *wk);
void func_ov176_0217d010(LiveBattleMatchFlow *wk, u32 result);
void func_ov176_0217d028(LiveBattleMatchFlow *wk);
void func_ov176_0217d1a0(LiveBattleMatchFlow *wk, u32 a1);
void func_ov176_0217d1c8(LiveBattleMatchFlow *wk, LiveBattleMatchPlayerData *data);
BOOL func_ov176_0217d238(LiveBattleMatchFlow *wk);
void func_ov176_0217d244(LiveBattleMatchFlow *wk);
void func_ov176_0217d2bc(LiveBattleMatchFlow *wk, PokeParty *dest);
void func_ov176_0217d2f0(LiveBattleMatchPlayerData *data, LiveBattleMatchFlow *wk);
void func_ov176_0217d388(LiveBattleMatchPlayerData *data, LiveBattleMatchFlow *wk);

// pokebw2's LiveBattleMatchFlow_Main


LiveBattleMatchFlow *func_ov176_0217b984(const LiveBattleMatchFlowParam *param, HeapID heapId) {
    LiveBattleMatchFlow *wk;
    ClActUnit *unit;
    SaveControl *save;

    wk = GFL_HeapAllocate(heapId, 0x1dc, FALSE, "livebattlematch_flow.c", 0x109);
    MI_CpuFill8(wk, 0, 0x1dc);
    wk->param = *param;
    wk->heapId = heapId;
    wk->hasVideo = func_0200b45c();
    func_ov177_021e58a4(wk->param.seq, 1, heapId);
    wk->wordSet = GFL_WordSetSystemCreate(8, 0x40, heapId);
    wk->msgMain = GFL_MsgSysLoadData(FALSE, 2, 0x3a, heapId);
    wk->msgCard = GFL_MsgSysLoadData(FALSE, 2, 0xd2, heapId);
    wk->font = GFL_FontCreate(0x17, 3, 0, 0, heapId);
    wk->task = func_ov177_021e8a3c(wk, func_ov176_0217bb58, heapId);
    wk->irc = func_ov176_0217e00c(wk->param.gameData, heapId);
    unit = func_ov177_021e5548(wk->param.net);
    wk->linkIcon = func_ov177_021e7d20(unit, wk->param.seq, heapId);
    func_ov177_021e7e0c(wk->linkIcon, FALSE);
    save = GameData_GetSaveControl(wk->param.gameData);
    wk->entry = func_0200b1d4(getTournamentBlockAddress(save), 1);
    save = GameData_GetSaveControl(wk->param.gameData);
    wk->record = func_0200ea08(getRecordBlkAddress(save));
    wk->party = PokeParty_Create(heapId);
    if (wk->param.mode == 0) {
        func_0203516c(8);
    }
    return wk;
}

void func_ov176_0217ba9c(LiveBattleMatchFlow *wk) {
    GFL_HeapFree(wk->party);
    if (wk->result == 1) {
        func_020351a0(8);
    }
    func_ov176_0217d174(wk);
    func_ov176_0217d224(wk);
    func_ov177_021e7df0(wk->linkIcon);
    func_ov176_0217e058(wk->irc);
    func_ov177_021e8a78(wk->task);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgCard);
    GFL_MsgDataFree(wk->msgMain);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_HeapFree(wk);
}

void func_ov176_0217baf4(LiveBattleMatchFlow *wk) {
    func_ov177_021e8a80(wk->task);
    func_ov176_0217e074(wk->irc);
    func_ov177_021e7e08(wk->linkIcon);
    if (wk->playerCard != NULL) {
        func_ov177_021e64c0(wk->playerCard, wk->param.printQueue);
    }
    if (wk->partnerCard != NULL) {
        func_ov177_021e7774(wk->partnerCard, wk->param.printQueue);
    }
    if (wk->teamList != NULL) {
        func_ov177_021e84f8(wk->teamList, wk->param.printQueue);
    }
}

// pokebw2's LiveBattleMatchFlow_GetResult
u32 func_ov176_0217bb3c(LiveBattleMatchFlow *wk) {
    if (func_ov177_021e8a90(wk->task)) {
        return wk->result;
    }
    return 0;
}

void func_ov176_0217bb58(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    switch (wk->param.mode) {
    case 0:
        func_ov177_021e8aa0(task, func_ov176_0217bbdc);
        break;
    case 1:
        if (func_0200b078(wk->entry, 10) == 3) {
            func_ov177_021e8aa0(task, func_ov176_0217cc78);
        } else {
            func_ov177_021e8aa0(task, func_ov176_0217c2ec);
        }
        break;
    case 2:
        func_ov177_021e8aa0(task, func_ov176_0217cae0);
        break;
    case 3:
        func_ov177_021e8aa0(task, func_ov176_0217cc78);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
}


void func_ov176_0217bbd4(WifiBattleMatchTask *task, u32 *seq, void *work) {
    func_ov177_021e8aa8(task);
}

static void func_ov176_0217bbdc(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    switch (*seq) {
    case 0:
        switch (func_0200b078(wk->entry, 10)) {
        case 0:
        case 3:
        case 4:
        case 5:
            if (func_0200b078(wk->entry, 2) == 0) {
                *seq = 12;
            } else {
                *seq = 1;
            }
            break;
        case 1:
            func_ov177_021e8aa0(task, func_ov176_0217bff8);
            break;
        case 2:
            func_ov177_021e8aa0(task, func_ov176_0217c2ec);
            break;
        }
        break;
    case 1:
        func_ov176_0217d028(wk);
        func_ov176_0217d1a0(wk, 1);
        *seq = 2;
        break;
    case 2:
        if (func_ov176_0217d188(wk)) {
            *seq = 3;
        }
        break;
    case 3: {
        SaveControl *save = GameData_GetSaveControl(wk->param.gameData);

        if (func_0200bbfc(getBattleBox(save), 2)) {
            *seq = 5;
        } else {
            *seq = 4;
        }
        break;
    }
    case 4:
        func_ov176_0217cfe8(wk, 0, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 6);
        break;
    case 5:
        func_ov176_0217cfe8(wk, 1, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6:
        func_ov176_0217ceac(wk, 0);
        *seq = 7;
        break;
    case 7: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 8;
            break;
        case 1:
            *seq = 10;
            break;
        }
        break;
    }
    case 8:
        if (func_ov176_0217d194(wk)) {
            *seq = 9;
        }
        break;
    case 9:
        func_ov176_0217d174(wk);
        *seq = 12;
        break;
    case 10:
        if (func_ov176_0217d194(wk)) {
            *seq = 11;
        }
        break;
    case 11:
        func_ov176_0217d174(wk);
        func_ov176_0217d010(wk, 1);
        break;
    case 12:
        func_ov176_0217cfe8(wk, 2, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 13);
        break;
    case 13:
        func_ov177_021e7e0c(wk->linkIcon, TRUE);
        func_ov176_0217cfe8(wk, 0x20, 0);
        *seq = 31;
        func_ov177_021e8ab4(task, 14);
        break;
    case 14:
        func_ov176_0217e318(wk->irc, wk->delivered);
        *seq = 15;
        break;
    case 15: {
        int status = func_ov176_0217e380(wk->irc);

        if (status != 0) {
            func_ov177_021e7e0c(wk->linkIcon, FALSE);
            if (status == 1) {
                *seq = 21;
            } else {
                *seq = 26;
            }
        }
        if (func_ov176_0217e078(wk->irc) == 1) {
            func_ov177_021e7e0c(wk->linkIcon, FALSE);
            *seq = 12;
            break;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            *seq = 16;
        }
        break;
    }
    case 16:
        func_ov176_0217e40c(wk->irc);
        *seq = 17;
        break;
    case 17:
        if (func_ov176_0217e414(wk->irc)) {
            *seq = 18;
        }
        break;
    case 18:
        func_ov177_021e7e0c(wk->linkIcon, FALSE);
        func_ov176_0217cfe8(wk, 0x15, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x13);
        break;
    case 19:
        func_ov176_0217ceac(wk, 0);
        *seq = 20;
        break;
    case 20: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 30;
            break;
        case 1:
            *seq = 12;
            break;
        }
        break;
    }
    case 21:
        if (func_0200b1a8(wk->delivered)) {
            if (func_0200b078(wk->delivered, 1) & (1 << GAME_VERSION)) {
                if (func_0200b078(wk->entry, 2) == func_0200b078(wk->delivered, 2)) {
                    switch (func_0200b078(wk->entry, 10)) {
                    case 4:
                        *seq = 27;
                        break;
                    case 3:
                        *seq = 28;
                        break;
                    default:
                        *seq = 22;
                        break;
                    }
                } else {
                    *seq = 22;
                }
            } else {
                *seq = 26;
            }
        } else {
            *seq = 25;
        }
        break;
    case 22:
        clearVSPlayer();
        func_ov176_0217cfe8(wk, 10, 2);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x17);
        break;
    case 23: {
        void *block = getTournamentBlockAddress(GameData_GetSaveControl(wk->param.gameData));
        void *records = func_0200b204(block, 1);

        func_0200b110(wk->delivered, 10, 1);
        func_0200e904(wk->record);
        func_0200b1f8(records);
        func_0200b1e0(block, 1, wk->delivered);
        func_02012dac(wk->param.gameData);
        *seq = 24;
        break;
    }
    case 24:
        if (func_02012dd0(wk->param.gameData) == 2) {
            *seq = 29;
        }
        break;
    case 25:
        func_ov176_0217cfe8(wk, 0x1a, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x1e);
        break;
    case 26:
        func_ov176_0217cfe8(wk, 0x1b, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x1e);
        break;
    case 27:
        func_ov176_0217cfe8(wk, 3, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x1e);
        break;
    case 28:
        func_ov176_0217cfe8(wk, 4, 1);
        *seq = 31;
        func_ov177_021e8ab4(task, 0x1e);
        break;
    case 29:
        func_ov177_021e8aa0(task, func_ov176_0217bff8);
        break;
    case 30:
        func_ov176_0217d010(wk, 1);
        break;
    case 31:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov176_0217bff8(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    switch (*seq) {
    case 0:
        func_ov176_0217d028(wk);
        func_ov176_0217d1a0(wk, 2);
        func_ov176_0217d244(wk);
        *seq = 1;
        break;
    case 1: {
        BOOL ready = TRUE;

        ready &= func_ov176_0217d188(wk);
        if (func_ov176_0217d2a4(wk) & ready) {
            *seq = 2;
        }
        break;
    }
    case 2:
        func_ov176_0217cfe8(wk, 5, 1);
        *seq = 21;
        func_ov177_021e8ab4(task, 3);
        break;
    case 3:
        func_ov176_0217ceac(wk, 0);
        *seq = 4;
        break;
    case 4: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 8;
            break;
        case 1:
            *seq = 5;
            break;
        }
        break;
    }
    case 5:
        func_ov176_0217cfe8(wk, 8, 1);
        *seq = 21;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6:
        func_ov176_0217ceac(wk, 0);
        *seq = 7;
        break;
    case 7: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 19;
            break;
        case 1:
            *seq = 2;
            break;
        }
        break;
    }
    case 8:
        if (func_ov176_0217d2b0(wk) == FALSE) {
            break;
        }
        func_ov176_0217d290(wk);
        *seq = 9;
        break;
    case 9: {
        u32 flags = 0;
        Regulation *regulation = func_0200ae14(wk->entry);

        func_ov176_0217d2bc(wk, wk->party);
        if (howManyPokesAreAbleToFight(wk->party) == 0) {
            *seq = 11;
            break;
        }
        if (func_0201a090(regulation, wk->party, &flags) == 0) {
            *seq = 13;
        } else {
            *seq = 10;
        }
        break;
    }
    case 10:
        func_ov176_0217cfe8(wk, 9, 1);
        *seq = 21;
        func_ov177_021e8ab4(task, 12);
        break;
    case 11:
        func_ov176_0217cfe8(wk, 0x25, 1);
        *seq = 21;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        if (func_ov176_0217d194(wk)) {
            func_ov176_0217d174(wk);
            *seq = 20;
        }
        break;
    case 13: {
        SaveControl *save = GameData_GetSaveControl(wk->param.gameData);
        BattleBoxSave *battleBox = getBattleBox(save);
        void *records = func_0200b204(getTournamentBlockAddress(save), 1);
        PokeParty *party = convertBoxedPokeSetToParty(battleBox, HEAPID_LIVEBATTLEMATCH_TEAM);

        func_0200bbc8(battleBox, 2);
        func_0200b110(wk->entry, 10, 2);
        func_0200b2a4(records, party);
        GFL_HeapFree(party);
        *seq = 14;
        break;
    }
    case 14:
        func_ov176_0217cfe8(wk, 10, 2);
        *seq = 21;
        func_ov177_021e8ab4(task, 15);
        break;
    case 15:
        func_02012dac(wk->param.gameData);
        *seq = 16;
        break;
    case 16:
        if (func_02012dd0(wk->param.gameData) == 2) {
            *seq = 17;
        }
        break;
    case 17:
        func_ov176_0217d1a0(wk, 0);
        func_ov176_0217cfe8(wk, 0xb, 1);
        *seq = 21;
        func_ov177_021e8ab4(task, 18);
        break;
    case 18:
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
        break;
    case 19: {
        BOOL done = TRUE;

        done &= func_ov176_0217d194(wk);
        if (func_ov176_0217d2b0(wk) & done) {
            func_ov176_0217d174(wk);
            func_ov176_0217d290(wk);
            *seq = 20;
        }
        break;
    }
    case 20:
        func_ov176_0217d010(wk, 1);
        break;
    case 21:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

static void func_ov176_0217c2ec(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    switch (*seq) {
    case 0:
        func_ov176_0217d2f0(wk->param.playerData, wk);
        if (wk->playerCard != NULL) {
            *seq = 4;
            break;
        }
        func_ov176_0217d028(wk);
        func_ov176_0217d1a0(wk, 0);
        *seq = 1;
        break;
    case 1:
        if (func_ov176_0217d188(wk)) {
            *seq = 2;
        }
        break;
    case 2:
        if (wk->param.matchResult->disconnected) {
            *seq = 3;
        } else {
            *seq = 4;
        }
        break;
    case 3:
        func_ov176_0217cfe8(wk, 0x27, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 4);
        break;
    case 4: {
        Regulation *regulation = func_0200ae14(wk->entry);
        u32 played = func_0200e918(wk->record, 0);

        if (played >= func_0200ae54(regulation, 0x16)) {
            func_ov177_021e8aa0(task, func_ov176_0217cc78);
        } else {
            *seq = 5;
        }
        break;
    }
    case 5:
        func_ov176_0217cfe8(wk, 0xc, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6:
        func_ov176_0217ceac(wk, 1);
        *seq = 7;
        break;
    case 7: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        if (wk->hasVideo && !wk->param.matchResult->disconnected) {
            switch (selection) {
            case 0:
                *seq = 23;
                break;
            case 1:
                *seq = 19;
                break;
            case 2:
                *seq = 8;
                break;
            }
        } else {
            switch (selection) {
            case 0:
                *seq = 23;
                break;
            case 1:
                *seq = 8;
                break;
            }
        }
        break;
    }
    case 8:
        func_ov176_0217cfe8(wk, 0x11, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 9);
        break;
    case 9:
        func_ov176_0217ceac(wk, 3);
        *seq = 10;
        break;
    case 10: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 11;
            break;
        case 1:
            *seq = 5;
            break;
        }
        break;
    }
    case 11:
        func_ov176_0217cfe8(wk, 0x12, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        func_ov176_0217ceac(wk, 3);
        *seq = 13;
        break;
    case 13: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            *seq = 14;
            break;
        case 1:
            *seq = 5;
            break;
        }
        break;
    }
    case 14:
        func_ov176_0217cfe8(wk, 10, 2);
        *seq = 25;
        func_ov177_021e8ab4(task, 15);
        break;
    case 15:
        func_0200b110(wk->entry, 10, 4);
        func_0200bbd8(getBattleBox(GameData_GetSaveControl(wk->param.gameData)), 2);
        func_02012dac(wk->param.gameData);
        *seq = 16;
        break;
    case 16:
        switch (func_02012dd0(wk->param.gameData)) {
        case 0:
        case 1:
            break;
        case 2:
        case 3:
            *seq = 17;
            break;
        }
        break;
    case 17:
        func_ov176_0217d1a0(wk, 1);
        func_ov176_0217cfe8(wk, 0x13, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 0x12);
        break;
    case 18:
        if (func_ov176_0217d194(wk)) {
            func_ov176_0217d174(wk);
            *seq = 24;
        }
        break;
    case 19:
        if (func_0200ba38((u8)wk->param.partnerData->unk44)) {
            *seq = 20;
        } else {
            *seq = 21;
        }
        break;
    case 20:
        func_ov176_0217cfe8(wk, 0x10, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 0x16);
        break;
    case 21:
        func_ov176_0217cfe8(wk, 0x22, 1);
        *seq = 25;
        func_ov177_021e8ab4(task, 5);
        break;
    case 22:
        func_ov176_0217d010(wk, 4);
        break;
    case 23:
        func_ov177_021e8aa0(task, func_ov176_0217c628);
        break;
    case 24:
        func_ov176_0217d010(wk, 1);
        break;
    case 25:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

static void func_ov176_0217c628(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    if (func_ov176_0217e078(wk->irc) == 1) {
        wk->param.matchResult->disconnected = 1;
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
    }
    switch (*seq) {
    case 0:
        func_ov176_0217cfe8(wk, 0x14, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 1);
        break;
    case 1:
        func_ov176_0217cfe8(wk, 0x21, 0);
        func_ov177_021e7e0c(wk->linkIcon, TRUE);
        GFL_SndSEPlay(SEQ_SE_SYS_102);
        wk->readySent = FALSE;
        wk->readyReceived = FALSE;
        *seq = 40;
        func_ov177_021e8ab4(task, 2);
        break;
    case 2:
        if (func_ov176_0217e0a8(wk->irc)) {
            *seq = 3;
        }
        break;
    case 3:
        switch (func_ov176_0217e0b0(wk->irc)) {
        case 1:
            *seq = 13;
            break;
        case 2:
            *seq = 9;
            break;
        }
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            *seq = 4;
        }
        break;
    case 4:
        func_ov176_0217e158(wk->irc);
        *seq = 5;
        break;
    case 5:
        if (func_ov176_0217e178(wk->irc)) {
            *seq = 6;
        }
        break;
    case 6:
        func_ov177_021e7e0c(wk->linkIcon, FALSE);
        func_ov176_0217cfe8(wk, 0x15, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 7);
        break;
    case 7:
        func_ov176_0217ceac(wk, 0);
        *seq = 8;
        break;
    case 8: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        switch (selection) {
        case 0:
            func_ov177_021e7e0c(wk->linkIcon, FALSE);
            func_ov177_021e8aa0(task, func_ov176_0217c2ec);
            break;
        case 1:
            *seq = 1;
            break;
        }
        break;
    }
    case 9:
        func_ov177_021e7e0c(wk->linkIcon, FALSE);
        func_ov176_0217e158(wk->irc);
        *seq = 10;
        break;
    case 10:
        if (func_ov176_0217e178(wk->irc)) {
            *seq = 11;
        }
        break;
    case 11:
        func_ov176_0217cfe8(wk, 0x1c, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 12);
        break;
    case 12:
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
        break;
    case 13:
        func_ov176_0217d388(wk->param.playerData, wk);
        if (func_ov176_0217e1fc(wk->irc, wk->param.playerData)) {
            *seq = 14;
        }
        break;
    case 14: {
        void *received;

        if (func_ov176_0217e23c(wk->irc, &received)) {
            MI_CpuCopy8(received, wk->param.partnerData, PokeParty_GetSaveDataSize() + LIVEBATTLEMATCH_PLAYER_HEADER_SIZE);
            *seq = 15;
        }
        break;
    }
    case 15:
        if (wk->param.playerData->entryField2 == wk->param.partnerData->entryField2) {
            u32 field12 = func_0200b078(wk->entry, 12);
            BOOL known = func_0200e994(wk->record, wk->param.partnerData->mac);

            wk->readySent = (field12 != 0 || (field12 == 0 && !known));
            *seq = 16;
        } else {
            func_ov177_021e7e0c(wk->linkIcon, FALSE);
            GFL_SndStop();
            *seq = 21;
        }
        break;
    case 16: {
        NetHandle *handle = func_020399a8();

        func_02039c68(handle, 0x48, 0x1f);
        *seq = 17;
        break;
    }
    case 17:
        if (func_02039cb4(func_020399a8(), 0x48, 0x1f)) {
            *seq = 18;
        }
        break;
    case 18:
        if (func_ov176_0217e2c0(wk->irc, &wk->readySent)) {
            *seq = 19;
        }
        break;
    case 19:
        if (func_ov176_0217e2f8(wk->irc, &wk->readyReceived)) {
            *seq = 20;
        }
        break;
    case 20:
        func_ov177_021e7e0c(wk->linkIcon, FALSE);
        GFL_SndStop();
        if (wk->readyReceived != 0 && wk->readySent != 0) {
            GFL_SndSEPlay(SEQ_SE_SYS_74);
            *seq = 27;
        } else {
            *seq = 24;
        }
        break;
    case 21:
        if (func_ov176_0217e1b8(wk->irc)) {
            *seq = 22;
        }
        break;
    case 22:
        if (func_ov176_0217e1c0(wk->irc)) {
            *seq = 23;
        }
        break;
    case 23:
        func_ov176_0217cfe8(wk, 0x16, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x27);
        break;
    case 24:
        if (func_ov176_0217e1b8(wk->irc)) {
            *seq = 25;
        }
        break;
    case 25:
        if (func_ov176_0217e1c0(wk->irc)) {
            *seq = 26;
        }
        break;
    case 26:
        func_ov176_0217cfe8(wk, 0x24, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x27);
        break;
    case 27:
        func_ov176_0217cfe8(wk, 0x17, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x1c);
        break;
    case 28:
        if (wk->frames++ > 0x78) {
            wk->frames = 0;
            GFL_BGSysSetBGEnabled(0, FALSE);
            *seq = 29;
        }
        break;
    case 29:
        func_ov176_0217d1c8(wk, wk->param.partnerData);
        *seq = 30;
        break;
    case 30:
        if (func_ov176_0217d238(wk)) {
            *seq = 31;
        }
        break;
    case 31:
        if (wk->frames++ > 300) {
            wk->frames = 0;
            *seq = 32;
        }
        break;
    case 32:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov176_0217cfe8(wk, 0x18, 1);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x21);
        break;
    case 33:
        func_ov176_0217cfe8(wk, 0x23, 2);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x22);
        break;
    case 34: {
        NetHandle *handle = func_020399a8();

        func_02039c68(handle, 0x47, 0x1f);
        *seq = 35;
        break;
    }
    case 35:
        if (func_02039cb4(func_020399a8(), 0x47, 0x1f)) {
            *seq = 36;
        }
        break;
    case 36:
        GFL_BGSysSetBGEnabled(0, TRUE);
        func_ov176_0217cfe8(wk, 0x26, 2);
        *seq = 40;
        func_ov177_021e8ab4(task, 0x25);
        break;
    case 37:
        if (wk->frames++ > 60) {
            Regulation *regulation = func_0200ae14(wk->entry);

            wk->frames = 0;
            func_0200ae54(regulation, 0x13);
            *seq = 38;
        }
        break;
    case 38:
        func_ov176_0217d010(wk, 2);
        break;
    case 39:
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
        break;
    case 40:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


static void func_ov176_0217cae0(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    if (func_ov176_0217e078(wk->irc) == TRUE) {
        wk->param.matchResult->disconnected = TRUE;
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
    }
    switch (*seq) {
    case 0: {
        RivalDataSave *rival = getHollow_RivalData(GameData_GetSaveControl(wk->param.gameData));

        func_0200ed54(rival, getIDAsUInt(&wk->param.partnerData->info));
        *seq = 1;
        break;
    }
    case 1:
        if (func_ov176_0217e1b8(wk->irc)) {
            *seq = 2;
        }
        break;
    case 2:
        if (func_ov176_0217e1c0(wk->irc)) {
            *seq = 3;
        }
        break;
    case 3: {
        u8 played;
        u32 won = 0;
        u32 lost;

        played = func_0200e918(wk->record, 0);
        func_0200e958(wk->record, played, wk->param.partnerData->mac);
        func_0200e978(wk->record, played, 0, wk->param.matchResult->unk08);
        func_0200e978(wk->record, played, 1, wk->param.matchResult->unk09);
        lost = 0;
        switch (wk->param.matchResult->result) {
        case 1:
        case 4:
            won = 1;
            break;
        case 0:
        case 3:
            lost = 1;
            break;
        case 2:
            break;
        default:
            GFL_ASSERT(0);
            break;
        }
        func_0200e920(wk->record, 1, won);
        func_0200e920(wk->record, 2, lost);
        func_0200e920(wk->record, 0, 1);
        func_ov176_0217d160(wk);
        *seq = 4;
        break;
    }
    case 4:
        func_ov176_0217cfe8(wk, 10, 2);
        *seq = 8;
        func_ov177_021e8ab4(task, 5);
        break;
    case 5:
        func_02012dac(wk->param.gameData);
        *seq = 6;
        break;
    case 6:
        if (func_02012dd0(wk->param.gameData) == 2) {
            *seq = 7;
        }
        break;
    case 7:
        func_ov176_0217d010(wk, 3);
        break;
    case 8:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}

static void func_ov176_0217cc78(WifiBattleMatchTask *task, u32 *seq, void *work) {
    LiveBattleMatchFlow *wk = work;

    switch (*seq) {
    case 0:
        func_ov176_0217d160(wk);
        if (func_0200b078(wk->entry, 10) == 3) {
            *seq = 6;
        } else {
            *seq = 1;
        }
        break;
    case 1: {
        Regulation *regulation = func_0200ae14(wk->entry);
        u32 played = func_0200e918(wk->record, 0);

        if (played >= func_0200ae54(regulation, 0x16)) {
            *seq = 2;
        } else {
            *seq = 16;
        }
        break;
    }
    case 2:
        func_ov176_0217cfe8(wk, 10, 2);
        *seq = 17;
        func_ov177_021e8ab4(task, 3);
        break;
    case 3:
        func_0200b110(wk->entry, 10, 3);
        func_0200bbd8(getBattleBox(GameData_GetSaveControl(wk->param.gameData)), 2);
        func_02012dac(wk->param.gameData);
        *seq = 4;
        break;
    case 4:
        switch (func_02012dd0(wk->param.gameData)) {
        case 0:
        case 1:
            break;
        case 2:
        case 3:
            *seq = 5;
            break;
        }
        break;
    case 5:
        func_ov176_0217d1a0(wk, 1);
        func_ov176_0217cfe8(wk, 0x19, 1);
        *seq = 17;
        func_ov177_021e8ab4(task, 6);
        break;
    case 6:
        func_ov176_0217cfe8(wk, 0x1d, 1);
        *seq = 17;
        func_ov177_021e8ab4(task, 7);
        break;
    case 7:
        func_ov176_0217ceac(wk, 2);
        *seq = 8;
        break;
    case 8: {
        s32 selection = func_ov176_0217cfdc(wk);

        if (selection == -1) {
            break;
        }
        func_ov176_0217cfd0(wk);
        if (wk->hasVideo && !wk->param.matchResult->disconnected) {
            switch (selection) {
            case 0:
                *seq = 11;
                break;
            case 1:
                *seq = 9;
                break;
            }
        } else if (selection == 0) {
            *seq = 9;
        }
        break;
    }
    case 9:
        func_ov176_0217cfe8(wk, 0x1f, 1);
        *seq = 17;
        func_ov177_021e8ab4(task, 10);
        break;
    case 10:
        if (func_ov176_0217d194(wk)) {
            func_ov176_0217d174(wk);
            *seq = 15;
        }
        break;
    case 11:
        if (func_0200ba38((u8)wk->param.partnerData->unk44)) {
            *seq = 12;
        } else {
            *seq = 13;
        }
        break;
    case 12:
        func_ov176_0217cfe8(wk, 0x10, 1);
        *seq = 17;
        func_ov177_021e8ab4(task, 14);
        break;
    case 13:
        func_ov176_0217cfe8(wk, 0x22, 1);
        *seq = 17;
        func_ov177_021e8ab4(task, 6);
        break;
    case 14:
        func_ov176_0217d010(wk, 4);
        break;
    case 15:
        func_ov176_0217d010(wk, 1);
        break;
    case 16:
        func_ov177_021e8aa0(task, func_ov176_0217c2ec);
        break;
    case 17:
        if (func_ov176_0217d000(wk)) {
            func_ov177_021e8ab8(task);
        }
        break;
    }
}


void func_ov176_0217ceac(LiveBattleMatchFlow *wk, u32 kind) {
    WifiBattleMatchMenuParam param;
    u32 corner;
    u8 height;
    u8 x;
    u8 y;
    u8 width;

    MI_CpuFill8(&param, 0, sizeof(WifiBattleMatchMenuParam));
    param.msgData = wk->msgMain;
    param.font = wk->param.font;
    param.printQueue = wk->param.printQueue;
    switch (kind) {
    case 3:
        param.cursorRow = 1;
    case 0:
        param.msgIds[0] = 6;
        param.msgIds[1] = 7;
        param.count = 2;
        param.canCancel = TRUE;
        param.cancelValue = TRUE;
        corner = 1;
        break;
    case 1:
        param.canCancel = FALSE;
        if (wk->hasVideo && !wk->param.matchResult->disconnected) {
            param.msgIds[0] = 13;
            param.msgIds[1] = 14;
            param.msgIds[2] = 15;
            param.count = 3;
        } else {
            param.msgIds[0] = 13;
            param.msgIds[1] = 15;
            param.count = 2;
        }
        corner = 0;
        break;
    case 2:
        param.canCancel = FALSE;
        if (wk->hasVideo && !wk->param.matchResult->disconnected) {
            param.msgIds[0] = 14;
            param.msgIds[1] = 30;
            param.count = 2;
        } else {
            param.msgIds[0] = 30;
            param.count = 1;
        }
        corner = 0;
        break;
    }
    param.bg = 0;
    param.palette = 15;
    param.framePalette = 13;
    param.frameChar = 1;
    switch (corner) {
    case 0:
        width = 28;
        height = param.count * 2;
        x = 2;
        y = 9 - height / 2;
        break;
    case 1:
        width = 12;
        height = param.count * 2;
        x = 19;
        y = 17 - height;
        break;
    }
    wk->menu = func_ov177_021e88b0(&param, x, y, width, height, wk->heapId);
    wk->menuKind = kind;
}


void func_ov176_0217cfd0(LiveBattleMatchFlow *wk) {
    func_ov177_021e89d0(wk->menu);
}


// pokebw2's LiveBattleMatchFlow_MenuGetSelection
s32 func_ov176_0217cfdc(LiveBattleMatchFlow *wk) {
    return func_ov177_021e8a00(wk->menu);
}

// pokebw2's LiveBattleMatchFlow_PrintMessage
void func_ov176_0217cfe8(LiveBattleMatchFlow *wk, u32 msgId, u32 type) {
    func_ov177_021e8738(wk->param.msgBox, wk->msgMain, msgId, type);
}

BOOL func_ov176_0217d000(LiveBattleMatchFlow *wk) {
    return func_ov177_021e8844(wk->param.msgBox);
}

void func_ov176_0217d010(LiveBattleMatchFlow *wk, u32 result) {
    wk->result = result;
    func_ov177_021e8aa0(wk->task, func_ov176_0217bbd4);
}

void func_ov176_0217d028(LiveBattleMatchFlow *wk) {
    if (wk->playerCard == NULL) {
        SaveControl *save = GameData_GetSaveControl(wk->param.gameData);
        Regulation *regulation;
        void *records;
        PlayerInfo *info;
        ClActUnit *link;
        WifiBattleMatchLiveCardInfo card;

        getBattleBox(save);
        regulation = func_0200ae14(wk->entry);
        records = func_0200b204(getTournamentBlockAddress(save), 1);
        info = GetGameDataPlayerInfo(wk->param.gameData);
        link = func_ov177_021e5548(wk->param.net);

        MI_CpuFill8(&card, 0, sizeof(WifiBattleMatchLiveCardInfo));
        card.unkA0 = func_0200e918(wk->record, 1);
        card.unkA2 = func_0200e918(wk->record, 2);
        card.unkA4 = func_0200e918(wk->record, 0);
        card.unkA6 = func_0200ae54(regulation, 0x16);
        card.look = func_02008554(info);
        MI_CpuCopy8(func_0200b074(wk->entry), card.data, sizeof(card.data));
        {
            u32 field6 = func_0200b078(wk->entry, 6);
            u32 field5 = func_0200b078(wk->entry, 5);
            u32 field4 = func_0200b078(wk->entry, 4);
            card.startDate = (field4 << 24) | ((field5 << 24) >> 8) | ((field6 << 24) >> 16);
        }
        {
            u32 field9 = func_0200b078(wk->entry, 9);
            u32 field8 = func_0200b078(wk->entry, 8);
            u32 field7 = func_0200b078(wk->entry, 7);
            card.endDate = (field7 << 24) | ((field8 << 24) >> 8) | ((field9 << 24) >> 16);
        }
        card.variant = func_0200b078(wk->entry, 11);
        wk->playerCard = func_ov177_021e6118(&card, info, link, wk->param.seq, wk->param.font, wk->param.printQueue, wk->msgCard, wk->wordSet, records, 0, wk->heapId);
    }
}


// pokebw2's LiveBattleMatchFlow_PlayerCardCreateWait
void func_ov176_0217d160(LiveBattleMatchFlow *wk) {
    func_ov176_0217d028(wk);
    while (!func_ov176_0217d188(wk)) {
    }
}

// pokebw2's LiveBattleMatchFlow_PlayerCardDelete
void func_ov176_0217d174(LiveBattleMatchFlow *wk) {
    if (wk->playerCard != NULL) {
        func_ov177_021e62a0(wk->playerCard);
        wk->playerCard = NULL;
    }
}

// pokebw2's LiveBattleMatchFlow_PlayerCardIsReady
BOOL func_ov176_0217d188(LiveBattleMatchFlow *wk) {
    return func_ov177_021e64c8(wk->playerCard);
}

// pokebw2's LiveBattleMatchFlow_PlayerCardIsDone
BOOL func_ov176_0217d194(LiveBattleMatchFlow *wk) {
    return func_ov177_021e65fc(wk->playerCard);
}

// pokebw2's LiveBattleMatchFlow_PlayerCardShow
void func_ov176_0217d1a0(LiveBattleMatchFlow *wk, u32 a1) {
    func_ov177_021e62cc(wk->playerCard, a1, wk->msgCard, wk->param.printQueue, wk->param.font, wk->heapId);
}

void func_ov176_0217d1c8(LiveBattleMatchFlow *wk, LiveBattleMatchPlayerData *data) {
    if (wk->partnerCard == NULL) {
        ClActUnit *link = func_ov177_021e5548(wk->param.net);
        u32 field11 = func_0200b078(wk->entry, 11);

        wk->partnerCard = func_ov177_021e7520(data, link, wk->param.seq, wk->param.font, wk->param.printQueue, wk->msgCard, wk->wordSet, 1, 0, field11, wk->heapId);
    }
}


// pokebw2's LiveBattleMatchFlow_PartnerCardDelete
void func_ov176_0217d224(LiveBattleMatchFlow *wk) {
    if (wk->partnerCard != NULL) {
        func_ov177_021e7710(wk->partnerCard);
        wk->partnerCard = NULL;
    }
}

// pokebw2's LiveBattleMatchFlow_PartnerCardIsDone
BOOL func_ov176_0217d238(LiveBattleMatchFlow *wk) {
    return func_ov177_021e777c(wk->partnerCard);
}

void func_ov176_0217d244(LiveBattleMatchFlow *wk) {
    if (wk->teamList == NULL) {
        ClActUnit *link = func_ov177_021e5548(wk->param.net);
        BattleBoxSave *battleBox = getBattleBox(GameData_GetSaveControl(wk->param.gameData));
        wk->teamList = func_ov177_021e7e3c(battleBox, link, wk->param.seq, wk->font, wk->param.printQueue, wk->msgCard, wk->heapId);
    }
}


// pokebw2's LiveBattleMatchFlow_TeamListDelete
void func_ov176_0217d290(LiveBattleMatchFlow *wk) {
    if (wk->teamList != NULL) {
        func_ov177_021e82bc(wk->teamList);
        wk->teamList = NULL;
    }
}

// pokebw2's LiveBattleMatchFlow_TeamListIsReady
u32 func_ov176_0217d2a4(LiveBattleMatchFlow *wk) {
    return func_ov177_021e8338(wk->teamList);
}

// pokebw2's LiveBattleMatchFlow_TeamListIsDone
BOOL func_ov176_0217d2b0(LiveBattleMatchFlow *wk) {
    return func_ov177_021e8420(wk->teamList);
}

// Copies the team of the battle box into dest
// pokebw2's LiveBattleMatchFlow_LoadTeam
void func_ov176_0217d2bc(LiveBattleMatchFlow *wk, PokeParty *dest) {
    PokeParty *party = convertBoxedPokeSetToParty(getBattleBox(GameData_GetSaveControl(wk->param.gameData)), wk->heapId);

    MI_CpuCopy8(party, dest, PokeParty_GetSaveDataSize());
    GFL_HeapFree(party);
}

// Fills what is sent to the other player
// pokebw2's LiveBattleMatchFlow_FillPlayerData
void func_ov176_0217d2f0(LiveBattleMatchPlayerData *data, LiveBattleMatchFlow *wk) {
    GameData *gameData = wk->param.gameData;
    PokeParty *party;

    data->unk44 = 100;
    MI_CpuCopy8(GetGameDataPlayerInfo(gameData), data, func_02008468());
    OS_GetMacAddress(data->mac);
    func_0200e6fc(func_0200e6e8(GameData_GetSaveControl(gameData)), 0, &data->sentence);
    party = convertBoxedPokeSetToParty(getBattleBox(GameData_GetSaveControl(gameData)), HEAPID_LIVEBATTLEMATCH_TEAM);
    MI_CpuCopy8(party, (u8 *)data + LIVEBATTLEMATCH_PLAYER_HEADER_SIZE, PokeParty_GetSaveDataSize());
    GFL_HeapFree(party);
    data->record1 = func_0200e918(wk->record, 1);
    data->record2 = func_0200e918(wk->record, 2);
    data->record0 = func_0200e918(wk->record, 0);
    data->entryField2 = func_0200b078(wk->entry, 2);
}

// pokebw2's LiveBattleMatchFlow_SetEntryField2
void func_ov176_0217d388(LiveBattleMatchPlayerData *data, LiveBattleMatchFlow *wk) {
    data->entryField2 = func_0200b078(wk->entry, 2);
}

DigitalCardCheck *func_ov176_0217d398(const DigitalCardCheckParam *param, HeapID heapId) {
    DigitalCardCheck *check;

    check = GFL_HeapAllocate(heapId, sizeof(DigitalCardCheck), FALSE, "digitalcardcheck.c", 154);
    MI_CpuFill8(check, 0, sizeof(DigitalCardCheck));
    check->param = *param;
    check->heapId = heapId;
    func_ov177_021e58a4(check->param.seq, 3, heapId);
    check->msgData = GFL_MsgSysLoadData(FALSE, 2, 0xd2, heapId);
    check->wordSet = GFL_WordSetSystemCreate(8, 0x40, heapId);
    check->task = func_ov177_021e8a3c(check, func_ov176_0217d458, heapId);
    return check;
}


