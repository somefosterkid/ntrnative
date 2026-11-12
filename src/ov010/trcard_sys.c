// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/trcard_sys.c, ported by tools/port_file.py.
// The trainer card's data, which this file gathers from the save, and the procs that run overlay 186's trainer card
// screen and the screens it calls. The name is the ROM's string, from GFL_HeapAllocate's asserts

#include "types.h"
#include "app/pms_select.h"
#include "constants/version.h"
#include "field/app_call.h"
#include "field/field.h"
#include "field/pass_power_check.h"
#include "field/trcard_sys.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/rtc.h"
#include "save/adventure.h"
#include "save/c_gear_savedata.h"
#include "save/event_work.h"
#include "save/high_link.h"
#include "save/medal_box.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/pokewood.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/pms.h"
#include "save/timesig_savedata.h"
#include "save/bsubway_save.h"
#include "save/musical_savedata.h"
#include "save/playtime.h"
#include "app/monolith/monolith_tool.h"

#define HEAPID_TRAINER_CARD 0x26


// Overlay 111's trainer card screen
BOOL func_ov111_021c3cc0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov111_021c3f54(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov111_021c40dc(GameProc *proc, u32 *state, void *param, void *work);

// Black's layout: the medals screen's parameter is Black 2's
typedef struct {
    u32 heapId;
    u32 unk04;
    GameProcManager *procMgr;
    PMSSelectParam greeting;
    TrainerCardParam *param;
    // The card's unk06 when the screen opened
    u32 unk1C;
} TrainerCardSysWork;

BOOL func_ov010_0216f0ac(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216f144(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216f174(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216f1e0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_0216f228(GameProc *proc, u32 *state, void *param, void *work);

// The order of the declarations gives the order of the tables, which MWCC sorts by size
const GameProcFunctions data_ov010_02172614 = {
    func_ov010_0216f0ac,
    func_ov010_0216f174,
    func_ov010_0216f1e0,
};

const GameProcFunctions data_ov010_02172608 = {
    func_ov010_0216f144,
    func_ov010_0216f174,
    func_ov010_0216f228,
};

static const GameProcFunctions data_ov010_021725fc = {
    func_ov111_021c3cc0,
    func_ov111_021c3f54,
    func_ov111_021c40dc,
};

// The records that each sum on the card adds up, ending at 0
static const u16 data_ov010_02172620[9][7] = {
    { 0xc, 0xd, 0x10, 0x11, 0x24, 0x25, 0x5a },
    { 0xd, 0x11, 0x25 },
    { 0xe, 0x12, 0x26 },
    { 0xf, 0x13, 0x27 },
    { 0xc, 0x10, 0x24, 0x5a },
    { 0x5 },
    { 0x6 },
    { 0x71 },
    { 0x72, 0x74 },
};

u32 func_ov010_0216f3d4(GameRecords *records, u32 row, u32 max);

// Runs the screen's proc manager, and frees it when the screen has ended
static BOOL func_ov010_0216f088(GameProcManager **procMgr) {
    if (*procMgr != NULL && GFL_ProcMgrUpdate(*procMgr) == FALSE) {
        FreeGameProcManager(*procMgr);
        *procMgr = NULL;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0216f0ac(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardParam *cardParam = param;
    TrainerCardSysWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TRAINER_CARD, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(TrainerCardSysWork), HEAPID_TRAINER_CARD);
    MI_CpuFill8(wk, 0, sizeof(TrainerCardSysWork));
    wk->heapId = HEAPID_TRAINER_CARD;
    wk->param = GFL_HeapAllocate(wk->heapId, sizeof(TrainerCardParam), TRUE, "trcard_sys.c", 159);
    *wk->param = *cardParam;
    wk->param->data = GFL_HeapAllocate(wk->heapId, sizeof(TrainerCardData), TRUE, "trcard_sys.c", 161);
    func_ov010_0216f40c(wk->param->data, cardParam->gameData, 0, wk->param->canEdit, wk->heapId);
    wk->unk1C = wk->param->data->unk06;
    return TRUE;
}


BOOL func_ov010_0216f144(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardSysWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TRAINER_CARD, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(TrainerCardSysWork), HEAPID_TRAINER_CARD);
    MI_CpuFill8(wk, 0, sizeof(TrainerCardSysWork));
    wk->heapId = HEAPID_TRAINER_CARD;
    wk->param = param;
    return TRUE;
}

u32 func_ov010_0216f244(void *work, void *param);
u32 func_ov010_0216f288(void *work);
u32 func_ov010_0216f2b4(void *work);
u32 func_ov010_0216f2e0(void *work);
u32 func_ov010_0216f31c(void *work);
u32 func_ov010_0216f378(void *work);
u32 func_ov010_0216f3b0(void *work);
BOOL func_ov010_0216f174(GameProc *proc, u32 *state, void *param, void *work) {
    switch (*state) {
    case 0:
        *state = func_ov010_0216f244(work, param);
        break;
    case 1:
        *state = func_ov010_0216f288(work);
        break;
    case 2:
        *state = func_ov010_0216f2b4(work);
        break;
    case 3:
        *state = func_ov010_0216f2e0(work);
        break;
    case 4:
        *state = func_ov010_0216f31c(work);
        break;
    case 5:
        *state = func_ov010_0216f378(work);
        break;
    case 6:
        *state = func_ov010_0216f3b0(work);
        break;
    case 7:
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0216f1e0(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardSysWork *wk = work;
    TrainerCardParam *tcParam = param;

    if (tcParam->canEdit != 0) {
        PlayerInfo *info = GetGameDataPlayerInfo(tcParam->gameData);
        u32 id = func_02008554(info);
        if (wk->unk1C != id) {
            func_02027174(id);
        }
    }
    tcParam->result = wk->param->result;
    GFL_HeapFree(wk->param->data);
    GFL_HeapFree(wk->param);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_TRAINER_CARD);
    return TRUE;
}


BOOL func_ov010_0216f228(GameProc *proc, u32 *state, void *param, void *work) {
    TrainerCardSysWork *wk = work;

    GFL_HeapFree(wk->param);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_TRAINER_CARD);
    return TRUE;
}

u32 func_ov010_0216f244(void *work, void *param) {
    TrainerCardSysWork *wk = work;
    TrainerCardParam *tcParam = param;
    TrainerCardData *data;

    switch (tcParam->appParam) {
    case 0:
        break;
    case 1:
        return 1;
    case 2:
        return 1;
    case 3:
        return 5;
    }

    data = wk->param->data;
    if (data->clearMonth == 0 && data->playTime != NULL) {
        return 5;
    }
    return 1;
}

u32 func_ov010_0216f288(void *work) {
    TrainerCardSysWork *wk = work;

    if (wk->procMgr == NULL) {
        wk->procMgr = CreateGameProcManager(wk->heapId);
    }
    QueueGameProc(wk->procMgr, OVERLAY_NONE, &data_ov010_021725fc, wk->param);
    return 2;
}

u32 func_ov010_0216f2b4(void *work) {
    TrainerCardSysWork *wk = work;

    if (!func_ov010_0216f088(&wk->procMgr)) {
        return 2;
    }
    if (wk->param->next == 1) {
        return 3;
    }
    if (wk->param->next == 2) {
        return 5;
    }
    return 7;
}

extern const GameProcFunctions data_ov110_021c39d4;
u32 func_ov010_0216f2e0(void *work) {
    TrainerCardSysWork *wk = work;
    SaveControl *save;

    if (wk->procMgr == NULL) {
        wk->procMgr = CreateGameProcManager(wk->heapId);
    }
    save = GameData_GetSaveControl(wk->param->gameData);
    wk->greeting.save = save;
    QueueGameProc(wk->procMgr, OVERLAY_ID(110), &data_ov110_021c39d4, &wk->greeting);
    return 4;
}


// The Black layout has no shownData/canEdit; gameData is at 0x04
u32 func_ov010_0216f31c(void *work) {
    TrainerCardSysWork *wk = work;
    void *cgear;
    TrainerCardData *data;
    PMSData *pms;

    if (!func_ov010_0216f088(&wk->procMgr)) {
        return 4;
    }
    if (wk->greeting.result != NULL) {
        cgear = getCGearDataBlkAddress(GameData_GetSaveControl(wk->param->gameData));
        data = wk->param->data;
        pms = wk->greeting.result;
        data->greeting.type = pms->type;
        data->greeting.id = pms->id;
        data->greeting.words[0] = pms->words[0];
        data->greeting.words[1] = pms->words[1];
        func_0200e714(cgear, 0, wk->greeting.result);
        func_0202715c(wk->greeting.result);
    }
    return 1;
}

extern const GameProcFunctions data_ov111_021c892c;
u32 func_ov010_0216f378(void *work) {
    TrainerCardSysWork *wk = work;
    SaveControl *save;

    if (wk->procMgr == NULL) {
        wk->procMgr = CreateGameProcManager(wk->heapId);
    }
    save = GameData_GetSaveControl(wk->param->gameData);
    wk->greeting.save = save;
    QueueGameProc(wk->procMgr, OVERLAY_ID(110), &data_ov111_021c892c, wk->param);
    return 6;
}

u32 func_ov010_0216f3b0(void *work) {
    TrainerCardSysWork *wk = work;

    if (!func_ov010_0216f088(&wk->procMgr)) {
        return 6;
    }
    if (wk->param->next == 3) {
        return 1;
    }
    return 7;
}










// Adds up a row of the records, up to max
u32 func_ov010_0216f3d4(GameRecords *records, u32 row, u32 max) {
    u32 sum = 0;
    int i;

    for (i = 0; i < 7; i++) {
        if (data_ov010_02172620[row][i] == 0) {
            break;
        }
        sum += RecordGet(records, data_ov010_02172620[row][i]);
        if (sum > max) {
            sum = max;
        }
    }
    return sum;
}

void func_ov010_0216f40c(TrainerCardData *data, GameData *gameData, u32 isSnapshot, u32 canEdit, HeapID heapId) {
    RTCDate date;
    RTCTime time;
    u8 passPowerFlags[2];
    SaveControl *save = GameData_GetSaveControl(gameData);
    void *timeSig = getTimeSigBlkAddress(save);
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(gameData);
    TrainerCardSave *card = getTrainerCardDataBlkAddress(gameData);
    AdventureTime *adventureTime = getSaveAdventureTimeBlock(save);
    GameRecords *records = getTrainerCardInfoBlkAddress(save);
    void *cgear = func_0200e6e8(save);
    PokeDexSave *pokedex = GameData_GetPokedex(gameData);
    HighLinkSave *highLink;
    void *passPowerData;
    u8 i;
    u8 bit;

    if (!func_020084bc(playerInfo)) {
        MIi_CpuCopy16(GetPlayerName(playerInfo), data->name, sizeof(data->name));
    } else {
        data->name[0] = 'N';
        data->name[1] = 'o';
        data->name[2] = 'N';
        data->name[3] = 'a';
        data->name[4] = 'm';
        data->name[5] = 'e';
        data->name[6] = GFL_StrBufGetTerminator();
    }
    data->stars = func_ov010_0216f830(gameData);
    data->gender = getTrainerGender(playerInfo);
    data->trainerId = getIDAsUInt(playerInfo);
    data->money = getCash(card);
    data->badges = 0;
    data->version = GAME_VERSION;
    for (i = 0, bit = 1; i < 8; bit <<= 1, i++) {
        if (isBadgeObtained(card, i) == TRUE) {
            data->badges |= bit;
        }
    }
    if (isSnapshot == TRUE) {
        PlayTime *playTime = func_02012fcc(gameData);

        data->unk06 = func_02008554(playerInfo);
        data->isOwn = FALSE;
        data->playTime = NULL;
        data->playHours = func_0200864c(playTime);
        data->playMinutes = func_02008650(playTime);
    } else {
        data->unk06 = func_02008554(playerInfo);
        data->isOwn = TRUE;
        data->playTime = func_02012fcc(gameData);
    }
    data->unk48 = func_02008a0c(timeSig);
    data->unk04_4 = func_02008a28(timeSig);
    func_0200e6fc(cgear, 0, &data->greeting);
    RTC_ConvertSecondToDateTime(&date, &time, adventureTime->startSeconds);
    data->startYear = date.year;
    data->startMonth = date.month;
    data->startDay = date.day;
    if (adventureTime->seconds == 0) {
        data->clearYear = 0;
        data->clearMonth = 0;
        data->clearDay = 0;
        data->clearHour = 0;
        data->clearMinute = 0;
    } else {
        RTC_ConvertSecondToDateTime(&date, &time, adventureTime->seconds);
        data->clearYear = date.year;
        data->clearMonth = date.month;
        data->clearDay = date.day;
        data->clearHour = time.hour;
        data->clearMinute = time.minute;
    }
    data->unk38 = func_ov010_0216f3d4(records, 0, 999999999);
    data->unk3C = func_ov010_0216f3d4(records, 1, 999999999);
    data->unk40 = func_ov010_0216f3d4(records, 2, 999999999);
    data->unk44 = func_ov010_0216f3d4(records, 3, 999999999);
    data->unk4C = func_ov010_0216f3d4(records, 4, 999999999);
    data->unk50 = func_ov010_0216f3d4(records, 5, 999999999);
    data->unk54 = func_ov010_0216f3d4(records, 6, 999999999);
    data->unk58 = func_0200c1dc(card);
    data->unk5C = func_ov010_0216f3d4(records, 7, 9999);
    highLink = getHighLinkBlockAddress(save);
    passPowerData = PassPowerData_Create(heapId);
    func_0200bf54(highLink, passPowerFlags, 2);
    data->passPowerCount = func_ov010_0216feac(passPowerData, func_020127b8(gameData), passPowerFlags);
    PassPowerData_Free(passPowerData);
    data->unk60 = func_ov010_0216f3d4(records, 8, 9999);
    data->palParkHighScore = func_0200c184(card);
    data->unk64 = func_02008ea0(records);
    data->battleTestRank = func_02008e78(records);
    data->unk68_0 = func_020090e0(func_0200921c(gameData));
    if (func_0200bf94(getHighLinkBlockAddress(save)) != 0) {
        data->unk68_1 = TRUE;
    }
    if (data->unk60 != 0) {
        data->unk68_2 = TRUE;
    }
    if (RecordGet(records, 0x78) != 0) {
        data->unk68_3 = TRUE;
    }
    if (RecordGet(records, 0x79) != 0) {
        data->unk68_4 = TRUE;
    }
    if (EventWork_FlagGet(GameData_GetEventWork(gameData), 0x962) == TRUE) {
        data->unk04_2 = TRUE;
    } else {
        data->unk04_2 = FALSE;
    }
    data->seenCount = func_0200d0ac(pokedex, heapId);
    data->unk04_3 = func_020089d0(timeSig);
    MI_CpuCopy8(func_020089cc(timeSig), data->unk74, sizeof(data->unk74));
    data->canEdit = canEdit;
}


TrainerCardParam *func_ov010_0216f7bc(GameData *gameData, HeapID heapId, BOOL canEdit) {
    TrainerCardParam *param;

    param = GFL_HeapAllocate(heapId, sizeof(TrainerCardParam), TRUE, "trcard_sys.c", 0x321);
    MI_CpuFill8(param, 0, sizeof(TrainerCardParam));
    param->data = NULL;
    param->gameData = gameData;
    param->next = 0;
    param->canEdit = canEdit;
    return param;
}

TrainerCardParam *func_ov010_0216f7f4(GameData *gameData, TrainerCardData *data, HeapID heapId) {
    TrainerCardParam *param;

    param = GFL_HeapAllocate(heapId, sizeof(TrainerCardParam), TRUE, "trcard_sys.c", 0x33a);
    param->data = data;
    param->gameData = gameData;
    param->next = 0;
    data->canEdit = 0;
    param->data->unk04_7 = 1;
    return param;
}

u32 func_ov010_0216f830(GameData *gameData) {
    u32 stars = 0;
    BSubwayScoreData *bsubway = func_02012ef0(gameData);
    MusicalSave *musical = getMusicalInfoBlkAddress(gameData);
    PassPowerLevel *levels = func_020127b8(gameData);

    if (EventWork_FlagGet(GameData_GetEventWork(gameData), 0x960)) {
        stars++;
    }
    if (PokeDex_IsCompleteNational(GameData_GetPokedex(gameData))) {
        stars++;
    }
    if (func_0200dba0(bsubway, 7, 0) && func_0200dba0(bsubway, 8, 0)) {
        stars++;
    }
    if (func_0200a73c(musical)) {
        stars++;
    }
    if (func_0200bdf8(levels) >= 30 && func_0200bdfc(levels) >= 30) {
        stars++;
    }
    return stars;
}




