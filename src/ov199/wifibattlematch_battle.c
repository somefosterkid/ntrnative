// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov290/wifibattlematch_battle.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifibattlematch_battle.c, in overlay 290 (a guessed name; the file has no string): the proc that runs a battle of
// the Battle Competition, for wifibattlematch_sys.c's battle step

#include "types.h"
#include "app/battle_championship_flow.h"
#include "app/comm_btl_demo.h"
#include "app/livebattlematch_irc.h"
#include "app/wifi_login.h"
#include "app/wifibattlematch.h"
#include "app/wifibattlematch_battle.h"
#include "battle/battle_proc.h"
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
#include "gfl/dwc_rap.h"

// The heap of the proc that runs the battle
#define HEAPID_WIFIBATTLEMATCH_BATTLE 0x58

// GFLNetInitData.bNetType of an infrared connection
#define NET_TYPE_IRC 3

// The battle proc's work
typedef struct {
    GameProcManager *manager;
    u32 timer;
    BOOL ov167Loaded;
    BOOL commandsSet;
} WifiBattleMatchBattleWork;

static BOOL func_ov199_02213814(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov199_02213850(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov199_02213894(GameProc *proc, u32 *state, void *param, void *work);


// pokebw2's WIFIBATTLEMATCH_BATTLE_PROC_FUNCTIONS
const GameProcFunctions data_ov199_02213ce8 = {
    func_ov199_02213814,
    func_ov199_02213894,
    func_ov199_02213850,
};

// pokebw2's WifiBattleMatchSys_BattleProcInit
static BOOL func_ov199_02213814(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchBattleParam *battle = param;
    WifiBattleMatchBattleWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_WIFIBATTLEMATCH_BATTLE, 0x150);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WifiBattleMatchBattleWork), HEAPID_WIFIBATTLEMATCH_BATTLE);
    MI_CpuFill8(wk, 0, sizeof(WifiBattleMatchBattleWork));
    battle->result = 0;
    wk->manager = CreateGameProcManager(HEAPID_WIFIBATTLEMATCH_BATTLE);
    GFL_SndBGMFadeOut(0x10);
    return TRUE;
}

static BOOL func_ov199_02213850(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchBattleWork *wk = work;

    if (wk->ov167Loaded) {
        GFL_OvlUnload(OVERLAY_ID(93));
        wk->ov167Loaded = FALSE;
    }
    if (wk->commandsSet) {
        func_0203a5e4(0x100);
        wk->commandsSet = FALSE;
    }
    FreeGameProcManager(wk->manager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_WIFIBATTLEMATCH_BATTLE);
    return TRUE;
}

static BOOL func_ov199_02213894(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchBattleParam *battle = param;
    WifiBattleMatchBattleWork *wk = work;
    int running;
    BOOL queue;
    int last;
    u32 first;
    u32 second;
    u32 client;
    u32 source;
    int i;
    int j;

    running = GFL_ProcMgrUpdate(wk->manager);
    switch (*state) {
    case 0:
        GFL_SndBGMPlay(battle->setup->fieldSituation.bgm, SND_CHANNEL_MASK_ALL);
        battle->players->base.unk44 = 1;
        QueueGameProc(wk->manager, OVERLAY_ID(213), &data_ov213_021bb0b0, battle->players);
        running = 2;
        *state = 1;
        break;
    case 1:
        if (running == TRUE) {
            break;
        }
        *state = 2;
        break;
    case 2:
        GFL_OvlLoad(OVERLAY_ID(93));
        wk->ov167Loaded = TRUE;
        func_0203a574(0x100, data_ov093_021f03c4, 9, NULL);
        wk->commandsSet = TRUE;
        func_02039c68(func_020399a8(), 0xc8, 0x24);
        *state = 3;
        break;
    case 3:
        if (func_02039cb4(func_020399a8(), 0xc8, 0x24)) {
            *state = 4;
        }
        break;
    case 4:
        QueueGameProc(wk->manager, OVERLAY_NONE, &data_ov093_021efdac, battle->setup);
        running = 2;
        *state = 5;
        break;
    case 5:
        if (running == TRUE) {
            break;
        }
        *state = 6;
        break;
    case 6:
        *state = 7;
        break;
    case 7:
        func_02039c68(func_020399a8(), 0xd2, 0x24);
        wk->timer = 0;
        *state = 8;
        break;
    case 8:
        if (func_02039cb4(func_020399a8(), 0xd2, 0x24)) {
            func_0203a5e4(0x100);
            wk->commandsSet = FALSE;
            GFL_OvlUnload(OVERLAY_ID(93));
            wk->ov167Loaded = FALSE;
            *state = 9;
        }
        if (func_0203cd98()) {
            if (func_0203d798()->bNetType == NET_TYPE_IRC) {
                if (wk->timer++ > 600) {
                    func_0203a5e4(0x100);
                    wk->commandsSet = FALSE;
                    GFL_OvlUnload(OVERLAY_ID(93));
                    wk->ov167Loaded = FALSE;
                    *state = 9;
                    wk->timer = 0;
                }
            }
        }
        break;
    case 9:
        func_02039c68(func_020399a8(), 0xd3, 0x24);
        *state = 10;
        wk->timer = 0;
        break;
    case 10:
        if (func_02039cb4(func_020399a8(), 0xd3, 0x24)) {
            *state = 11;
        }
        if (func_0203cd98()) {
            if (func_0203d798()->bNetType == NET_TYPE_IRC) {
                if (wk->timer++ > 600) {
                    *state = 11;
                    wk->timer = 0;
                }
            }
        }
        break;
    case 11:
        queue = TRUE;
        if (battle->setup->fieldSituation.unk1a == 0) {
            battle->players->base.unk44 = 2;
        } else {
            battle->players->base.unk44 = 4;
        }
        switch (battle->setup->unk8C) {
        case 1:
        case 4:
            battle->players->base.result = 0;
            break;
        case 0:
        case 3:
            battle->players->base.result = 1;
            break;
        case 2:
            battle->players->base.result = 2;
            break;
        case 6:
            wk->timer = 0;
            queue = FALSE;
            *state = 15;
            break;
        default:
            battle->players->base.result = 2;
            break;
        }
        if (battle->setup->fieldSituation.unk1a == 0) {
            last = 1;
            first = 0;
            second = 1;
        } else {
            last = 3;
            first = 0;
            second = 2;
        }
        client = battle->setup->fieldSituation.unk19;
        for (i = 0; i <= last; i++) {
            if ((i & 1) == (client & 1)) {
                if (client & 1) {
                    source = (i & 2) + 1;
                } else {
                    source = i & 2;
                }
                for (j = 0; j < 6; j++) {
                    battle->players->base.players[first + (i >> 1)].order[j] = battle->setup->unkC8[source][j];
                }
            } else {
                if (client & 1) {
                    source = i & 2;
                } else {
                    source = (i & 2) + 1;
                }
                for (j = 0; j < 6; j++) {
                    battle->players->base.players[second + (i >> 1)].order[j] = battle->setup->unkC8[source][j];
                }
            }
        }
        if (queue) {
            QueueGameProc(wk->manager, OVERLAY_ID(213), &data_ov213_021bb0b0, battle->players);
            running = 2;
            *state = 12;
        }
        break;
    case 12:
        if (running == TRUE) {
            break;
        }
        *state = 14;
        break;
    case 13:
        break;
    case 14:
        return TRUE;
    case 15:
        if (wk->timer++ > 1800) {
            func_0200fb38();
            battle->result = 2;
            return TRUE;
        }
        break;
    }
    if (func_0203cd98() && running != TRUE) {
        if (func_0203d798()->bNetType == NET_TYPE_IRC) {
            if (func_0200fb24()) {
                func_0200fb38();
                battle->result = 1;
                return TRUE;
            }
        } else {
            switch (func_ov009_02157170(TRUE, TRUE)) {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
            default:
                battle->result = 2;
                return TRUE;
            case 4:
                battle->result = 3;
                return TRUE;
            }
        }
    }
    return FALSE;
}



