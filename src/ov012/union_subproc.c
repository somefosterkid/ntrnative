// The Union Room's commands (a menu's choice, such as a trade or a battle), each run as a field event, and the event
// that runs them: pokebw2's src/ov028/union_subproc.c (GPL-3.0, https://github.com/fuddlesworth/pokebw2), in its
// order. Black's code is 0x0217413c-0x02174b40, its command table at 0x02175498 and the "" string that its asserts
// share at 0x02175628

#include "types.h"
#include "field/union_comm.h"
#include "field/union_main.h"
#include "gfl/std.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "field/field_event.h"
#include "field/colosseum.h"
#include "field/union_oneself.h"
#include "field/union_msg.h"
#include "app/pokemon_trade.h"
#include "demo/shinka_demo.h"
#include "gfl/net.h"
#include "system/game_system.h"
#include "system/game_data.h"
#include "app/oekaki.h"
#include "app/guru2.h"
#include "gfl/sound.h"
#include "field/field_sound.h"
#include "field/event_mapchange.h"
#include "constants/zones.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "gfl/net_handle.h"
#include "app/p_status.h"
#include "app/pokelist.h"
#include "field/event_colosseum_battle.h"
#include "gfl/heap.h"
#include "gfl/net_sync.h"
#include "gfl/overlay.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "save/wifi_list.h"

// pokebw2's UnionCommandEvent: the event that runs a command
typedef struct {
    GameSystem *gsys;
    Field *field;
    UnionSystem *unisys;
} UnionCommandEvent;

GameEventReturnCode func_ov012_0217416c(GameEvent *event, u32 *seq, void *work);
extern const GameProcFunctions data_ov010_02172608;
extern u32 OVERLAY_111_ID[];
extern const GameProcFunctions data_ov110_021c39d4;

BOOL func_ov012_02174284(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
BOOL func_ov012_021742bc(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event, GameEvent **next, u8 *step);
BOOL func_ov012_021743a4(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event, GameEvent **next, u8 *step);
BOOL func_ov012_02174654(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
BOOL func_ov012_021746a8(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
BOOL func_ov012_021746fc(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);
BOOL func_ov012_02174744(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step);

// A command: its function, and the activities the player is set to before and after it
typedef struct UnionCommandInfo {
    BOOL (*func)(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event, GameEvent **next,
                 u8 *step);
    u8 startActivity;
    u8 endActivity;
} UnionCommandInfo;

static const UnionCommandInfo data_ov012_02175498[] = {
#include "asset/union_subproc_data_ov012_02175498.inc"
};

GameEvent *func_ov012_0217413c(GameSystem *gsys, Field *field, UnionSystem *sys) {
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov012_0217416c, sizeof(UnionCommandEvent));
    UnionCommandEvent *data = GameEvent_GetData(event);

    data->gsys = gsys;
    data->field = field;
    data->unisys = sys;
    sys->subproc.active = TRUE;
    return event;
}

GameEventReturnCode func_ov012_0217416c(GameEvent *event, u32 *seq, void *work) {
    UnionCommandEvent *data = work;
    GameSystem *gsys = data->gsys;
    UnionSystem *unisys = data->unisys;
    UnionSubproc *subproc = &unisys->subproc;
    GameEvent *next = NULL;
    BOOL done;

    switch (*seq) {
    case 0:
        func_ov019_0217f074(unisys);
        if (data_ov012_02175498[subproc->id].startActivity != UNION_ACTIVITY_NONE) {
            func_ov012_021734a8(unisys, 4, data_ov012_02175498[subproc->id].startActivity);
        }
        (*seq)++;
    case 1:
        done = data_ov012_02175498[subproc->id].func(gsys, unisys, data->field, subproc->arg, event, &next,
                                                       &subproc->step);
        if (next != NULL) {
            GameEvent_ChainNext(event, next);
        } else if (done == TRUE) {
            (*seq)++;
        }
        break;
    case 2:
        if (data_ov012_02175498[subproc->id].endActivity != UNION_ACTIVITY_NONE) {
            func_ov012_021734a8(unisys, 4, data_ov012_02175498[subproc->id].endActivity);
        }
        subproc->id = 0;
        subproc->arg = NULL;
        subproc->step = 0;
        subproc->active = FALSE;
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov012_0217422c(UnionSystem *sys, u32 id, void *arg) {
    GFL_ASSERT(sys->subproc.id == 0 && sys->subproc.active == 0);
    (sys->subproc).id = id;
    (sys->subproc).arg = arg;
    (sys->subproc).step = 0;
}

BOOL func_ov012_02174268(UnionSystem *sys) {
    if (((sys->subproc).id == 0) && ((sys->subproc).active == 0)) {
        return 0;
    }
    return 1;
}

BOOL func_ov012_02174284(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **out, u8 *state) {
    if (*state == 0) {
        *out = EventFieldSubprocessTransition_Create(gsys, field, OVERLAY_ID(111), &data_ov010_02172608, param);
    } else {
        return TRUE;
    }
    (*state)++;
    return FALSE;
}

BOOL func_ov012_021742bc(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step) {
    PokemonTradeParam *trade = param;
    ShinkaDemoParam *evolution;

    switch (*step) {
    case 0:
        *next = CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0);
        (*step)++;
        break;
    case 1:
        *next = CreateFieldCloseEvent(gsys, field);
        (*step)++;
        break;
    case 2:
        GSYS_QueueProc(gsys, OVERLAY_POKEMONTRADE, &data_ov119_021e7608, trade);
        (*step)++;
        break;
    case 3:
        if (GSYS_GetProcMgrState(gsys) != 0) {
            break;
        }
        if (trade->next == TRADE_NEXT_EVOLVE) {
            *step = 4;
        } else {
            *step = 6;
        }
        break;
    case 4:
        evolution = trade->evolution;
        evolution->gameData = trade->gameData;
        evolution->party = trade->party;
        evolution->species = trade->evolveSpecies;
        evolution->partyIndex = 0;
        evolution->method = trade->evolveMethod;
        evolution->unkC = 1;
        evolution->canCancel = FALSE;
        trade->evolution = evolution;
        GSYS_QueueProc(gsys, OVERLAY_SHINKA_DEMO, &data_ov195_02208c40, evolution);
        (*step)++;
        break;
    case 5:
        if (GSYS_GetProcMgrState(gsys) != 0) {
            break;
        }
        if (func_0200fb24()) {
            *step = 6;
        } else {
            trade->next = TRADE_NEXT_EVOLVE;
            *step = 2;
        }
        break;
    case 6:
        *next = EventFieldOpen_CreateHeadless(gsys);
        *step = 7;
        break;
    case 7:
        *next = CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0);
        *step = 8;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov012_021743a4(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event,
                         GameEvent **next, u8 *step) {
    UnionSelf *self = &unisys->situ;
    UnionSubproc *subproc = &unisys->subproc;
    UnionProcParam *param;
    GameEvent *chain;
    u32 joined;

    if (func_0200fb24() && *step > 2 && *step <= 13) {
        *step = 16;
    }
    if (unisys->alloc.uniapp != NULL) {
        func_ov022_02183c3c(unisys->alloc.uniapp, unisys);
    }
    switch (*step) {
    case 0:
        chain = CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 1:
        chain = CreateFieldCloseEvent(gsys, field);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 2:
        GFL_OvlLoad(OVERLAY_ID(22));
        unisys->alloc.uniapp = func_ov022_02183b60(unisys, 3, 5, GetGameDataPlayerInfo(unisys->param->gameData));
        if (self->mycomm.unk46_1 == 1) {
            *step = 5;
        } else {
            (*step)++;
        }
        break;
    case 3:
        func_02039c68(func_020399a8(), 9, 0x14);
        (*step)++;
        break;
    case 4:
        if (func_02039cb4(func_020399a8(), 9, 0x14) == TRUE) {
            *step = 7;
        }
        break;
    case 5:
        if (func_ov012_02173e1c() == TRUE) {
            (*step)++;
        }
        break;
    case 6:
        if (func_ov022_02183f84(unisys->alloc.uniapp) == TRUE) {
            *step = 7;
        }
        break;
    case 7:
        if (func_ov012_02173ec8() == TRUE) {
            (*step)++;
        }
        break;
    case 8:
        if (func_ov022_0218401c(unisys->alloc.uniapp) == TRUE) {
            (*step)++;
        }
        break;
    case 9:
        joined = func_ov022_021841a0(unisys->alloc.uniapp);
        if (func_ov012_02173f90(joined, GetGameDataPlayerInfo(unisys->param->gameData)) == TRUE) {
            (*step)++;
        }
        break;
    case 10:
        if (func_ov022_0218402c(unisys->alloc.uniapp) == TRUE) {
            (*step)++;
        }
        break;
    case 11:
        if (self->mycomm.unk46_1 == 1) {
            joined = func_ov022_021841a0(unisys->alloc.uniapp);
            if (func_ov012_02174084(joined, GetGameDataPlayerInfo(unisys->param->gameData)) == TRUE) {
                func_ov022_021840ac(unisys->alloc.uniapp);
                *step = 13;
            }
        } else {
            func_ov012_021736e0(self->activity);
            func_02039c68(func_020399a8(), 10, 0x14);
            *step = 12;
        }
        break;
    case 12:
        if (func_02039cb4(func_020399a8(), 10, 0x14) == TRUE) {
            func_0203d1e8(func_020399a8(), TRUE);
            *step = 13;
        }
        break;
    case 13:
        func_0203d7bc(FALSE);
        if (self->activity == 3) {
            param = subproc->arg;
            param->app = unisys->alloc.uniapp;
            GSYS_QueueProc(gsys, OVERLAY_ID(23), &data_ov023_02186b30, param);
        } else {
            param = subproc->arg;
            param->app = unisys->alloc.uniapp;
            GSYS_QueueProc(gsys, OVERLAY_ID(137), &data_ov137_021e1a2c, param);
        }
        (*step)++;
        break;
    case 14:
        if (GSYS_GetProcMgrState(gsys) != 0) {
            break;
        }
        if (self->activity == 0x19) {
            param = subproc->arg;
            if (param->unk14 == 1) {
                func_02005da4((u16)GetMapBGMIDByPlayerState2(unisys->param->gameData, 0x1a6,
                                                             GameData_GetSeason(unisys->param->gameData)),
                              0xffff, 6, 0x3c);
                *step = 15;
            } else {
                *step = 16;
            }
        } else {
            *step = 16;
        }
        break;
    case 15:
        if (GFL_SndBGMIsFading()) {
            break;
        }
        *step = 16;
        break;
    case 16:
        if (unisys->alloc.uniapp != NULL) {
            func_ov022_02183be0(unisys->alloc.uniapp);
            unisys->alloc.uniapp = NULL;
            GFL_OvlUnload(OVERLAY_ID(22));
        }
        *step = 17;
        break;
    case 17:
        chain = EventFieldOpen_CreateHeadless(gsys);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 18:
        chain = CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 19:
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov012_02174654(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event,
                         GameEvent **next, u8 *step) {
    VecFx32 pos;

    if (*step == 0) {
        pos.x = (func_0203d1a4(func_020399a8()) * 16 + 88) << FX32_SHIFT;
        pos.y = 0;
        pos.z = FX32_CONST(152);
        *next = EventMapChangeWarp_CreateGrid(gsys, field, ZONE_UNION_ROOM_COLOSSEUM, &pos, 0, FALSE);
    } else {
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

BOOL func_ov012_021746a8(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event,
                         GameEvent **next, u8 *step) {
    VecFx32 pos;

    if (*step == 0) {
        pos.x = (func_0203d1a4(func_020399a8()) * 16 + 88) << FX32_SHIFT;
        pos.y = 0;
        pos.z = FX32_CONST(152);
        *next = EventMapChangeWarp_CreateGrid(gsys, field, ZONE_UNION_ROOM_COLOSSEUM_2, &pos, 0, FALSE);
    } else {
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

BOOL func_ov012_021746fc(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event,
                         GameEvent **next, u8 *step) {
    VecFx32 pos;

    if (*step == 0) {
        pos.x = FX32_CONST(184);
        pos.y = 0;
        pos.z = FX32_CONST(232);
        *next = EventMapChangeWarp_CreateGrid(gsys, field, ZONE_UNION_ROOM, &pos, 0, FALSE);
    } else {
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

BOOL func_ov012_02174744(GameSystem *gsys, UnionSystem *unisys, Field *field, void *arg, GameEvent *event,
                         GameEvent **next, u8 *step) {
    UnionColosseumParam *param = arg;
    UnionColosseum *colosseum = unisys->colosseum_sys;
    UnionSelf *self = &unisys->situ;
    PStatusParam *status = &param->status;
    ColosseumBattleParam *battle = param->battle;
    u32 selfId = func_0203d1a4(func_020399a8());
    PokeParty *tmpParty;
    PokeParty *party;
    GameEvent *chain;
    WifiList *wifiList;
    u32 friendIndex;
    int count;
    u32 isRedTeam;
    int i;

    if (*step > 2 && *step < 7) {
        func_ov090_021b6114(&param->list);
    }
    switch (*step) {
    case 0:
        chain = CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 1:
        chain = CreateFieldCloseEvent(gsys, field);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 2:
        GFL_OvlLoad(OVERLAY_ID(90));
        func_ov090_021b6100(&param->list);
        (*step)++;
        break;
    case 3:
        GSYS_QueueProc(gsys, OVERLAY_ID(91), &data_ov091_021bed00, param);
        (*step)++;
        break;
    case 4:
        if (GSYS_GetProcMgrState(gsys) != 0) {
            break;
        }
        if (param->list.result == 1) {
            *step = 5;
        } else {
            *step = 7;
        }
        break;
    case 5:
        status->party = param->list.party;
        status->trainerData = param->list.trainerData;
        status->gameData = GSYS_GetGameData(gsys);
        status->dataType = PSTATUS_DATA_PARTY;
        status->partyCount = PokeParty_GetPkmCount(param->list.party);
        status->mode = PSTATUS_MODE_NORMAL;
        status->partyIndex = param->list.index;
        status->page = PSTATUS_PAGE_INFO;
        status->isNationalDex = PokeDex_IsNationalObtained(GameData_GetPokedex(status->gameData));
        GSYS_QueueProc(gsys, OVERLAY_PSTATUS, &data_ov131_021dc800, status);
        (*step)++;
        break;
    case 6:
        if (GSYS_GetProcMgrState(gsys) != 0) {
            break;
        }
        param->list.index = status->partyIndex;
        *step = 3;
        break;
    case 7:
        func_ov090_021b6108(&param->list);
        GFL_OvlUnload(OVERLAY_ID(90));
        *step = 8;
        break;
    case 8:
        if (func_0200fb24()) {
            *step = 17;
            break;
        }
        GFL_ASSERT(param->list.result == 0);
        GFL_ASSERT(param->list.index == 6);
        party = colosseum->party[selfId];
        tmpParty = PokeParty_Create(0x41);
        PokeParty_Copy(party, tmpParty);
        PokeParty_Init(party);
        for (i = 0; i < 6; i++) {
            if (param->list.picked[i] == 0) {
                break;
            }
            PokeParty_AddPkm(party, PokeParty_GetPkm(tmpParty, param->list.picked[i] - 1));
        }
        GFL_HeapFree(tmpParty);
        func_ov012_02175240(colosseum, TRUE);
        func_02039c68(func_020399a8(), 22, 0x14);
        (*step)++;
        break;
    case 9:
        if (func_0200fb24()) {
            *step = 17;
            break;
        }
        if (func_02039cb4(func_020399a8(), 22, 0x14) == TRUE) {
            (*step)++;
        }
        break;
    case 10:
        if (func_0200fb24()) {
            *step = 17;
            break;
        }
        if (func_ov012_02174d98(colosseum->party[selfId]) == TRUE) {
            (*step)++;
        }
        break;
    case 11:
        if (func_0200fb24()) {
            *step = 17;
            break;
        }
        if (func_ov012_02175214(colosseum) == TRUE) {
            (*step)++;
        }
        break;
    case 12:
        battle->party = colosseum->party[selfId];
        battle->unk12 = colosseum->unk15C[selfId];
        battle->regulation = unisys->alloc.regulation;
        if (selfId == 0 || (colosseum->unk15C[0] & 2) == (colosseum->unk15C[selfId] & 2)) {
            battle->bgm = 0x48b;
        } else {
            battle->bgm = 0x48c;
        }
        func_ov012_02175280(colosseum, gsys, &battle->players, 0x41);
        *next = func_ov010_02157450(gsys, field, self->activity, battle);
        (*step)++;
        break;
    case 13:
        if (!func_0200fb24()) {
            wifiList = func_0201283c(GSYS_GetGameData(gsys));
            count = func_0203d1b4();
            isRedTeam = colosseum->unk15C[selfId] & 1;
            for (i = 0; i < count; i++) {
                if (i == selfId) {
                    continue;
                }
                if (func_02009d1c(wifiList, &colosseum->members[i].info, &friendIndex) != TRUE) {
                    continue;
                }
                if (isRedTeam == (colosseum->unk15C[i] & 1)) {
                    func_02009b80(wifiList, friendIndex);
                } else {
                    switch (battle->players.battle.result) {
                    case 0:
                        func_02009bb8(wifiList, friendIndex, 1, 0, 0);
                        break;
                    case 1:
                        func_02009bb8(wifiList, friendIndex, 0, 1, 0);
                        break;
                    case 2:
                        func_02009b80(wifiList, friendIndex);
                        break;
                    }
                }
            }
        }
        func_ov012_0217534c(&battle->players);
        *step = 16;
        break;
    case 14:
        chain = EventFieldOpen_CreateHeadless(gsys);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    case 15:
        chain = CallFieldMapEntranceInTransition(gsys, field, 0, 0, 1, 0, 0);
        GameEvent_ChainNext(event, chain);
        (*step)++;
        break;
    default:
        return TRUE;
    case 17:
        *step = 14;
        break;
    }
    return FALSE;
}

BOOL func_ov012_02174ad8(GameSystem *gsys, UnionSystem *unisys, Field *field, void *param, GameEvent *event,
                         GameEvent **next, u8 *step) {
    UnionSelf *self = &unisys->situ;

    if (*step == 0) {
        *next = func_ov010_02157450(gsys, field, self->activity, param);
    } else {
        return TRUE;
    }
    (*step)++;
    return FALSE;
}

BOOL func_ov012_02174b08(GameSystem *gsys, UnionSystem *sys, Field *field, void *param, GameEvent *event, GameEvent **next, u8 *step) {
    if (*step == 0) {
        *next = EventFieldSubprocessTransition_Create(gsys, field, OVERLAY_ID(110), &data_ov110_021c39d4, param);
    } else {
        return TRUE;
    }
    (*step)++;
    return FALSE;
}
