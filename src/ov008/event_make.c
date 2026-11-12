// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov010/event_make.c, ported by tools/port_file.py.
#include "types.h"
#include "app/comm_btl_demo.h"
#include "battle/battle_proc.h"
#include "battle/battle_rec_tool.h"
#include "battle/btl_net.h"
#include "battle/btl_setup.h"
#include "field/bsubway_scr.h"
#include "field/event_make.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "save/save_control.h"
#include "system/game_event.h"
#include "system/game_system.h"

#define HEAPID_BATTLE_PROC 0x76

const GameProcFunctions data_ov008_0215513c = { func_ov008_02154ca0, func_ov008_02154cf8, func_ov008_02154cc8 };

BOOL func_ov008_02154ca0(GameProc *proc, u32 *state, void *param, void *work) {
    BattleProcWork *procWork;

    GFL_HeapCreateChild(1, HEAPID_BATTLE_PROC, 0x4000);
    procWork = GFL_ProcInitSubsystem(proc, sizeof(BattleProcWork), HEAPID_BATTLE_PROC);
    procWork->manager = CreateGameProcManager(HEAPID_BATTLE_PROC);
    return TRUE;
}

BOOL func_ov008_02154cc8(GameProc *proc, u32 *state, void *param, void *work) {
    BattleParam *battle = param;
    BattleProcWork *procWork = work;

    if (battle->unk14 == 1 && func_0200fb24()) {
        func_0200fb38();
    }
    FreeGameProcManager(procWork->manager);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BATTLE_PROC);
    return TRUE;
}

BOOL func_ov008_02154cf8(GameProc *proc, u32 *state, void *param, void *work) {
    BattleParam *battle = param;
    BattleProcWork *procWork = work;
    BOOL running = GFL_ProcMgrUpdate(procWork->manager);
    s32 last;
    s32 first;
    s32 second;
    s32 client;
    s32 source;
    s32 i;
    s32 j;
    BOOL saved;

    if (running != TRUE && func_0200fb24() && (s32)*state >= 0 && (s32)*state < 13) {
        if (procWork->commandsSet == TRUE) {
            func_0203a5e4(0x100);
        }
        if (procWork->ov167Loaded == TRUE) {
            GFL_OvlUnload(OVERLAY_ID(93));
        }
        if (procWork->vsPlayerLoaded == TRUE) {
            freeVSPlayerBlkClearPtr();
        }
        return TRUE;
    }
    switch (*state) {
    case 0:
        if (battle->setup->fieldSituation.unk1a == 0) {
            battle->players->unk44 = 1;
        } else {
            battle->players->unk44 = 3;
        }
        QueueGameProc(procWork->manager, OVERLAY_ID(213), &data_ov213_021bb0b0, battle->players);
        *state = 1;
        break;
    case 1:
        if (running == TRUE) {
            break;
        }
        if (battle->setup->fieldSituation.unk1a == 0) {
            *state = 6;
        } else {
            *state = 2;
        }
        break;
    case 2:
        func_0203d594(func_020399a8(), 0xc9);
        *state = 3;
        break;
    case 3:
        if (func_0203d5a0(func_020399a8(), 0xc9) == 0) {
            break;
        }
        *state = 4;
        break;
    case 4:
        if (battle->setup->fieldSituation.unk19 <= 1) {
            procWork->ov171Param.flipped = 0;
            procWork->ov171Param.party0 = battle->players->players[0].party;
            procWork->ov171Param.party1 = battle->players->players[1].party;
        } else {
            procWork->ov171Param.flipped = 1;
            procWork->ov171Param.party0 = battle->players->players[1].party;
            procWork->ov171Param.party1 = battle->players->players[0].party;
        }
        QueueGameProc(procWork->manager, OVERLAY_ID(97), &data_ov097_021f75ec, &procWork->ov171Param);
        *state = 5;
        break;
    case 5:
        if (running == TRUE) {
            break;
        }
        *state = 6;
        break;
    case 6:
        procWork->ov167Loaded = TRUE;
        GFL_OvlLoad(OVERLAY_ID(93));
        procWork->commandsSet = TRUE;
        func_0203a574(0x100, data_ov093_021f03c4, 9, NULL);
        func_0203d594(func_020399a8(), 0xc8);
        *state = 7;
        break;
    case 7:
        if (func_0203d5a0(func_020399a8(), 0xc8) == 0) {
            break;
        }
        *state = 8;
        break;
    case 8:
        procWork->vsPlayerLoaded = TRUE;
        func_0200b400(HEAPID_BATTLE_PROC);
        QueueGameProc(procWork->manager, -1, &data_ov093_021efdac, battle->setup);
        *state = 9;
        break;
    case 9:
        if (running == TRUE) {
            break;
        }
        *state = 10;
        break;
    case 10:
        *state = 11;
        break;
    case 11:
        func_0203d594(func_020399a8(), 0xca);
        *state = 12;
        break;
    case 12:
        if (func_0203d5a0(func_020399a8(), 0xca) == 0) {
            break;
        }
        func_0203a5e4(0x100);
        procWork->commandsSet = FALSE;
        GFL_OvlUnload(OVERLAY_ID(93));
        procWork->ov167Loaded = FALSE;
        func_0200ba58();
        func_ov188_021eee78(battle->setup);
        func_0200ba68();
        *state = 13;
        break;
    case 13:
        if (battle->setup->fieldSituation.unk1a == 0) {
            battle->players->unk44 = 2;
        } else {
            battle->players->unk44 = 4;
        }
        switch (battle->setup->unk8C) {
        case 1:
        case 4:
            battle->players->result = 0;
            break;
        case 0:
        case 3:
            battle->players->result = 1;
            break;
        case 2:
            battle->players->result = 2;
            break;
        case 6:
            break;
        default:
            battle->players->result = 2;
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
                source = (client & 1) ? (i & 2) + 1 : (i & 2);
                for (j = 0; j < 6; j++) {
                    battle->players->players[first + (i >> 1)].order[j] = battle->setup->unkC8[source][j];
                }
            } else {
                source = (client & 1) ? (i & 2) : (i & 2) + 1;
                for (j = 0; j < 6; j++) {
                    battle->players->players[second + (i >> 1)].order[j] = battle->setup->unkC8[source][j];
                }
            }
        }
        if (battle->setup->unk8C == 6) {
            *state = 16;
            break;
        }
        QueueGameProc(procWork->manager, OVERLAY_ID(213), &data_ov213_021bb0b0, battle->players);
        *state = 14;
        break;
    case 14:
        if (running == TRUE) {
            break;
        }
        *state = 15;
        break;
    case 15:
        if (battle->setup->unk94 == NULL) {
            saved = FALSE;
        } else {
            saved = func_0200ba38(battle->setup->unk91) != 0;
        }
        battle->ov306Param.gameData = battle->gameData;
        battle->ov306Param.unk4 = saved;
        battle->ov306Param.unk8 = 1;
        battle->ov306Param.unkC = 1;
        battle->ov306Param.unk10 = battle->rule;
        battle->ov306Param.unk14 = battle->unk10;
        QueueGameProc(procWork->manager, OVERLAY_ID(214), &data_ov214_021bb474, &battle->ov306Param);
        *state = 16;
        break;
    case 16:
        if (running == TRUE) {
            break;
        }
        freeVSPlayerBlkClearPtr();
        procWork->vsPlayerLoaded = FALSE;
        *state = 17;
        break;
    case 17:
        return TRUE;
    }
    return FALSE;
}


GameEvent *eventMakeFunc(GameSystem *gsys, void *data) {
    const EventMakeArgs *args = data;

    return func_ov008_021550b0(gsys, args->setup, args->players, args->unk08);
}

GameEvent *func_ov008_021550b0(GameSystem *gsys, BtlSetup *setup, BattlePlayers *players, u32 a3) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov008_021550dc, sizeof(EventMakeWork));
    EventMakeWork *work = GameEvent_GetData(event);

    work->gsys = gsys;
    work->setup = setup;
    work->players = players;
    work->unk0C = a3;
    return event;
}

GameEventReturnCode func_ov008_021550dc(GameEvent *event, u32 *state, void *data) {
    EventMakeWork *work = data;
    GameSystem *gsys = work->gsys;
    BattleParam *param;

    switch (*state) {
    case 0:
        param = &work->param;
        work->param.gameData = GSYS_GetGameData(gsys);
        param->setup = work->setup;
        param->players = work->players;
        param->rule = work->players->rule;
        param->unk10 = work->players->unk4C;
        param->unk14 = work->unk0C;
        GSYS_QueueProc(gsys, -1, &data_ov008_0215513c, param);
        (*state)++;
        break;
    case 1:
        if (!GSYS_GetProcMgrState(gsys)) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}
