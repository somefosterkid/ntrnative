// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov290/wifibattlematch_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifibattlematch_sys.c, in overlay 290 (named by its embedded string): the Battle Competition. It runs a chain of the
// overlays that make it up, each with a function that makes its parameters and one that reads what it left

#include "types.h"
#include "app/battle_championship_flow.h"
#include "app/livebattlematch_irc.h"
#include "app/ov306.h"
#include "app/wifi_login.h"
#include "app/wifibattlematch.h"
#include "app/wifibattlematch_battle.h"
#include "app/wifibattlematch_link.h"
#include "app/wifibattlematch_subproc.h"
#include "battle/battle_proc.h"
#include "battle/battle_rec_tool.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "dwc/dwc.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "system/game_data.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
extern const GameProcFunctions data_ov176_0217e538;
extern const GameProcFunctions data_ov214_021bb474;
extern const GameProcFunctions data_ov199_02213ce8;
extern const GameProcFunctions data_ov178_021e5b3c;
extern const GameProcFunctions data_ov175_021e2c68;
extern const GameProcFunctions data_ov178_021e5b30;
extern const GameProcFunctions data_ov115_021d6338;
extern const GameProcFunctions data_ov115_021d654c;
extern const GameProcFunctions data_ov093_021efdac;
void MI_CpuCopy8(const void *src, void *dest, u32 size);

#define HEAPID_WIFIBATTLEMATCH 0x56
// What the chain runs, by the number that func_ov199_02213808 gets
enum {
    STEP_CHAMPIONSHIP,
    STEP_NET,
    STEP_TEAM_LIST,
    STEP_BATTLE,
    STEP_LOGIN,
    STEP_TEAM_EXCHANGE,
    STEP_RESULT,
    STEP_LOGOUT,
    STEP_REPLAY,
    STEP_COUNT,
};

typedef struct WifiBattleMatchSys WifiBattleMatchSys;
typedef struct WifiBattleMatchChain WifiBattleMatchChain;

// Makes the parameters of a step, which the chain frees or the step's finish function does
typedef void *(*WifiBattleMatchCreateFunc)(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
// Reads what the step left, and returns TRUE once it is done
typedef BOOL (*WifiBattleMatchFinishFunc)(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);

typedef struct {
    s32 overlayId;
    const GameProcFunctions *functions;
    WifiBattleMatchCreateFunc create;
    WifiBattleMatchFinishFunc finish;
} WifiBattleMatchStep;

// Black 2's; twinned code confirms the offsets of procManager, p_proc_param, heapId, sys, steps, previous, next,
// current, state and procResult in Black
struct WifiBattleMatchChain {
    GameProcManager *procManager;
    void *p_proc_param;
    HeapID heapId;
    WifiBattleMatchSys *sys;
    const WifiBattleMatchStep *steps;
    u32 stepCount;
    // The step that ran last, the one to run next and the one running
    u8 previous;
    u8 next;
    u8 current;
    u8 state;
    u32 procResult;
};

// A player as the two machines exchange it: the info and the team picked
// Black 2's, not checked against Black
typedef struct {
    PlayerInfo info;
    u8 unk20[0x24];
    u32 unk44;
    u32 unk48;
    u8 unk4C[0xd4 - 0x4c];
    u8 partyData[];
} WifiBattleMatchPlayer;

// What the battle left, for the steps that follow it
// Black 2's; twinned code confirms the offsets of unkC in Black
typedef struct {
    u32 result;
    u32 battleStyle;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    // Set when the other player left
    u32 unkC;
    u32 unk10;
    u32 unk14;
} WifiBattleMatchBattleResult;

// What a battle sends to the ranking: the other player's team, then the player's own
// Black 2's, not checked against Black
typedef struct {
    s32 profileId;
    u32 unk4;
    u16 species[6];
    u8 forms[6];
    u8 levels[6];
    u8 genders[6];
    u16 ownSpecies[6];
    u8 ownForms[6];
    u8 ownLevels[6];
    u8 ownGenders[6];
    u32 year : 7;
    u32 month : 4;
    u32 day : 5;
    u32 hour : 5;
    u32 minute : 6;
    u32 unk : 4;
    u32 unkFlag : 1;
    u32 unk48;
    u8 result;
    u8 battleType;
    u8 aliveOwn : 4;
    u8 aliveOther : 4;
} WifiBattleMatchReport;

// Black 2's; twinned code confirms the offsets of ownsGameData, param, chain, mode, result, player, otherPlayer, party,
// otherParty, pickParty, otherPickParty, battleResult, unk1CC and relogin in Black
struct WifiBattleMatchSys {
    BOOL ownsGameData;
    WifiBattleMatchParam param;
    WifiBattleMatchChain *chain;
    // 0 and 1 are the competitions that the first screen starts, 2 and 3 the live ones
    u32 mode;
    u32 result;
    BOOL unk24;
    WifiBattleMatchPlayer *player;
    WifiBattleMatchPlayer *otherPlayer;
    // The team picked, the other player's team, and the parties they pick from
    PokeParty *party;
    PokeParty *otherParty;
    PokeParty *pickParty;
    PokeParty *otherPickParty;
    u8 unk40[0x174];
    WifiBattleMatchBattleResult battleResult;
    u32 unk1CC;
    WifiBattleMatchReport report;
    BOOL relogin;
    u8 unk224[8];
    u8 unk22C[0x54c];
    u8 unk778[0x5c];
    u8 unk7D4[0xd4];
    u32 unk8A8;
    BOOL unk8AC;
};

// What the infrared steps get
// Black 2's, not checked against Black
typedef struct {
    GameData *gameData;
    u32 previous;
    u32 result;
    WifiBattleMatchPlayer *player;
    WifiBattleMatchPlayer *otherPlayer;
    WifiBattleMatchBattleResult *battleResult;
} WifiBattleMatchChampionshipParam;

// What the Wi-Fi link gets
// Black 2's, not checked against Black
typedef struct {
    u32 mode;
    // What the link leaves for the chain to do next
    u32 action;
    WifiBattleMatchParam *param;
    u32 result;
    void *record;
    WifiBattleMatchPlayer *player;
    WifiBattleMatchPlayer *otherPlayer;
    void *unk1C;
    u32 *unk20;
    WifiBattleMatchReport *report;
    void *unk28;
    WifiBattleMatchBattleResult *battleResult;
    void *unk30;
    void *unk34;
    void *unk38;
    void *unk3C;
    BOOL *unk40;
} WifiBattleMatchNetParam;

static BOOL func_ov199_02212560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov199_022126f4(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov199_0221268c(GameProc *proc, u32 *state, void *param, void *work);
static void *func_ov199_02212740(WifiBattleMatchChain *chain, HeapID heapId,
                                                        WifiBattleMatchSys *sys);
static BOOL func_ov199_022127f0(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_02212854(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_0221290c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
void *func_ov199_02212988(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_02212a90(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_02212af8(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
BOOL func_ov199_02212e74(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_02212fd8(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_0221303c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_02213078(WifiBattleMatchChain *chain, HeapID heapId,
                                                    WifiBattleMatchSys *sys);
static BOOL func_ov199_022130d4(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
void *func_ov199_02213134(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_0221320c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_02213234(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_02213278(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void *func_ov199_022132c4(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys);
static BOOL func_ov199_02213338(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys);
static void func_ov199_0221336c(WifiBattleMatchSys *sys, HeapID heapId);
static void func_ov199_022133d0(WifiBattleMatchSys *sys);
void func_ov199_022133e4(WifiBattleMatchReport *report, PokeParty *party, PokeParty *otherParty, WifiBattleMatchPlayer *otherPlayer, Regulation *regulation, u32 unk14, BtlSetup *setup, WifiBattleMatchBattleResult *battleResult, u32 unk10);
void func_ov199_022135cc(const BtlSetup *setup, u8 *own, u8 *other);
void func_ov199_02213684(const BtlSetup *setup, u8 *own, u8 *other);
static WifiBattleMatchChain *func_ov199_022136e4(const WifiBattleMatchStep *steps, u32 stepCount,
                                                            WifiBattleMatchSys *sys, HeapID heapId);
static void func_ov199_0221372c(WifiBattleMatchChain *p_wk);
static BOOL func_ov199_02213754(WifiBattleMatchChain *chain);
static u32 func_ov199_02213800(WifiBattleMatchChain *chain);
static u8 func_ov199_02213804(WifiBattleMatchChain *chain);
static void func_ov199_02213808(WifiBattleMatchChain *chain, u8 step);
static void func_ov199_0221380c(WifiBattleMatchChain *chain);
// pokebw2's WIFIBATTLEMATCH_PROC_FUNCTIONS
const GameProcFunctions data_ov199_02213c4c = {
    func_ov199_02212560,
    func_ov199_022126f4,
    func_ov199_0221268c,
};

// pokebw2's sWifiBattleMatchSysSteps
static const WifiBattleMatchStep data_ov199_02213c58[STEP_COUNT] = {
    { OVERLAY_ID(176), &data_ov176_0217e538, func_ov199_02212740,
      func_ov199_022127f0 },
    { OVERLAY_ID(175), &data_ov175_021e2c68, func_ov199_02212854, func_ov199_0221290c },
    { OVERLAY_ID(178), &data_ov178_021e5b30, func_ov199_02212988,
      func_ov199_02212a90 },
    { OVERLAY_NONE, &data_ov199_02213ce8, func_ov199_02212af8, func_ov199_02212e74 },
    { OVERLAY_ID(115), &data_ov115_021d6338, func_ov199_02212fd8, func_ov199_0221303c },
    { OVERLAY_ID(178), &data_ov178_021e5b3c, func_ov199_02213078,
      func_ov199_022130d4 },
    { OVERLAY_ID(214), &data_ov214_021bb474, func_ov199_02213134, func_ov199_0221320c },
    { OVERLAY_ID(115), &data_ov115_021d654c, func_ov199_02213234,
      func_ov199_02213278 },
    { OVERLAY_NONE, &data_ov093_021efdac, func_ov199_022132c4, func_ov199_02213338 },
};

// pokebw2's WifiBattleMatchSys_Init
static BOOL func_ov199_02212560(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchParam *matchParam = param;
    WifiBattleMatchSys *sys;
    WifiBattleMatchPlayer *player;
    PokeParty *party;
    HeapID parentHeapId;

    switch (matchParam->unk8) {
    case 1:
        parentHeapId = HEAPID_USER;
        break;
    case 0:
        parentHeapId = HEAPID_USER;
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    GFL_HeapCreateChild(parentHeapId, HEAPID_WIFIBATTLEMATCH, 0x8000);
    sys = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchSys), HEAPID_WIFIBATTLEMATCH);
    MI_CpuFill8(sys, 0, sizeof(WifiBattleMatchSys));
    MI_CpuCopy8(matchParam, &sys->param, sizeof(WifiBattleMatchParam));
    sys->result = 0;
    if (sys->param.gameData == NULL) {
        sys->param.gameData = GameData_Create(HEAPID_USER);
        sys->ownsGameData = TRUE;
    }
    sys->party = PokeParty_Create(HEAPID_WIFIBATTLEMATCH);
    sys->otherParty = PokeParty_Create(HEAPID_WIFIBATTLEMATCH);
    sys->pickParty = PokeParty_Create(HEAPID_WIFIBATTLEMATCH);
    sys->otherPickParty = PokeParty_Create(HEAPID_WIFIBATTLEMATCH);
    func_0200b400(HEAPID_WIFIBATTLEMATCH);
    func_ov199_0221336c(sys, HEAPID_WIFIBATTLEMATCH);
    sys->chain = func_ov199_022136e4(data_ov199_02213c58, STEP_COUNT, sys, HEAPID_WIFIBATTLEMATCH);
    switch (sys->param.unk8) {
    case 0:
        sys->mode = 0;
        func_ov199_02213808(sys->chain, STEP_CHAMPIONSHIP);
        break;
    case 1:
        sys->mode = 2;
        func_ov199_02213808(sys->chain, STEP_LOGIN);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    player = sys->player;
    switch (sys->param.unkC) {
    case 0:
        party = GameData_GetParty(sys->param.gameData);
        MI_CpuCopy8(party, player->partyData, PokeParty_GetSaveDataSize());
        break;
    case 1:
        party = convertBoxedPokeSetToParty(getBattleBox(GameData_GetSaveControl(sys->param.gameData)),
                                           HEAPID_TAIL(HEAPID_WIFIBATTLEMATCH));
        MI_CpuCopy8(party, player->partyData, PokeParty_GetSaveDataSize());
        GFL_HeapFree(party);
        break;
    }
    return TRUE;
}

// pokebw2's WifiBattleMatchSys_Exit
static BOOL func_ov199_0221268c(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchSys *sys = work;

    func_ov199_0221372c(sys->chain);
    func_ov199_022133d0(sys);
    freeVSPlayerBlkClearPtr();
    GFL_HeapFree(sys->otherPickParty);
    GFL_HeapFree(sys->pickParty);
    GFL_HeapFree(sys->otherParty);
    GFL_HeapFree(sys->party);
    if (sys->ownsGameData) {
        GameData_Free(sys->param.gameData);
    }
    if (sys->param.unk8 != 1) {
        OS_ResetSystem(0);
    }
    if (sys->param.unk10 != 0) {
        GFL_HeapFree(param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH);
    func_02005d28();
    return TRUE;
}

// pokebw2's WifiBattleMatchSys_Main
static BOOL func_ov199_022126f4(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchSys *sys = work;
    u32 result;

    switch (*state) {
    case 0:
        if (func_ov199_02213754(sys->chain)) {
            *state = 1;
        }
        if (sys->unk1CC != 0) {
            sys->unk1CC--;
        }
        result = func_ov199_02213800(sys->chain);
        if (result == 2 || result == 0) {
            GFL_NetErrShow(0);
        }
        break;
    case 1:
        return TRUE;
    }
    return FALSE;
}

static void *func_ov199_02212740(WifiBattleMatchChain *chain, HeapID heapId,
                                                        WifiBattleMatchSys *sys) {
    WifiBattleMatchChampionshipParam *param =
        GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchChampionshipParam), FALSE, "wifibattlematch_sys.c", 591);

    MI_CpuFill8(param, 0, sizeof(WifiBattleMatchChampionshipParam));
    param->gameData = sys->param.gameData;
    param->player = sys->player;
    param->otherPlayer = sys->otherPlayer;
    param->battleResult = &sys->battleResult;
    switch (func_ov199_02213804(chain)) {
    case STEP_LOGIN:
    case STEP_LOGOUT:
        param->previous = 1;
        break;
    case STEP_REPLAY:
        param->previous = 6;
        break;
    case STEP_BATTLE:
        param->previous = 4;
        break;
    case STEP_RESULT:
        param->previous = 5;
        break;
    case STEP_CHAMPIONSHIP:
        param->previous = 0;
        break;
    default:
        if (sys->battleResult.unkC != 0) {
            param->previous = 6;
        } else {
            GFL_ASSERT(0);
        }
        break;
    }
    if (GFL_SndBGMGetID() != SEQ_BGM_WCS) {
        GFL_SndBGMPlay(SEQ_BGM_WCS, SND_CHANNEL_MASK_ALL);
    }
    return param;
}

// pokebw2's WifiBattleMatchSys_FinishChampionship
static BOOL func_ov199_022127f0(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiBattleMatchChampionshipParam *championship = param;

    switch (championship->result) {
    case 0:
        func_ov199_0221380c(chain);
        break;
    case 1:
        sys->mode = 0;
        func_ov199_02213808(chain, STEP_LOGIN);
        break;
    case 2:
        sys->mode = 1;
        func_ov199_02213808(chain, STEP_TEAM_LIST);
        break;
    case 3:
        sys->mode = 1;
        func_ov199_02213808(chain, STEP_RESULT);
        break;
    case 4:
        sys->mode = 1;
        func_ov199_02213808(chain, STEP_REPLAY);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    GFL_HeapFree(championship);
    return TRUE;
}

// pokebw2's WifiBattleMatchSys_CreateNetParam
static void *func_ov199_02212854(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiBattleMatchNetParam *param =
        GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchNetParam), FALSE, "wifibattlematch_sys.c", 704);

    MI_CpuFill8(param, 0, sizeof(WifiBattleMatchNetParam));
    param->param = &sys->param;
    param->result = sys->result;
    param->mode = sys->mode - 2;
    param->player = sys->player;
    param->otherPlayer = sys->otherPlayer;
    param->unk1C = sys->unk40;
    param->unk20 = &sys->unk1CC;
    param->report = &sys->report;
    param->unk3C = sys->unk7D4;
    param->unk28 = &sys->unk8A8;
    param->battleResult = &sys->battleResult;
    param->unk30 = sys->unk224;
    param->unk34 = sys->unk22C;
    param->unk38 = sys->unk778;
    param->unk40 = &sys->unk8AC;
    param->record = func_0200ea04(getRecordBlkAddress(GameData_GetSaveControl(sys->param.gameData)));
    if (GFL_SndBGMGetID() != SEQ_BGM_WCS) {
        GFL_SndBGMPlay(SEQ_BGM_WCS, SND_CHANNEL_MASK_ALL);
    }
    return param;
}

// pokebw2's WifiBattleMatchSys_FinishNet
static BOOL func_ov199_0221290c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiBattleMatchNetParam *net = param;

    switch (net->mode) {
    case 0:
        sys->mode = 2;
        break;
    case 1:
        sys->mode = 3;
        break;
    case 2:
        break;
    case 3:
        sys->mode = 3;
        break;
    }
    switch (net->action) {
    case 1:
        func_ov199_02213808(chain, STEP_TEAM_LIST);
        break;
    case 2:
        func_ov199_02213808(chain, STEP_RESULT);
        break;
    case 3:
        sys->relogin = TRUE;
        func_ov199_02213808(chain, STEP_LOGIN);
        break;
    case 4:
        sys->unk24 = TRUE;
        // fall through
    case 0:
        func_ov199_02213808(chain, STEP_LOGOUT);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    GFL_HeapFree(net);
    return TRUE;
}

void *func_ov199_02212988(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiBattleMatchListParam *param;
    WifiBattleMatchPlayer *otherPlayer;
    u32 fileId;

    PokeParty_Init(sys->party);
    PokeParty_Init(sys->otherParty);
    param = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchListParam), FALSE, "wifibattlematch_sys.c", 815);
    MI_CpuFill8(param, 0, sizeof(WifiBattleMatchListParam));
    if (sys->mode - 2 <= 1) {
        switch (sys->param.unk4) {
        case 0:
            fileId = 0xf;
            break;
        case 1:
            fileId = 0x10;
            break;
        case 2:
            fileId = 0x11;
            break;
        case 3:
            fileId = 0x12;
            break;
        case 4:
            fileId = 0x13;
            break;
        default:
            GFL_ASSERT(0);
            break;
        }
        param->regulation = func_0201a354(fileId, heapId);
    } else if (sys->mode == 0) {
        param->regulation = func_0200ae14(func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(sys->param.gameData)), 0));
    } else if (sys->mode == 1) {
        param->regulation = func_0200ae14(func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(sys->param.gameData)), 1));
    } else {
        GFL_ASSERT(0);
    }
    param->selected = sys->party;
    param->gameData = sys->param.gameData;
    PokeParty_Copy((PokeParty *)sys->player->partyData, sys->pickParty);
    func_0201a2a8(param->regulation, sys->pickParty);
    param->party = sys->pickParty;
    otherPlayer = sys->otherPlayer;
    PokeParty_Copy((PokeParty *)otherPlayer->partyData, sys->otherPickParty);
    func_0201a2a8(param->regulation, sys->otherPickParty);
    param->otherName = GetPlayerName(&otherPlayer->info);
    param->otherParty = sys->otherPickParty;
    param->otherGender = getTrainerGender(&otherPlayer->info);
    return param;
}


// pokebw2's WifiBattleMatchSys_FinishList
static BOOL func_ov199_02212a90(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiBattleMatchListParam *list = param;

    if (sys->mode == 1) {
        switch (list->result) {
        case 0:
            func_ov199_02213808(chain, STEP_TEAM_EXCHANGE);
            break;
        case 2:
            sys->battleResult.unkC = TRUE;
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
            break;
        }
    } else {
        switch (list->result) {
        case 0:
            func_ov199_02213808(chain, STEP_TEAM_EXCHANGE);
            break;
        case 1:
            sys->relogin = TRUE;
            func_ov199_02213808(chain, STEP_LOGIN);
            break;
        case 3:
            sys->result = 2;
            func_ov199_02213808(chain, STEP_NET);
            break;
        }
    }
    if (sys->mode - 2 <= 1) {
        GFL_HeapFree(list->regulation);
    }
    GFL_HeapFree(list);
    return TRUE;
}

static void *func_ov199_02212af8(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiBattleMatchBattleParam *param;
    Regulation *regulation;
    void *tournament;
    void *entry;
    BOOL ownsRegulation = FALSE;
    u32 fileId;
    u32 battleType;
    int param0D;
    u32 value;
    u32 param12;
    BattlePlayer *player;

    tournament = getTournamentBlockAddress(GameData_GetSaveControl(sys->param.gameData));
    param = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchBattleParam), FALSE, "wifibattlematch_sys.c", 992);
    fileId = 0x10;
    MI_CpuFill8(param, 0, sizeof(WifiBattleMatchBattleParam));
    param->gameData = sys->param.gameData;
    clearVSPlayer();
    MI_CpuFill8(&sys->battleResult, 0, sizeof(WifiBattleMatchBattleResult));
    param->players = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchPlayers), FALSE, "wifibattlematch_sys.c", 1003);
    MI_CpuFill8(param->players, 0, sizeof(WifiBattleMatchPlayers));
    param->players->records = GameData_GetRecords(sys->param.gameData);
    param->players->unk50 = 0;
    player = &param->players->base.players[0];
    player->info = &sys->player->info;
    player->party = sys->party;
    player->unk08[1] = sys->player->unk44;
    player = &param->players->base.players[1];
    player->info = &sys->otherPlayer->info;
    player->party = sys->otherParty;
    player->unk08[1] = sys->otherPlayer->unk44;
    param->setup = GFL_HeapAllocate(heapId, sizeof(BtlSetup), FALSE, "wifibattlematch_sys.c", 1027);
    MI_CpuFill8(param->setup, 0, sizeof(BtlSetup));
    GFL_OvlLoad(OVERLAY_ID(93));
    if (sys->mode - 2 <= 1) {
        switch (sys->param.unk4) {
        default:
        case 0:
            fileId = 0xf;
            BtlSetup_SetNet1v1Single(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 1:
            BtlSetup_SetNet1v1Double(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 2:
            fileId = 0x11;
            BtlSetup_SetNetTriple(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 3:
            fileId = 0x12;
            BtlSetup_SetNetRotation(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 4:
            fileId = 0x13;
            BtlSetup_SetNetTriple(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        }
        regulation = func_0201a354(fileId, heapId);
        ownsRegulation = TRUE;
    } else if (sys->mode == 0) {
        regulation = func_0200ae14(func_0200b1d4(tournament, 0));
        switch (func_0200ae54(regulation, 0x15)) {
        case 0:
            BtlSetup_SetNet1v1Single(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 1:
            BtlSetup_SetNet1v1Double(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 2:
            BtlSetup_SetNetTriple(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 3:
            BtlSetup_SetNetRotation(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        default:
            BtlSetup_SetNet1v1Single(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        }
    } else if (sys->mode == 1) {
        regulation = func_0200ae14(func_0200b1d4(tournament, 1));
        switch (func_0200ae54(regulation, 0x15)) {
        case 0:
            BtlSetup_SetNet1v1Single(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 1:
            BtlSetup_SetNet1v1Double(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 2:
            BtlSetup_SetNetTriple(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        case 3:
            BtlSetup_SetNetRotation(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        default:
            BtlSetup_SetNet1v1Single(param->setup, sys->param.gameData, func_020399a8(), 2, heapId);
            break;
        }
    } else {
        GFL_ASSERT(0);
    }
    if (sys->mode == 3) {
        battleType = sys->param.unk4 + 0x17;
    } else if (sys->mode == 2) {
        battleType = sys->param.unk4 + 0x1c;
    } else {
        param0D = func_0200ae54(regulation, 0xd);
        value = func_0200ae54(regulation, 0x15);
        if (param0D >= 2) {
            param0D = 1;
        }
        battleType = value * 2 + 0x21 + param0D;
    }
    param->players->base.rule = battleType;
    if (sys->mode - 2 <= 1) {
        param->setup->fieldSituation.bgm = SEQ_BGM_VS_TRAINER_WIFI;
        param->setup->fieldSituation.unk12 = SEQ_BGM_WIN2;
    } else if (sys->mode == 0) {
        entry = func_0200b1d4(tournament, 0);
        value = func_0200b078(entry, 0xb);
        param12 = func_0200ae54(func_0200ae14(entry), 0x12);
        if (value == 1) {
            param->setup->fieldSituation.bgm = SEQ_BGM_VS_WCS;
            param->setup->fieldSituation.unk12 = SEQ_BGM_WIN5;
        } else {
            param->setup->fieldSituation.bgm = SEQ_BGM_VS_SUBWAY_TRAINER;
            param->setup->fieldSituation.unk12 = SEQ_BGM_WIN2;
        }
        if (param12 - 2 <= 1) {
            param->setup->fieldSituation.env.terrain = 0xe;
            param->setup->fieldSituation.env.bgType = 0x12;
            param->players->unk50 = 1;
        }
    } else if (sys->mode == 1) {
        entry = func_0200b1d4(tournament, 1);
        value = func_0200b078(entry, 0xb);
        param12 = func_0200ae54(func_0200ae14(entry), 0x12);
        if (value == 1) {
            param->setup->fieldSituation.bgm = SEQ_BGM_VS_WCS;
            param->setup->fieldSituation.unk12 = SEQ_BGM_WIN5;
        } else {
            param->setup->fieldSituation.bgm = SEQ_BGM_VS_SUBWAY_TRAINER;
            param->setup->fieldSituation.unk12 = SEQ_BGM_WIN2;
        }
        if (param12 - 2 <= 1) {
            param->setup->fieldSituation.env.terrain = 0xe;
            param->setup->fieldSituation.env.bgType = 0x12;
            param->players->unk50 = 1;
        }
    }
    func_02013128(param->setup, sys->party, 0);
    func_02013160(param->setup, regulation, HEAPID_TAIL(heapId));
    if (sys->unk8AC) {
        param->setup->unkC1_2 = 1;
    }
    func_02013980(param->setup, heapId);
    GFL_OvlUnload(OVERLAY_ID(93));
    if (ownsRegulation) {
        GFL_HeapFree(regulation);
    }
    return param;
}


BOOL func_ov199_02212e74(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiBattleMatchBattleParam *battle = param;
    BtlSetup *setup = battle->setup;
    u32 result = battle->result;
    Regulation *regulation;
    void *entry;
    u32 value;
    u8 aliveOther;
    u8 aliveOwn;
    u8 hpOwn;

    sys->battleResult.battleStyle = setup->battleStyle;
    sys->battleResult.result = setup->unk8C;
    sys->battleResult.unk10 = setup->unkC1_0;
    sys->battleResult.result = setup->unk8C;
    func_ov199_022135cc(battle->setup, &aliveOwn, &aliveOther);
    sys->battleResult.unk8 = aliveOther;
    func_ov199_02213684(battle->setup, &hpOwn, &sys->battleResult.unk9);
    func_0200ba58();
    func_ov188_021eee78(battle->setup);
    func_0200ba68();
    if (sys->mode == 0) {
        entry = func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(sys->param.gameData)), 0);
        regulation = func_0200ae14(entry);
        value = func_0200b078(entry, 2);
        if (result == 0) {
            func_ov199_022133e4(&sys->report, sys->party, sys->otherParty, sys->otherPlayer, regulation, value, setup, &sys->battleResult, 0);
        }
    } else if (sys->mode == 2) {
        regulation = func_0201a354(sys->param.unk4 + 0xf, HEAPID_WIFIBATTLEMATCH);
        if (result == 0) {
            func_ov199_022133e4(&sys->report, sys->party, sys->otherParty, sys->otherPlayer, regulation, 0, setup, &sys->battleResult, 2);
        }
        GFL_HeapFree(regulation);
    }
    func_020130dc(battle->setup);
    GFL_HeapFree(battle->setup);
    GFL_HeapFree(battle->players);
    GFL_HeapFree(battle);
    if (sys->mode == 1) {
        switch (result) {
        case 0:
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
            break;
        case 1:
            sys->battleResult.unkC = TRUE;
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
            break;
        }
    } else {
        switch (result) {
        case 0:
            sys->result = 1;
            func_ov199_02213808(chain, STEP_NET);
            break;
        case 2:
            sys->relogin = TRUE;
            func_ov199_02213808(chain, STEP_LOGIN);
            break;
        case 3:
            sys->result = 2;
            func_ov199_02213808(chain, STEP_NET);
            break;
        }
    }
    return TRUE;
}


// pokebw2's WifiBattleMatchSys_CreateLoginParam
static void *func_ov199_02212fd8(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiLoginParam *param = GFL_HeapAllocate(heapId, sizeof(WifiLoginParam), FALSE, "wifibattlematch_sys.c", 1416);
    u32 serviceId;

    MI_CpuFill8(param, 0, sizeof(WifiLoginParam));
    param->gameData = sys->param.gameData;
    param->unk4 = 0;
    param->unk8 = 1;
    param->buffer = sys->unk40;
    if (sys->param.unk8 == 0) {
        serviceId = 0x24;
    } else {
        serviceId = 0x34;
    }
    param->unkC = serviceId;
    if (sys->relogin) {
        sys->relogin = FALSE;
        param->unk14 = TRUE;
    } else {
        param->unk14 = FALSE;
    }
    return param;
}

// pokebw2's WifiBattleMatchSys_FinishLogin
static BOOL func_ov199_0221303c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiLoginParam *login = param;
    u32 result = login->result;

    GFL_HeapFree(login);
    switch (result) {
    case 0:
        sys->result = 0;
        func_ov199_02213808(chain, STEP_NET);
        break;
    case 1:
        if (sys->param.unk8 == 0) {
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
        } else {
            func_ov199_0221380c(chain);
        }
        break;
    }
    return TRUE;
}

static void *func_ov199_02213078(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiBattleMatchExchangeParam *param = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchExchangeParam), FALSE, "wifibattlematch_sys.c", 1500);

    MI_CpuFill8(param, 0, sizeof(WifiBattleMatchExchangeParam));
    param->gameData = &sys->param.gameData;
    param->party = sys->party;
    param->otherParty = sys->otherParty;
    param->unk14 = sys->unk7D4;
    param->unk18 = &sys->unk8A8;
    if (sys->mode == 1) {
        param->mode = WIFIBATTLEMATCH_EXCHANGE_IRC;
    } else {
        param->mode = WIFIBATTLEMATCH_EXCHANGE_WIFI;
    }
    return param;
}

// pokebw2's WifiBattleMatchSys_FinishExchange
static BOOL func_ov199_022130d4(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    WifiBattleMatchExchangeParam *exchange = param;
    u32 result = exchange->result;

    GFL_HeapFree(exchange);
    if (sys->mode == 1) {
        switch (result) {
        case 0:
            func_ov199_02213808(chain, STEP_BATTLE);
            break;
        case 2:
            sys->battleResult.unkC = TRUE;
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
            break;
        }
    } else {
        switch (result) {
        case 0:
            func_ov199_02213808(chain, STEP_BATTLE);
            break;
        case 1:
            sys->relogin = TRUE;
            func_ov199_02213808(chain, STEP_LOGIN);
            break;
        case 3:
            sys->result = 2;
            func_ov199_02213808(chain, STEP_NET);
            break;
        }
    }
    return TRUE;
}

void *func_ov199_02213134(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    Ov306Param *param = GFL_HeapAllocate(heapId, sizeof(Ov306Param), FALSE, "wifibattlematch_sys.c", 1591);
    u32 battleType;
    u32 index;
    u32 value;
    int param0D;
    Regulation *regulation;

    MI_CpuFill8(param, 0, sizeof(Ov306Param));
    if (sys->mode == 3) {
        battleType = sys->param.unk4 + 0x17;
    } else if (sys->mode == 2) {
        battleType = sys->param.unk4 + 0x1c;
    } else {
        if (sys->mode == 0) {
            index = 0;
        } else if (sys->mode == 1) {
            index = 1;
        } else {
            GFL_ASSERT(0);
        }
        regulation = func_0200ae14(func_0200b1d4(getTournamentBlockAddress(GameData_GetSaveControl(sys->param.gameData)), index));
        param0D = func_0200ae54(regulation, 0xd);
        value = func_0200ae54(regulation, 0x15);
        if (param0D >= 2) {
            param0D = 1;
        }
        battleType = value * 2 + 0x21 + param0D;
    }
    param->gameData = sys->param.gameData;
    param->unk4 = TRUE;
    param->unk8 = FALSE;
    param->unk10 = battleType;
    param->unk14 = 0;
    if (sys->mode == 1) {
        param->unkC = TRUE;
    } else {
        param->unkC = (sys->battleResult.unk10 != 0 || sys->battleResult.unk14 != 0) ? FALSE : TRUE;
    }
    if (func_0200ba38(sys->otherPlayer->unk44)) {
        param->unk4 = TRUE;
    } else {
        param->unk4 = FALSE;
    }
    return param;
}


// pokebw2's WifiBattleMatchSys_FinishResult
static BOOL func_ov199_0221320c(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    GFL_HeapFree(param);
    if (sys->mode == 1) {
        func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
    } else {
        sys->result = 3;
        func_ov199_02213808(chain, STEP_NET);
    }
    return TRUE;
}

// pokebw2's WifiBattleMatchSys_CreateLogoutParam
static void *func_ov199_02213234(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    WifiLogoutParam *param = GFL_HeapAllocate(heapId, sizeof(WifiLogoutParam), FALSE, "wifibattlematch_sys.c", 1714);

    MI_CpuFill8(param, 0, sizeof(WifiLogoutParam));
    param->gameData = sys->param.gameData;
    param->unk4 = 0;
    param->unk8 = 1;
    if (sys->unk24) {
        param->unk10 = 0x12;
    }
    return param;
}

// pokebw2's WifiBattleMatchSys_FinishLogout
static BOOL func_ov199_02213278(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    switch (sys->mode) {
    case 0:
        if (sys->unk24 != 0) {
            func_ov199_0221380c(chain);
        } else {
            func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
        }
        break;
    case 1:
        GFL_ASSERT(0);
        break;
    case 2:
    case 3:
        func_ov199_0221380c(chain);
        break;
    }
    GFL_HeapFree(param);
    return TRUE;
}

static void *func_ov199_022132c4(WifiBattleMatchChain *chain, HeapID heapId, WifiBattleMatchSys *sys) {
    BtlSetup *setup = GFL_HeapAllocate(heapId, sizeof(BtlSetup), FALSE, "wifibattlematch_sys.c", 1780);

    MI_CpuFill8(setup, 0, sizeof(BtlSetup));
    GFL_OvlLoad(OVERLAY_ID(93));
    BtlSetup_Reset(setup);
    func_020138c4(setup, sys->param.gameData, heapId);
    GFL_OvlUnload(OVERLAY_ID(93));
    func_0200ba58();
    func_ov188_021eeea8(setup, heapId);
    func_0200ba68();
    GFL_OvlLoad(OVERLAY_ID(93));
    func_02005d28();
    GFL_SndBGMPlay(setup->fieldSituation.bgm, SND_CHANNEL_MASK_ALL);
    return setup;
}


// pokebw2's WifiBattleMatchSys_FinishReplay
static BOOL func_ov199_02213338(WifiBattleMatchChain *chain, void *param, WifiBattleMatchSys *sys) {
    BtlSetup *setup = param;

    func_02005d28();
    GFL_HeapFree(setup->unk34[0]);
    func_02013930(setup);
    GFL_HeapFree(setup);
    GFL_OvlUnload(OVERLAY_ID(93));
    func_ov199_02213808(chain, STEP_CHAMPIONSHIP);
    return TRUE;
}

static inline void WifiBattleMatchSys_MemClear(void *dest, u32 size) {
    MI_CpuFill8(dest, 0, size);
}

// pokebw2's WifiBattleMatchSys_AllocPlayers
static void func_ov199_0221336c(WifiBattleMatchSys *sys, HeapID heapId) {
    sys->player = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchPlayer) + PokeParty_GetSaveDataSize(), FALSE,
                                   "wifibattlematch_sys.c", 1837);
    WifiBattleMatchSys_MemClear(sys->player, sizeof(WifiBattleMatchPlayer) + PokeParty_GetSaveDataSize());
    sys->otherPlayer = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchPlayer) + PokeParty_GetSaveDataSize(), FALSE,
                                        "wifibattlematch_sys.c", 1839);
    WifiBattleMatchSys_MemClear(sys->otherPlayer, sizeof(WifiBattleMatchPlayer) + PokeParty_GetSaveDataSize());
}

// pokebw2's WifiBattleMatchSys_FreePlayers
static void func_ov199_022133d0(WifiBattleMatchSys *sys) {
    GFL_HeapFree(sys->player);
    GFL_HeapFree(sys->otherPlayer);
}

void func_ov199_022133e4(WifiBattleMatchReport *report, PokeParty *party, PokeParty *otherParty,
                                WifiBattleMatchPlayer *otherPlayer, Regulation *regulation, u32 unk14,
                                BtlSetup *setup, WifiBattleMatchBattleResult *battleResult, u32 unk10) {
    RTCDate date;
    RTCTime time;
    u8 alive[2];
    PartyPkm *pkm;
    BOOL ret;
    int i;

    MI_CpuFill8(report, 0, sizeof(WifiBattleMatchReport));
    report->profileId = func_0200853c(&otherPlayer->info);
    report->unk4 = otherPlayer->unk48;
    for (i = 0; i < PokeParty_GetPkmCount(otherParty); i++) {
        pkm = PokeParty_GetPkm(otherParty, i);
        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            report->species[i] = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
            report->forms[i] = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            report->levels[i] = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
            report->genders[i] = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
        }
    }
    for (i = 0; i < PokeParty_GetPkmCount(party); i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            report->ownSpecies[i] = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
            report->ownForms[i] = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            report->ownLevels[i] = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
            report->ownGenders[i] = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
        }
    }
    ret = DWC_GetDateTime(&date, &time);
    GFL_ASSERT(ret);
    report->year = date.year;
    report->month = date.month;
    report->day = date.day;
    report->hour = time.hour;
    report->minute = time.minute;
    report->unk48 = unk14;
    report->result = setup->unk8C;
    func_ov199_022135cc(setup, &alive[0], &alive[1]);
    report->aliveOwn = alive[0];
    report->aliveOther = alive[1];
    report->battleType = func_0200ae54(regulation, 0x15);
    report->unkFlag = func_0200ae54(regulation, 0xd);
}


void func_ov199_022135cc(const BtlSetup *setup, u8 *own, u8 *other) {
    int i;
    int j;
    int count;
    u32 client;
    u32 index;

    *own = 0;
    *other = 0;
    client = setup->fieldSituation.unk19 & 1;
    for (i = 0; i <= 1; i++) {
        if ((i & 1) == client) {
            if (client) {
                index = (i & 2) + 1;
            } else {
                index = i & 2;
            }
            count = PokeParty_GetPkmCount(setup->party[index]);
            for (j = 0; j < count; j++) {
                if (setup->unkC8[index][j] != 2) {
                    (*own)++;
                }
            }
        } else {
            if (client) {
                index = i & 2;
            } else {
                index = (i & 2) + 1;
            }
            count = PokeParty_GetPkmCount(setup->party[index]);
            for (j = 0; j < count; j++) {
                if (setup->unkC8[index][j] != 2) {
                    (*other)++;
                }
            }
        }
    }
}


void func_ov199_02213684(const BtlSetup *setup, u8 *own, u8 *other) {
    int i;
    u32 client = setup->fieldSituation.unk19 & 1;
    u32 index;

    *own = 0;
    *other = 0;
    for (i = 0; i <= 1; i++) {
        if ((i & 1) == client) {
            if (client) {
                index = (i & 2) + 1;
            } else {
                index = i & 2;
            }
            *own = setup->unkE0[index];
        } else {
            if (client) {
                index = i & 2;
            } else {
                index = (i & 2) + 1;
            }
            *other = setup->unkE0[index];
        }
    }
}


static WifiBattleMatchChain *func_ov199_022136e4(const WifiBattleMatchStep *steps, u32 stepCount, WifiBattleMatchSys *sys, HeapID heapId) {
    WifiBattleMatchChain *chain = GFL_HeapAllocate(heapId, sizeof(WifiBattleMatchChain), FALSE, "wifibattlematch_sys.c", 2118);

    MI_CpuFill8(chain, 0, sizeof(WifiBattleMatchChain));
    chain->procManager = CreateGameProcManager(heapId);
    chain->sys = sys;
    chain->steps = steps;
    chain->stepCount = stepCount;
    chain->heapId = heapId;
    return chain;
}

// pokebw2's WifiBattleMatchSys_ChainFree
static void func_ov199_0221372c(WifiBattleMatchChain *p_wk) {
    GFL_ASSERT(p_wk->p_proc_param == NULL);
    FreeGameProcManager(p_wk->procManager);
    GFL_HeapFree(p_wk);
}

// pokebw2's WifiBattleMatchSys_ChainUpdate
static BOOL func_ov199_02213754(WifiBattleMatchChain *chain) {
    const WifiBattleMatchStep *step;

    switch (chain->state) {
    case 0:
        chain->previous = chain->current;
        chain->current = chain->next;
        chain->state = 1;
        break;
    case 1:
        step = &chain->steps[chain->current];
        if (step->create != NULL) {
            chain->p_proc_param = step->create(chain, chain->heapId, chain->sys);
        } else {
            chain->p_proc_param = NULL;
        }
        QueueGameProc(chain->procManager, chain->steps[chain->current].overlayId,
                      chain->steps[chain->current].functions, chain->p_proc_param);
        chain->state = 2;
        break;
    case 2:
        chain->procResult = GFL_ProcMgrUpdate(chain->procManager);
        if (chain->procResult == 0) {
            chain->state = 3;
        }
        break;
    case 3:
        step = &chain->steps[chain->current];
        if (step->finish != NULL) {
            if (!step->finish(chain, chain->p_proc_param, chain->sys)) {
                break;
            }
            chain->p_proc_param = NULL;
            chain->state = 4;
        } else {
            chain->state = 4;
        }
        break;
    case 4:
        if (chain->next == 0xff) {
            chain->state = 5;
        } else {
            chain->state = 0;
        }
        break;
    case 5:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiBattleMatchSys_ChainGetResult
static u32 func_ov199_02213800(WifiBattleMatchChain *chain) {
    return chain->procResult;
}

// pokebw2's WifiBattleMatchSys_ChainGetPrevious
static u8 func_ov199_02213804(WifiBattleMatchChain *chain) {
    return chain->previous;
}

// pokebw2's WifiBattleMatchSys_ChainSetNext
static void func_ov199_02213808(WifiBattleMatchChain *chain, u8 step) {
    chain->next = step;
}

// pokebw2's WifiBattleMatchSys_ChainEnd
static void func_ov199_0221380c(WifiBattleMatchChain *chain) {
    chain->next = 0xff;
}
