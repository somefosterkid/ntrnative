// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_server_flow.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_ability.h"
#include "battle/btl_action.h"
#include "battle/btl_action_order.h"
#include "battle/btl_display.h"
#include "battle/btl_event.h"
#include "battle/btl_faintrec.h"
#include "battle/btl_field.h"
#include "battle/btl_handler.h"
#include "battle/btl_handler_work.h"
#include "battle/btl_item.h"
#include "battle/btl_main.h"
#include "battle/btl_monset.h"
#include "battle/btl_move.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_pospoke.h"
#include "battle/btl_server.h"
#include "battle/btl_server_cmd.h"
#include "battle/btl_server_flow.h"
#include "battle/btl_server_flow_sub.h"
#include "battle/btl_tables.h"
#include "battle/btl_wazarec.h"
#include "battle/condition_effects.h"
#include "battle/pos_handlers.h"
#include "battle/side_handlers.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/tr_ai.h"
#include "constants/types.h"
#include "gfl/arc.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/comm_player_support.h"
#include "system/game_beacon.h"
#include "battle/btl_setup.h"
#include "constants/battle.h"
#include "gfl/net_system.h"

s32 ConvertConditionCode(BattleMon *mon, s32 *condition);




// The targets hit by one strike of a damaging move, as func_ov093_021c11f0 works through them
// pokebw2's sHitMons
static BattleMon *data_ov093_021f3b38[3];
// pokebw2's sHitEffectiveness
static u32 data_ov093_021f3b2c[3];
// pokebw2's sHitDamages
static u16 data_ov093_021f3b26[3];
// pokebw2's sHitUnk9
static u8 data_ov093_021f3b23[3];
// pokebw2's sHitCritical
static u8 data_ov093_021f3b20[3];

u32 func_ov093_021c7330(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021c76f4(BtlServerFlow *flow, BattleMon *mon, s32 param3);
void func_ov093_021c7780(BtlServerFlow *flow, BattleMon *mon);
u16 func_ov093_021c78f4(BtlServerFlow *flow, BattleMon *mon, BattleMon *source, u16 amount);
u32 func_ov093_021c7664(BtlServerFlow *flow, u16 move, BattleMon *attacker);
void func_ov093_021c7998(BtlServerFlow *flow, s32 param2);
void func_ov093_021c7c24(u16 *counts, BtlServerFlow *flow, BattleMon *mon, BattleMon *target, s32 effect);
u32 func_ov093_021c79e0(BtlServerFlow *flow, u16 move, BattleMon *mon);
void func_ov093_021c7ad0(BtlServerFlow *flow, BattleMon *mon, s32 param3);
void func_ov093_021c7b0c(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c78a8(BtlServerFlow *flow, BattleMon *mon, s32 param3, s32 param4, s32 param5);
BOOL func_ov093_021c77b0(BtlServerFlow *flow, BattleMon *mon, u32 param3, u8 param4, s32 param5, u32 param6);
void func_ov093_021c785c(BtlServerFlow *flow, u8 param2, BattleMon *mon, u32 param4, s32 param5);
void func_ov093_021c7820(BtlServerFlow *flow, BattleMon *mon, u32 param3);
void func_ov093_021c7c18(void *param_1);
BOOL func_ov093_021c71e0(BtlServerFlow *flow, BattleMon *mon, u16 move, BtlFlowHitWork *hitWork);
u16 func_ov093_021c728c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender, BtlFlowMoveParam *param);
u16 func_ov093_021c7374(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender, const BtlFlowMoveParam *param, BOOL critical);
u16 func_ov093_021c7474(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender, const BtlFlowMoveParam *param, BOOL critical);
u32 func_ov093_021c75ec(BtlServerFlow *flow, BattleMon *mon, u32 type);
void func_ov093_021c7b3c(BtlServerFlow *flow, u8 monId, u16 oldAbility, u16 newAbility);
void func_ov093_021c7b7c(BtlServerFlow *flow, u8 monId);

BtlServerFlow *func_ov093_021bb968(BtlServer *server, BtlMainModule *mainModule, BtlPokeCon *pokeCon,
                                   BtlServerCmdQueue *queue, u32 a4, HeapID heapId) {
    BtlServerFlow *flow;

    flow = GFL_HeapAllocate(heapId, sizeof(BtlServerFlow), TRUE, "btl_server_flow.c", 581);
    flow->server = server;
    flow->pokeCon = pokeCon;
    flow->mainModule = mainModule;
    flow->actionOrderCount = 0;
    flow->unk10 = 0;
    flow->unk14 = 0;
    flow->queue = queue;
    flow->heapId = heapId;
    flow->unk18 = a4;
    flow->unk4A4 = loadEvolutionFile(heapId);
    func_ov093_021bb9d8(flow);
    return flow;
}

void func_ov093_021bb9d0(BtlServerFlow *flow) {
    func_ov093_021bb9d8(flow);
}

void func_ov093_021bb9d8(BtlServerFlow *flow) {
    func_ov093_021d6eac();
    func_ov093_021ef874(0);
    data_ov095_0689cff1(&flow->wazaRec);
    data_ov095_0689d119(&flow->faintRec);
    data_ov095_0689d1fd(&flow->posPoke, flow->mainModule, flow->pokeCon, BtlSetup_GetBattleStyle(flow->mainModule));
    MI_CpuFill8(flow->unk7A9, 0, sizeof(flow->unk7A9));
    MI_CpuFill8(flow->unk7C1, 0, sizeof(flow->unk7C1));
    MI_CpuFill8(flow->unk7D9, 0, sizeof(flow->unk7D9));
    MI_CpuFill8(flow->unk4D4, 0, sizeof(flow->unk4D4));
    func_ov093_021c7c18(&flow->unk1F7C);
    func_ov093_021c8588(flow);
    func_ov093_021d8230(&flow->clientIdList);
    func_ov093_021cc8c8(&flow->actionState);
    func_ov093_021c5474(&flow->unk1B50);
    data_ov095_06898bfd();
    flow->unk786 = 0;
    flow->unk787 = 0;
    flow->unk78A_0 = 0;
    flow->unk78A_1 = 0;
    flow->unk78A_2 = 0;
    flow->unk77C = 0;
    flow->unk789 = 0x1f;
    flow->unk1F74 = 0;
    flow->unk784 = 6;
    flow->unk774 = 0;
    flow->unk77E = 0;
    flow->unk778 = 0;
    flow->unk78A_6 = 0;
}














void func_ov093_021bbb48(BtlServerFlow *flow) {
    GFL_ArcToolFree(flow->unk4A4);
    GFL_HeapFree(flow);
}

BOOL func_ov093_021bbb60(BtlServerFlow *flow) {
    BtlServerCmdQueue *queue;
    u8 weather;
    u32 clientId;
    int result;
    BtlServerClient *client;
    u32 i;
    BattleMon *mon;

    queue = flow->queue;
    queue->writePtr = 0;
    result = FALSE;
    queue->readPtr = 0;
    weather = GetFieldEffectData(flow->mainModule)->env.weather;
    if (weather != 0 && ServerControl_ChangeWeather(flow, weather, 0xff)) {
        result = TRUE;
    }
    for (clientId = 0; clientId < 4; clientId++) {
        client = func_ov093_021bb878(flow->server, clientId);
        if (client != NULL) {
            for (i = 0; i < client->numCoverPos; i++) {
                mon = func_ov093_021b9bf0(client->party, i);
                if (mon != NULL && !IsFainted(mon)) {
                    ServerControl_SwitchInCore(flow, clientId, i, i);
                }
            }
            if (BtlSetup_GetBattleStyle(flow->mainModule) == 3) {
                for (i = 0; i < 3; i++) {
                    mon = func_ov093_021b9bf0(client->party, i);
                    if (mon != NULL && !IsFainted(mon)) {
                        func_ov093_021b8750(flow->mainModule, clientId, mon);
                    }
                }
            }
        }
    }
    if (ServerControl_AfterSwitchIn(flow)) {
        result = TRUE;
    }
    return result;
}

void func_ov093_021bbc34(BtlServerFlow *flow) {
    flow->unk77C = 0;
    flow->unk783 = 0;
}

u32 func_ov093_021bbc44(BtlServerFlow *flow, BtlClientActions *clientActions) {
    u32 i;

    flow->unk14 = 0;
    BtlServerCmdQueue_Init(flow->queue);
    if (flow->unk77C == 0) {
        func_ov093_021c84e8(flow);
        data_ov095_0689d135(&flow->faintRec);
        for (i = 0; i < 4; i++) {
            flow->unk1FE8[i] = 0;
        }
        func_ov093_021d6ee8();
        func_ov093_021bb920(flow->server);
        func_ov093_021d74e4();
        func_ov093_021bbcd4(flow);
        flow->actionOrderCount = func_ov093_021bc610(flow, clientActions, flow->actionOrder, 6);
        flow->unk77C = 1;
    }
    flow->unk783 = func_ov093_021bbfa8(flow, flow->unk783);
    return flow->unk14;
}




void func_ov093_021bbcd4(BtlServerFlow *flow) {
    BtlFlowMonIter iter;
    BattleMon *mon;

    func_ov093_021bd2b4(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &mon)) {
        if (GetAdditionalConditionFlag(mon, 0xc)) {
            scPut_ResetContFlag(flow, mon, 0xc);
        }
    }
}

void func_ov093_021bbd20(BtlServerFlow *flow) {
    flow->unk77C = 0;
}

u32 func_ov093_021bbd2c(BtlServerFlow *flow, BtlClientActions *clientActions) {
    flow->unk14 = 0;
    BtlServerCmdQueue_Init(flow->queue);
    if (flow->unk77C == 0) {
        func_ov093_021bb920(flow->server);
        func_ov093_021d74e4();
        flow->unk77D = 0;
        flow->unk77C = 1;
        if (func_ov093_021bc220(flow, clientActions)) {
            flow->unk14 = 3;
            return flow->unk14;
        }
    }
    flow->unk783 = func_ov093_021bbfa8(flow, flow->unk783);
    return flow->unk14;
}

void func_ov093_021bbd80(BtlServerFlow *flow) {
    flow->unk77C = 0;
}

u32 func_ov093_021bbd8c(BtlServerFlow *flow, BtlClientActions *clientActions) {
    u32 i;
    u32 sideEffects;

    flow->unk14 = 0;
    BtlServerCmdQueue_Init(flow->queue);
    if (flow->unk77C == 0) {
        func_ov093_021bb920(flow->server);
        func_ov093_021d74e4();
        sideEffects = data_ov095_0689d175(&flow->faintRec, 0);
        flow->actionOrderCount = func_ov093_021bc610(flow, clientActions, flow->actionOrder, 6);
        flow->unk783 = 0;
        for (i = 0; i < flow->actionOrderCount; i++) {
            if (flow->actionOrder[i].action.change.action == 3 && !flow->actionOrder[i].action.change.unk10 &&
                !IsFainted(flow->actionOrder[i].mon)) {
                func_ov093_021bdc98(flow, flow->actionOrder[i].mon, flow->actionOrder[i].action.change.slot);
                flow->actionOrder[i].done = TRUE;
            }
        }
        for (i = 0; i < flow->actionOrderCount; i++) {
            if (flow->actionOrder[i].action.change.action == 3 && !flow->actionOrder[i].action.change.unk10 &&
                IsFainted(flow->actionOrder[i].mon)) {
                ServerControl_SwitchInFillSlot(flow, flow->actionOrder[i].clientId,
                                               flow->actionOrder[i].action.change.unk4,
                                               flow->actionOrder[i].action.change.slot, TRUE);
                flow->actionOrder[i].done = TRUE;
            }
        }
        if (BtlSetup_GetBattleStyle(flow->mainModule) == 3) {
            for (i = 0; i < flow->actionOrderCount; i++) {
                if (flow->actionOrder[i].action.change.action == 6 && !flow->actionOrder[i].action.change.unk10) {
                    func_ov093_021bcce0(flow, &flow->actionOrder[i]);
                }
            }
        }
        flow->unk77C = 1;
    }
    ServerControl_AfterSwitchIn(flow);
    func_ov093_021c5200(flow);
    func_ov093_021bc4dc(flow, &flow->unk4CE);
    if (sideEffects == data_ov095_0689d175(&flow->faintRec, 0)) {
        func_ov093_021c4614(flow);
        return 0;
    }
    if (!ServerControl_CheckMatchup(flow)) {
        func_ov093_021c6158(flow, &flow->unk4CE);
        return 2;
    }
    return 4;
}





u32 func_ov093_021bbfa8(BtlServerFlow *flow, u32 i) {
    u32 prevAction;
    u32 action;
    u8 switched;
    u32 sideEffects;

    prevAction = 0;
    for (; i < flow->actionOrderCount; i++) {
        action = BattleAction_GetAction(&flow->actionOrder[i].action);
        if (prevAction == 6 && action != 6) {
            func_ov093_021bdc2c(flow);
            func_ov093_021bc0e8(flow, &flow->actionOrder[i], flow->actionOrderCount - i);
        }
        prevAction = func_ov093_021bcce0(flow, &flow->actionOrder[i]);
        switched = func_ov093_021c5200(flow);
        if ((u8)ServerControl_CheckMatchup(flow)) {
            flow->unk14 = 4;
            return i + 1;
        }
        if (flow->unk14 == 6) {
            return i + 1;
        }
        if (flow->unk14 == 1) {
            return i + 1;
        }
        if (switched) {
            flow->unk14 = 3;
            return i + 1;
        }
    }
    if (flow->unk14 == 0) {
        switched = func_ov093_021c446c(flow);
        if ((u8)ServerControl_CheckMatchup(flow)) {
            flow->unk14 = 4;
            return flow->actionOrderCount;
        }
        if (switched) {
            flow->unk14 = 3;
            return flow->actionOrderCount;
        }
        sideEffects = data_ov095_0689d175(&flow->faintRec, 0);
        if (func_ov093_021c8534(flow) || sideEffects) {
            func_ov093_021bc4dc(flow, &flow->unk4CE);
            func_ov093_021c6158(flow, &flow->unk4CE);
            flow->unk14 = 2;
            return flow->actionOrderCount;
        }
        flow->unk14 = 0;
    }
    return flow->actionOrderCount;
}





static inline u32 ActionOrder_MakeKey(u16 speed, u8 unk13, u8 unk16, u8 unk22) {
    return ((unk22 & 7) << 22) | ((unk16 & 0x3f) << 16) | ((unk13 & 7) << 13) | (speed & 0x1fff);
}


static inline void ActionOrder_SetSpeed(ActionOrderEntry *entry, u16 speed) {
    entry->key = (entry->key & 0xffffe000) | (speed & 0x1fff);
}

static inline void ActionOrder_SetPriority(ActionOrderEntry *entry, s8 priority) {
    entry->key = ActionOrder_MakeKey(entry->key & 0x1fff, (entry->key >> 13) & 7, priority, (entry->key >> 22) & 7);
}

static inline void ActionOrder_SetUnk13(ActionOrderEntry *entry, u8 value) {
    entry->key = (entry->key & 0xffff1fff) | ((value & 7) << 13);
}

// The move of an entry's fight action, 0 for any other action
static inline u16 ActionOrder_GetMove(const ActionOrderEntry *entry) {
    return BattleAction_GetMove(&entry->action);
}

void func_ov093_021bc0e8(BtlServerFlow *flow, ActionOrderEntry *order, u32 count) {
    u32 i;
    u32 action;
    u16 move;
    u32 state;
    u32 unk16;
    u16 speed;

    for (i = 0; i < count; i++) {
        speed = ServerEvent_CalculateSpeed(flow, order[i].mon, TRUE);
        order[i].key = (order[i].key & 0xffffe000) | (speed & 0x1fff);
        action = BattleAction_GetAction(&order[i].action);
        if (action == 1) {
            move = ActionOrder_GetMove(&order[i]);
            state = PushState(&flow->actionState, 0x434);
            unk16 = func_ov093_021bc8ec(flow, move, order[i].mon);
            order[i].key =
                ActionOrder_MakeKey(order[i].key & 0x1fff, (order[i].key >> 13) & 7, unk16, (order[i].key >> 22) & 7);
            PopState(&flow->actionState, state, 0x437);
        }
        if (action == 1 || action == 5) {
            state = PushState(&flow->actionState, 0x43f);
            order[i].key = (order[i].key & 0xffff1fff) | ((func_ov093_021c6350(flow, order[i].mon) & 7) << 13);
            PopState(&flow->actionState, state, 0x442);
        }
    }
    func_ov093_021bc874(order, count);
}

BOOL func_ov093_021bc220(BtlServerFlow *flow, BtlClientActions *clientActions) {
    u32 clientId;
    u32 count;
    u32 i;
    BattleAction action;

    for (clientId = 0; clientId < 4; clientId++) {
        if (func_ov093_021bb85c(flow->server, clientId)) {
            count = func_ov093_021bb8a8(clientActions, clientId);
            for (i = 0; i < count; i++) {
                action = func_ov093_021bb8b0(clientActions, clientId, i);
                if (action.change.action == 3 && !action.change.unk10) {
                    ServerControl_SwitchInFillSlot(flow, clientId, action.change.unk4, action.change.slot, TRUE);
                }
            }
        }
    }
    ServerControl_AfterSwitchIn(flow);
    return func_ov093_021c5200(flow);
}

BOOL ServerControl_CheckMatchup(BtlServerFlow *flow) {
    u8 hasMons[2];
    u32 i;
    BattleParty *party;
    u8 side;

    hasMons[0] = hasMons[1] = 0;
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(flow->mainModule, i)) {
            party = GetPartyData(flow->pokeCon, i);
            side = GetClientSide(flow->mainModule, i);
            if (GetAlivePartyCount(party) != 0) {
                hasMons[side] = 1;
            }
        }
    }
    if (!hasMons[0] || !hasMons[1]) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov093_021bc310(BtlServerFlow *flow) {
    u32 i;

    for (i = 0; i < 4; i++) {
        if (DoesClientExist(flow->mainModule, i) &&
            GetClientSide(flow->mainModule, i) ==
                GetClientSide(flow->mainModule, GetPlayerClientID(flow->mainModule)) &&
            GetAlivePartyCount(GetPartyData(flow->pokeCon, i)) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 func_ov093_021bc360(BtlServerFlow *flow) {
    BattleMon *mon;
    u32 result;

    mon = func_ov093_021b990c(flow->pokeCon, GetPlayerClientID(flow->mainModule), 0);
    result = 0;
    BtlServerCmdQueue_Init(flow->queue);
    if (ServerControl_Escape(flow, mon)) {
        result = 1;
    }
    return result;
}

BOOL func_ov093_021bc390(BtlServerFlow *flow) {
    BOOL result;

    result = FALSE;
    BtlServerCmdQueue_Init(flow->queue);
    func_ov093_021bc3b0(flow);
    if (flow->queue->writePtr != 0) {
        result = TRUE;
    }
    return result;
}

void func_ov093_021bc3b0(BtlServerFlow *flow) {
    u32 i;
    u32 count;
    u8 clientIds[4];

    if (flow->unk18 == 1) {
        for (i = 0; i < 4; i++) {
            if (func_ov093_021bb890(flow->server, i)) {
                if (BtlSetup_GetBattleStyle(flow->mainModule) != 2) {
                    count = 1;
                } else {
                    count = data_ov095_0689d559(&flow->posPoke, i, clientIds) + 1;
                }
                func_ov093_021cd4b0(flow->queue, 0x29, (u8)i, (u8)count);
            }
        }
    }
}




void func_ov093_021bc418(BtlServerFlow *flow, u8 clientId, u8 slot) {
    BattleParty *party;
    BattleMon *outMon;
    BattleMon *inMon;

    if (slot != 1 && slot != 0) {
        party = GetPartyData(flow->pokeCon, clientId);
        func_ov093_021cd4b0(flow->queue, 0x48, clientId, slot);
        func_ov093_021b9c50(party, slot, &outMon, &inMon);
        if (!IsFainted(outMon)) {
            ServerControl_ClearMonDependentEffects(flow, outMon, TRUE);
        }
        if (!IsFainted(inMon)) {
            if (func_ov093_021ef948(2)) {
                if (CheckCondition(inMon, 0x20)) {
                    ServerControl_CureCondition(flow, inMon, 0x20, 0);
                }
                if (CheckCondition(inMon, 0x1e)) {
                    ServerControl_CureCondition(flow, inMon, 0x1e, 0);
                }
            }
            AbilityEvent_ItemRotationWake(inMon);
            ItemEvent_ItemRotationWake(inMon);
        }
        data_ov095_0689d339(&flow->posPoke, slot, clientId, inMon, flow->pokeCon);
    }
}




BOOL func_ov093_021bc4dc(BtlServerFlow *flow, BtlFlowClientList *list) {
    BOOL result;
    u8 clientId;
    u8 count;
    u8 i;
    u8 positions[4];

    result = FALSE;
    list->count = 0;
    for (clientId = 0; clientId < 4; clientId++) {
        count = data_ov095_0689d559(&flow->posPoke, clientId, positions);
        if (count != 0) {
            for (i = 0; i < count; i++) {
                RequestChangePokemon(flow->server, positions[i]);
            }
            result = TRUE;
            list->clientIds[list->count++] = clientId;
        }
    }
    return result;
}




BtlClientIDList *func_ov093_021bc550(BtlServerFlow *flow) {
    return &flow->clientIdList;
}

u8 func_ov093_021bc55c(BtlServerFlow *flow) {
    return flow->unk784;
}

void func_ov093_021bc568(BtlServerFlow *flow) {
    u32 state;
    u32 i;
    u16 move;
    BattleMon *mon;
    ActionOrderEntry *entry;

    state = PushState(&flow->actionState, 0x591);
    for (i = 0; i < flow->actionOrderCount; i++) {
        entry = &flow->actionOrder[i];
        move = BattleAction_GetMove(&entry->action);
        if (move != 0) {
            mon = flow->actionOrder[i].mon;
            if (MoveEvent_AddItem(mon, move, GetBattleMonStat(mon, 0xc))) {
                func_ov093_021c6394(flow, mon, move);
                func_ov093_021e0394(mon, move);
            }
        }
    }
    PopState(&flow->actionState, state, 0x5a0);
}

u8 func_ov093_021bc610(BtlServerFlow *flow, BtlClientActions *clientActions, const ActionOrderEntry *order, u8 max) {
    u16 speed;
    BattleMon *mon;
    u32 state;
    u8 clientId;
    u8 j;
    u8 numActions;
    BOOL rotated;
    u8 i;
    u8 count;
    BtlServerClient *client;
    u8 slot;
    ActionOrderEntry *entry;
    u8 kind;
    u8 priority;

    rotated = FALSE;
    for (clientId = 0, count = 0; clientId < 4; clientId++) {
        client = func_ov093_021bb85c(flow->server, clientId);
        if (client == NULL) {
            continue;
        }
        numActions = func_ov093_021bb8a8(clientActions, clientId);
        slot = 0;
        for (j = 0; j < numActions; j++) {
            entry = &order[count];
            entry->action = func_ov093_021bb8b0(clientActions, clientId, j);
            if (func_ov093_021d83e0(&entry->action)) {
                break;
            }
            switch ((u8)BattleAction_GetAction(&entry->action)) {
            case 6:
                slot = func_ov093_021b9ab0((u8)entry->action.change.unk4);
                entry->mon = func_ov093_021b9bf0(client->party, slot);
                rotated = TRUE;
                break;
            case 3:
                entry->mon = func_ov093_021b9bf0(client->party, entry->action.change.unk4);
                slot++;
                break;
            default:
                entry->mon = func_ov093_021b9bf0(client->party, slot);
                slot++;
                break;
            }
            entry->clientId = clientId;
            entry->done = FALSE;
            entry->interrupting = FALSE;
            count++;
        }
    }
    for (i = 0; i < count; i++) {
        state = PushState(&flow->actionState, 0x640);
        entry = &order[i];
        mon = order[i].mon;
        switch (entry->action.bits.action) {
        case 4:
            if (BtlSetup_GetBattleType(flow->mainModule) == 0 && entry->clientId == 1) {
                kind = 0;
            } else {
                kind = 4;
            }
            break;
        case 3:
            kind = 3;
            break;
        case 2:
            kind = 2;
            break;
        case 6:
            kind = 1;
            break;
        case 7:
            kind = 0;
            break;
        case 5:
            kind = 0;
            break;
        case 1:
            kind = 0;
            break;
        case 0:
            entry->key = ActionOrder_MakeKey(0, 1, 0, 4);
            continue;
        default:
            kind = 0;
            break;
        case 8:
            continue;
        }
        if (entry->action.bits.action == 1) {
            priority = func_ov093_021bc8ec(flow, BattleAction_GetMove(&order[i].action), mon);
        } else if (entry->action.bits.action == 5 || entry->action.bits.action == 7) {
            priority = 7;
        } else {
            priority = 0;
        }
        speed = ServerEvent_CalculateSpeed(flow, mon, TRUE);
        PopState(&flow->actionState, state, 0x678);
        entry->key = ActionOrder_MakeKey(speed, 1, priority, kind);
    }
    func_ov093_021bc874(order, count);
    if (!rotated) {
        for (i = 0; i < count; i++) {
            entry = &order[i];
            if (entry->action.bits.action == 1 || entry->action.bits.action == 5) {
                state = PushState(&flow->actionState, 0x68a);
                entry->key = (entry->key & 0xffff1fff) | ((func_ov093_021c6350(flow, entry->mon) & 7) << 13);
                PopState(&flow->actionState, state, 0x68d);
            }
        }
        func_ov093_021bc874(order, count);
    }
    return count;
}

void func_ov093_021bc874(ActionOrderEntry *order, u32 count) {
    u32 i;
    u32 j;
    ActionOrderEntry tmp;

    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (order[i].key <= order[j].key && (order[i].key != order[j].key || BattleRandom(2) != 0)) {
                tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }
}

u8 func_ov093_021bc8ec(BtlServerFlow *flow, u16 move, BattleMon *mon) {
    u8 priority;

    priority = PML_MoveGetParam(move, 6) + 7;
    BattleEventVar_Push(0x6c7);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetValue(0x18, priority);
    BattleEvent_CallHandlers(flow, 0x11);
    priority = BattleEventVar_GetValue(0x18);
    BattleEventVar_Pop(0x6cd);
    return priority;
}

u16 ServerEvent_CalculateSpeed(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    u32 speed;

    speed = GetBattleMonStat(mon, 0xc);
    BattleEventVar_Push(0x6e0);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x2f, speed);
    BattleEventVar_SetValue(0x51, 1);
    BattleEventVar_SetValue(0x4a, 0);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEvent_CallHandlers(flow, 0x13);
    speed = fixed_round(BattleEventVar_GetValue(0x2f), BattleEventVar_GetValue(0x35));
    if (GetBattleMonStatus(mon) == 1 && BattleEventVar_GetValue(0x51)) {
        speed = speed * 25 / 100;
    }
    if (speed > 10000) {
        speed = 10000;
    }
    if (flag && BattleEventVar_GetValue(0x4a)) {
        speed = 10000 - speed;
    }
    BattleEventVar_Pop(0x700);
    return speed;
}

// Function name from swan.
ActionOrderEntry *ActionOrder_SearchByMonID(BtlServerFlow *flow, u8 monId) {
    u32 i;

    for (i = 0; i < flow->actionOrderCount; i++) {
        if (GetMonID(flow->actionOrder[i].mon) == monId) {
            return &flow->actionOrder[i];
        }
    }
    return NULL;
}

ActionOrderEntry *func_ov093_021bca50(BtlServerFlow *flow, ActionOrderEntry *after, u8 monId) {
    u32 i;

    for (i = 0; i < flow->actionOrderCount; i++) {
        if (&flow->actionOrder[i] == after) {
            i++;
            break;
        }
    }
    for (; i < flow->actionOrderCount; i++) {
        if (monId == GetMonID(flow->actionOrder[i].mon)) {
            return &flow->actionOrder[i];
        }
    }
    return NULL;
}

ActionOrderEntry *ActionOrder_SearchByMoveID(BtlServerFlow *flow, u16 moveId, u8 start) {
    u32 i;

    for (i = start; i < flow->actionOrderCount; i++) {
        if (!flow->actionOrder[i].done && BattleAction_GetAction(&flow->actionOrder[i].action) == 1 &&
            moveId == func_ov093_021d8340(&flow->actionOrder[i].action)) {
            return &flow->actionOrder[i];
        }
    }
    return NULL;
}

ActionOrderEntry *func_ov093_021bcb14(BtlServerFlow *flow, u16 move, u8 monId, u8 target) {
    ActionOrderEntry *entry;

    entry = ActionOrder_SearchByMoveID(flow, move, 0);
    while (entry != NULL) {
        if (IsAllyMonID(monId, GetMonID(entry->mon)) && !IsFainted(entry->mon)) {
            return entry;
        }
        entry = ActionOrder_SearchByMoveID(flow, move, func_ov093_021bcb68(flow, entry) + 1);
    }
    return NULL;
}

u8 func_ov093_021bcb68(BtlServerFlow *flow, ActionOrderEntry *entry) {
    s32 i;

    for (i = 0; i < flow->actionOrderCount; i++) {
        if (&flow->actionOrder[i] == entry) {
            return i;
        }
    }
    return flow->actionOrderCount;
}

s32 ActionOrderTool_Interrupt(BtlServerFlow *flow, ActionOrderEntry *entry, s32 start) {
    s32 from;
    s32 to;
    s32 i;
    u8 count;

    from = -1;
    to = -1;
    count = flow->actionOrderCount;
    for (i = start; i < count; i++) {
        if (&flow->actionOrder[i] == entry) {
            from = i;
            break;
        }
    }
    for (; start < count; start++) {
        if (!flow->actionOrder[start].done) {
            to = start;
            break;
        }
    }
    if (from >= 0 && to >= 0 && from > to) {
        flow->tempEntry = *entry;
        for (; from > to; from--) {
            flow->actionOrder[from] = flow->actionOrder[from - 1];
        }
        flow->actionOrder[to] = flow->tempEntry;
        return to;
    }
    return -1;
}

void ActionOrderTool_SendToLast(BtlServerFlow *flow, ActionOrderEntry *entry) {
    s32 from;
    s32 i;

    from = -1;
    for (i = 0; i < flow->actionOrderCount; i++) {
        if (&flow->actionOrder[i] == entry) {
            from = i;
            break;
        }
    }
    if (from >= 0) {
        flow->tempEntry = *entry;
        for (; from < flow->actionOrderCount - 1; from++) {
            flow->actionOrder[from] = flow->actionOrder[from + 1];
        }
        flow->actionOrder[from] = flow->tempEntry;
    }
}

u32 func_ov093_021bcce0(BtlServerFlow *flow, ActionOrderEntry *entry) {
    BattleMon *mon;
    BattleAction action;
    u32 state;

    if (!entry->done) {
        mon = entry->mon;
        action = entry->action;
        func_ov093_021c859c(flow);
        entry->done = TRUE;
        if ((flow->unk14 != 5 || action.bits.action == 4) && !IsFainted(mon) &&
            (action.bits.action == 6 || DoesBattleMonExist(&flow->posPoke, GetMonID(mon)))) {
            if (CheckCondition(mon, 0x21) && (action.bits.action != 4 || GetRunMode(flow->mainModule) != 2)) {
                if (GetConditionCount(mon, 3)) {
                    ServerControl_SetMonCounter(flow, mon, 3, 0);
                }
            } else {
                func_ov093_021bcefc(flow, mon, entry->action.bits.action);
                func_ov093_021d5fc0(mon, 0);
                switch (action.bits.action) {
                case 6:
                    if (BtlSetup_GetBattleStyle(flow->mainModule) == 3) {
                        func_ov093_021bc418(flow, entry->clientId, action.change.unk4);
                    }
                    break;
                case 1:
                    if (!flow->unk1FE8[0]) {
                        func_ov093_021bc568(flow);
                        flow->unk1FE8[0] = TRUE;
                    }
                    flow->unk1F78 = func_ov093_021bcfac(flow, mon);
                    func_ov093_021bde98(flow, mon, &action, entry->key & 0x3fffff);
                    break;
                case 2:
                    if (func_ov093_021cb730(flow, mon, action.item.item, action.item.param, action.item.target) == 1 &&
                        func_ov093_021bd708(flow, mon, TRUE, TRUE)) {
                        flow->unk14 = 5;
                    }
                    break;
                case 3:
                    func_ov093_021bdc98(flow, mon, action.change.slot);
                    ServerControl_AfterSwitchIn(flow);
                    break;
                case 4:
                    if (ServerControl_Escape(flow, mon)) {
                        flow->unk14 = 5;
                    }
                    break;
                case 8:
                    flow->unk14 = 4;
                    break;
                case 5:
                    func_ov093_021bd3e8(flow, mon);
                    break;
                case 7:
                    func_ov093_021c54bc(flow, mon);
                    break;
                }
                if (action.bits.action == 1 || action.bits.action == 2) {
                    func_ov093_021d5fc0(mon, 1);
                    scPut_SetContFlag(flow, mon, 0);
                }
                state = PushState(&flow->actionState, 0x83b);
                func_ov093_021bcf70(flow, mon, action.bits.action);
                PopState(&flow->actionState, state, 0x83d);
            }
        }
        func_ov093_021c85b8(flow);
        return action.bits.action;
    }
    return 0;
}




void func_ov093_021bcefc(BtlServerFlow *flow, BattleMon *mon, u32 action) {
    u32 state;

    state = PushState(&flow->actionState, 0x854);
    func_ov093_021bcf34(flow, mon, action);
    PopState(&flow->actionState, state, 0x857);
}

void func_ov093_021bcf34(BtlServerFlow *flow, BattleMon *mon, u32 action) {
    u8 monId;

    BattleEventVar_Push(0x864);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0xc, action);
    BattleEvent_CallHandlers(flow, 1);
    BattleEventVar_Pop(0x868);
}

void func_ov093_021bcf70(BtlServerFlow *flow, BattleMon *mon, u32 action) {
    u8 monId;

    BattleEventVar_Push(0x874);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0xc, action);
    BattleEvent_CallHandlers(flow, 2);
    BattleEventVar_Pop(0x878);
}

u32 func_ov093_021bcfac(BtlServerFlow *flow, BattleMon *mon) {
    PartyPkm *src;
    PlayerInfo *player;
    u32 badges;
    u16 level;
    u16 maxLevel;
    u16 rand;

    if (BtlSetup_GetBattleType(flow->mainModule) <= 1 &&
        func_ov093_021b8ddc(GetMonID(mon)) == GetPlayerClientID(flow->mainModule)) {
        src = GetSrcData(mon);
        player = func_ov093_021b871c(flow->mainModule);
        if (!IsTrainerOT(src, player)) {
            badges = func_ov093_021b85a0(flow->mainModule);
            if (badges < 8) {
                level = GetBattleMonStat(mon, 0xf);
                maxLevel = (badges + 1) * 10;
                if (level <= maxLevel) {
                    return 0;
                }
                if ((u16)BattleRandom(level + maxLevel + 1) < maxLevel) {
                    return 0;
                }
                if (CheckCondition(mon, 2)) {
                    return 4;
                }
                rand = BattleRandom(0x100);
                if (rand < level - maxLevel && !func_ov093_021bd080(flow, mon)) {
                    return 2;
                }
                rand -= level - maxLevel;
                if (rand < level - maxLevel) {
                    return 3;
                }
                return 4;
            }
        }
    }
    return 0;
}


BOOL func_ov093_021bd080(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x8d0);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEvent_CallHandlers(flow, 0xe);
    value = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x8d5);
    return value;
}

BOOL ActionOrder_InterruptProc(BtlServerFlow *flow, u8 monId, u8 targetId) {
    ActionOrderEntry *entry;

    entry = ActionOrder_SearchByMonID(flow, monId);
    if (entry != NULL) {
        if (entry->done && BattleAction_GetAction(&entry->action) == 6) {
            entry = func_ov093_021bca50(flow, entry, monId);
        }
        if (!entry->done) {
            if (func_ov093_021bde48(entry->mon, &entry->action)) {
                return FALSE;
            }
            if (BattleAction_GetAction(&entry->action) == 6) {
                return FALSE;
            }
            BattleAction_ChangeFightTargetPos(&entry->action, GetBattlePos(&flow->posPoke, targetId));
            func_ov093_021bcce0(flow, entry);
            return TRUE;
        }
    }
    return FALSE;
}



// Function names from swan.
BOOL ActionOrder_InterruptReserve(BtlServerFlow *flow, u8 monId) {
    ActionOrderEntry *entry;

    entry = ActionOrder_SearchByMonID(flow, monId);
    if (entry && !entry->done && ActionOrderTool_Interrupt(flow, entry, 0) >= 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL ActionOrder_InterruptReserveByMove(BtlServerFlow *flow, u16 moveId) {
    u32 start;
    BOOL didInterrupt;
    ActionOrderEntry *entry;
    s32 index;

    start = 0;
    entry = ActionOrder_SearchByMoveID(flow, moveId, 0);
    didInterrupt = FALSE;
    while (entry) {
        index = ActionOrderTool_Interrupt(flow, entry, start);
        if (index < 0) {
            break;
        }
        start = index + 1;
        entry = ActionOrder_SearchByMoveID(flow, moveId, (u8)start);
        didInterrupt = TRUE;
    }
    return didInterrupt;
}

BOOL ActionOrder_SendToLast(BtlServerFlow *flow, u8 monId) {
    ActionOrderEntry *entry;

    entry = ActionOrder_SearchByMonID(flow, monId);
    if (entry && !entry->done) {
        ActionOrderTool_SendToLast(flow, entry);
        return TRUE;
    }
    return FALSE;
}

void ActionOrder_ForceDone(BtlServerFlow *flow, u8 monId) {
    ActionOrderEntry *entry;

    entry = ActionOrder_SearchByMonID(flow, monId);
    if (entry) {
        entry->done = 1;
    }
}


void func_ov093_021bd1e0(BattleMoveEffectState *targets) {
    targets->unk00 = 0;
    targets->pos1 = 6;
    targets->pos2 = 6;
    targets->index = 0;
    targets->enabled = FALSE;
    targets->unk05_1 = 0;
}

void func_ov093_021bd200(BattleMoveEffectState *targets, BtlServerFlow *flow, BattleMon *mon, void *monSet) {
    u32 count;

    count = data_ov095_0689cd41(monSet);
    targets->pos1 = GetBattlePos(&flow->posPoke, GetMonID(mon));
    targets->pos2 = 6;
    targets->enabled = FALSE;
    targets->unk05_1 = 0;
    if (count == 1 && data_ov095_0689cd39(monSet)) {
        targets->pos2 = GetBattlePos(&flow->posPoke, GetMonID(data_ov095_0689cc71(monSet, 0)));
    }
}
















void func_ov093_021bd2b4(BtlFlowMonIter *iter, BtlServerFlow *flow) {
    u8 clientId;
    BtlServerClient *client;
    u8 i;

    clientId = 0;
    iter->clientId = 0;
    iter->index = clientId;
    iter->done = TRUE;
    iter->rotation = clientId;
    for (; clientId < 4; clientId++) {
        client = func_ov093_021bb878(flow->server, clientId);
        if (client != NULL) {
            for (i = 0; i < client->numCoverPos; i++) {
                if (BtlFlow_IsMonAlive(GetBattleMonFromParty(client->party, i))) {
                    iter->clientId = clientId;
                    iter->done = FALSE;
                    iter->index = i;
                    return;
                }
            }
        }
    }
}

void func_ov093_021bd338(BtlFlowMonIter *iter, BtlServerFlow *flow) {
    if (BtlSetup_GetBattleStyle(flow->mainModule) == 3) {
        iter->rotation = TRUE;
    }
}

BOOL func_ov093_021bd34c(BtlFlowMonIter *iter, BtlServerFlow *flow, BattleMon **mon) {
    BtlServerClient *client;
    u8 count;

    if (iter->done) {
        return FALSE;
    }
    *mon = func_ov093_021b9bf0(func_ov093_021bb85c(flow->server, iter->clientId)->party, iter->index);
    iter->index++;
    while (iter->clientId < 4) {
        client = func_ov093_021bb878(flow->server, iter->clientId);
        if (client != NULL) {
            count = !iter->rotation ? client->numCoverPos : 3;
            for (; iter->index < count; iter->index++) {
                if (BtlFlow_IsMonAlive(func_ov093_021b9bf0(client->party, iter->index))) {
                    return TRUE;
                }
            }
        }
        iter->clientId++;
        iter->index = 0;
    }
    iter->done = TRUE;
    return TRUE;
}

void func_ov093_021bd3e8(BtlServerFlow *flow, BattleMon *mon) {
    u8 clientId;
    s32 slot;

    clientId = func_ov093_021b8ddc(GetMonID(mon));
    slot = FindPartyMon(GetPartyData(flow->pokeCon, clientId), mon);
    if (slot == 0 || slot == 2) {
        ServerControl_MoveCore(flow, clientId, slot, 1, 0);
        ServerDisplay_SkyDropTargetAppear(flow, mon, 0xe7);
        ServerControl_AfterMove(flow, clientId, slot, 1);
    }
}

void ServerControl_MoveCore(BtlServerFlow *handler, u8 clientId, u8 firstSlot, u8 secondSlot, u32 flag) {
    BattleParty *party;
    u8 firstPos;
    u8 secondPos;
    BattleMon *first;
    BattleMon *second;

    party = GetPartyData(handler->pokeCon, clientId);
    firstPos = func_ov093_021b8bec(handler->mainModule, clientId, firstSlot);
    secondPos = func_ov093_021b8bec(handler->mainModule, clientId, secondSlot);
    data_ov095_0689d4f1(&handler->posPoke, firstPos, secondPos);
    func_ov093_021b9c10(party, firstSlot, secondSlot);
    if (!flag) {
        func_ov093_021cd4b0(handler->queue, 0x44, clientId, firstPos, secondPos);
        first = func_ov093_021b9bf0(party, firstSlot);
        second = func_ov093_021b9bf0(party, secondSlot);
        if (!IsFainted(first)) {
            func_ov093_021d5fc0(first, 0xc);
        }
        if (!IsFainted(second)) {
            func_ov093_021d5fc0(second, 0xc);
        }
    }
}



void ServerControl_AfterMove(BtlServerFlow *handler, u8 clientId, u8 firstSlot, u8 secondSlot) {
    BattleParty *party;
    BattleMon *first;
    BattleMon *second;

    party = GetPartyData(handler->pokeCon, clientId);
    first = GetBattleMonFromParty(party, firstSlot);
    second = GetBattleMonFromParty(party, secondSlot);
    ServerControl_AfterMoveCore(handler, first);
    ServerControl_AfterMoveCore(handler, second);
}

void ServerControl_AfterMoveCore(BtlServerFlow *flow, BattleMon *mon) {
    u32 state;

    if (!IsFainted(mon)) {
        state = PushState(&flow->actionState, 0xa89);
        func_ov093_021bd554(flow, mon);
        PopState(&flow->actionState, state, 0xa8b);
    }
}


void func_ov093_021bd554(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0xa98);
    BattleEventVar_SetValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0xa4);
    BattleEventVar_Pop(0xa9b);
}

BOOL ServerControl_Escape(BtlServerFlow *flow, BattleMon *mon) {
    u8 clientId;
    u8 playerClientId;
    BOOL result;

    clientId = func_ov093_021b8ddc(GetMonID(mon));
    playerClientId = GetPlayerClientID(flow->mainModule);
    result = ServerControl_EscapeSub(flow, mon, FALSE);
    if (result) {
        return TRUE;
    }
    if (clientId == playerClientId) {
        func_ov093_021ba1b4(flow->mainModule, 0x4e);
        func_ov093_021cd64c(flow->queue, 0x5a, 0x49, 0xffff0000);
    } else {
        func_ov093_021cd64c(flow->queue, 0x5b, 0x36b, GetMonID(mon), 0xffff0000);
    }
    if (clientId == playerClientId) {
        flow->unk786++;
        if (flow->unk786 > 30) {
            flow->unk786 = 30;
        }
    }
    return result;
}

BOOL ServerControl_EscapeSub(BtlServerFlow *handler, BattleMon *mon, u32 flag) {
    BOOL result;
    BattleMon *enemy;
    u16 speed;
    u16 enemySpeed;
    u32 chance;

    result = FALSE;
    if (GetRunMode(handler->mainModule) == 0) {
        if (func_ov093_021b8ddc(GetMonID(mon)) == GetPlayerClientID(handler->mainModule)) {
            result = func_ov093_021bd6b8(handler, mon);
            if (result) {
                flag = TRUE;
            }
        } else {
            flag = TRUE;
        }
    } else {
        flag = TRUE;
        result = TRUE;
    }
    if (!flag) {
        enemy = GetClientMonData(handler->pokeCon, 1, 0);
        if (enemy != NULL) {
            speed = RawBattleMonStat(mon, 0xc);
            enemySpeed = RawBattleMonStat(enemy, 0xc);
            if (speed <= enemySpeed) {
                chance = (speed << 12) / enemySpeed * 128 >> 12;
                chance += handler->unk786 * 30;
                if (BattleRandom(256) >= chance) {
                    return FALSE;
                }
            }
        }
    }
    return func_ov093_021bd708(handler, mon, result, FALSE);
}

BOOL func_ov093_021bd6b8(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;
    BOOL fainted;
    u32 value;

    fainted = IsFainted(mon);
    if (fainted == 0) {
        BattleEventVar_Push(0xb21);
        monId = GetMonID(mon);
        BattleEventVar_SetConstValue(2, monId);
        BattleEventVar_SetRewriteOnceValue(0x51, 0);
        BattleEvent_CallHandlers(flow, 0xb);
        value = BattleEventVar_GetValue(0x51);
        BattleEventVar_Pop(0xb26);
        return value;
    }
    return 0;
}

BOOL func_ov093_021bd708(BtlServerFlow *flow, BattleMon *mon, BOOL arg2, BOOL arg3) {
    u8 clientId;
    u8 isEnemy;
    BOOL result;
    u32 state;
    u32 state2;

    clientId = func_ov093_021b8ddc(GetMonID(mon));
    if (GetRunMode(flow->mainModule) == 0) {
        isEnemy = !IsAllyClientID(clientId, GetPlayerClientID(flow->mainModule)) ? TRUE : FALSE;
        if (!arg2 && !IsFainted(mon)) {
            state = PushState(&flow->actionState, 0xb48);
            result = func_ov093_021bd850(flow, mon);
            PopState(&flow->actionState, state, 0xb4a);
            if (result) {
                return FALSE;
            }
        }
        if (!flow->unk78A_2) {
            if (!arg3) {
                state2 = PushState(&flow->actionState, 0xb58);
                result = func_ov093_021bd8ac(flow, mon);
                PopState(&flow->actionState, state2, 0xb5a);
            } else {
                result = FALSE;
            }
            if (!result) {
                if (isEnemy) {
                    if (BtlSetup_GetBattleType(flow->mainModule) == 0) {
                        func_ov093_021cd64c(flow->queue, 0x5c, 0x4b, 0x56a, GetMonID(mon), 0xffff0000);
                    }
                } else {
                    func_ov093_021cd64c(flow->queue, 0x5c, 0x48, 0x56a, 0xffff0000);
                }
            }
            flow->unk78A_2 = TRUE;
        }
    } else if (func_ov093_021b86c0(flow->mainModule) && !flow->unk78A_2) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0xc6, 0xffff0000);
        flow->unk78A_2 = TRUE;
    }
    func_ov093_021d8244(&flow->clientIdList, clientId);
    return TRUE;
}

BOOL func_ov093_021bd850(BtlServerFlow *flow, BattleMon *mon) {
    BOOL result;

    BattleEventVar_Push(0xb8d);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    data_ov095_0689c68d(flow, mon);
    BattleEvent_CallHandlers(flow, 0xc);
    result = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0xb93);
    return result;
}




BOOL func_ov093_021bd8ac(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0xba3);
    monId = GetMonID(mon);
    BattleEventVar_SetValue(2, monId);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, 0xd);
    value = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0xba8);
    return value;
}

void ServerControl_SwitchInCore(BtlServerFlow *flow, u8 clientId, u8 pos, u8 slot) {
    BattleParty *party;
    BattleMon *mon;
    u8 monId;

    party = GetPartyData(flow->pokeCon, clientId);
    func_ov093_021cd4b0(flow->queue, 0x2a, clientId);
    func_ov093_021b9d10(flow->mainModule, party, clientId);
    if (pos != slot) {
        func_ov093_021b9c10(party, pos, slot);
    }
    mon = func_ov093_021b9bf0(party, pos);
    monId = GetMonID(mon);
    func_ov093_021b8750(flow->mainModule, clientId, mon);
    AbilityEvent_AddItem(mon);
    ItemEvent_AddItem(mon);
    func_ov093_021d63ec(mon, flow->unk10);
    func_ov093_021d6580(mon);
    flow->unk7C1[monId] = TRUE;
    if (BtlSetup_GetBattleStyle(flow->mainModule) == 3 && func_ov093_021b99b8(flow->mainModule, clientId) <= pos) {
        flow->unk7C1[monId] = FALSE;
    }
    func_ov093_021cd4b0(flow->queue, 0x12, clientId, pos, slot, (u16)flow->unk10);
    data_ov095_0689d321(&flow->posPoke, func_ov093_021b8bec(flow->mainModule, clientId, pos), monId, flow->pokeCon);
}



void ServerControl_SwitchInFillSlot(BtlServerFlow *handler, u8 target, u8 slot, u8 slotAgain, BOOL flag) {
    ServerControl_SwitchInCore(handler, target, slot, slotAgain);
    func_ov093_021cd4b0(handler->queue, 0x3d, target, slot, slotAgain, (u8)flag);
}

BOOL ServerControl_AfterSwitchIn(BtlServerFlow *handler) {
    BOOL result;
    BtlFlowMonIter iter;
    BattleMon *mon;
    u8 monId;
    void *monSet;
    u32 state;

    result = FALSE;
    monSet = &handler->unk1A68;
    data_ov095_0689cb3d(monSet);
    func_ov093_021bd2b4(&iter, handler);
    while (func_ov093_021bd34c(&iter, handler, &mon)) {
        monId = GetMonID(mon);
        if (handler->unk7C1[monId]) {
            data_ov095_0689cb49(monSet, mon);
            handler->unk7C1[monId] = FALSE;
        }
    }
    SortBySpeed(monSet, handler);
    state = PushState(&handler->actionState, 0xc18);
    func_ov093_021bdbec(handler);
    if (BattleHandler_Result(handler)) {
        result = TRUE;
    }
    PopState(&handler->actionState, state, 0xc1e);
    data_ov095_0689cc85(monSet);
    for (mon = data_ov095_0689cc8d(monSet); mon != NULL; mon = data_ov095_0689cc8d(monSet)) {
        state = PushState(&handler->actionState, 0xc24);
        func_ov093_021bdb88(handler, mon);
        if (BattleHandler_Result(handler)) {
            result = TRUE;
        }
        PopState(&handler->actionState, state, 0xc2a);
    }
    state = PushState(&handler->actionState, 0xc2d);
    func_ov093_021bdc0c(handler);
    if (BattleHandler_Result(handler)) {
        result = TRUE;
    }
    PopState(&handler->actionState, state, 0xc33);
    return result;
}




















void func_ov093_021bdb88(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0xc43);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x55);
    BattleEventVar_Pop(0xc46);
}

void func_ov093_021bdbb8(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0xc55);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    func_ov093_021d70fc(flow, 0x55, 4);
    BattleEventVar_Pop(0xc58);
}

void func_ov093_021bdbec(BtlServerFlow *flow) {
    BattleEventVar_Push(0xc65);
    BattleEvent_CallHandlers(flow, 0x56);
    BattleEventVar_Pop(0xc67);
}

void func_ov093_021bdc0c(BtlServerFlow *flow) {
    BattleEventVar_Push(0xc73);
    BattleEvent_CallHandlers(flow, 0x57);
    BattleEventVar_Pop(0xc75);
}

void func_ov093_021bdc2c(BtlServerFlow *flow) {
    u32 state;

    state = PushState(&flow->actionState, 0xc82);
    func_ov093_021bdc58(flow);
    PopState(&flow->actionState, state, 0xc84);
}

void func_ov093_021bdc58(BtlServerFlow *flow) {
    BattleEventVar_Push(0xc91);
    BattleEvent_CallHandlers(flow, 0x58);
    BattleEventVar_Pop(0xc93);
}

void func_ov093_021bdc78(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;

    monId = GetMonID(mon);
    func_ov093_021cd4b0(flow->queue, 0x3b, func_ov093_021b8ddc(monId), monId);
}

void func_ov093_021bdc98(BtlServerFlow *flow, BattleMon *mon, u8 slot) {
    u8 clientId;
    u8 index;

    func_ov093_021bdc78(flow, mon);
    if (ServerControl_SwitchOut(flow, mon, FALSE)) {
        func_ov093_021b8e28(flow->mainModule, MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon)),
                            &clientId, &index);
        if (flow->unk7D9[clientId] < 0xff) {
            flow->unk7D9[clientId]++;
        }
        ServerControl_SwitchInFillSlot(flow, clientId, index, slot, TRUE);
    }
}

BOOL ServerControl_SwitchOut(BtlServerFlow *handler, BattleMon *mon, u8 flag) {
    u32 count;
    u32 i;

    if (!flag) {
        count = ServerEvent_InterruptSwitch(handler, mon);
        if (count != 0) {
            handler->unk78A_0 = TRUE;
            for (i = 0; i < count; i++) {
                ActionOrder_InterruptProc(handler, handler->interruptMonIds[i], GetMonID(mon));
                if (IsFainted(mon)) {
                    break;
                }
            }
            handler->unk78A_0 = FALSE;
        }
    }
    if (!IsFainted(mon)) {
        ServerControl_SwitchOutCore(handler, mon, 0x26c);
        return TRUE;
    }
    return FALSE;
}

void ServerControl_SwitchOutCore(BtlServerFlow *flow, BattleMon *mon, u32 effect) {
    u8 monId;
    u8 pos;
    u32 state;

    monId = GetMonID(mon);
    pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, monId);
    if (pos != 6) {
        func_ov093_021cd4b0(flow->queue, 0x3c, pos, effect);
    }
    ActionOrder_ForceDone(flow, monId);
    state = PushState(&flow->actionState, 0xcff);
    ServerControl_SwitchOutConfirm(flow, mon);
    PopState(&flow->actionState, state, 0xd01);
    ServerControl_ClearMonDependentEffects(flow, mon, FALSE);
    Clear_ForSwitch(mon);
    func_ov093_021cd4b0(flow->queue, 0x20, GetMonID(mon));
    data_ov095_0689d2f9(&flow->posPoke, monId);
}



void ServerControl_SwitchOutConfirm(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0xd14);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x54);
    BattleEventVar_Pop(0xd17);
}


u16 func_ov093_021bde48(BattleMon *mon, BattleAction *action) {
    u16 move;

    if (CheckCondition(mon, 0x17) && action->bits.move != 0xa5) {
        move = Condition_GetParam(GetConditionContinuationParam(mon, 0x17));
        if (move != action->bits.move && func_ov093_021d55a0(mon, move)) {
            return move;
        }
    }
    return 0;
}

void func_ov093_021bde98(BtlServerFlow *flow, BattleMon *mon, BattleAction *action, u32 key) {
    BtlFlowFightWork work;
    u8 target;
    u32 prevStatus;
    u8 cond19;
    u8 cond1a;
    u8 hasDelegate;
    u8 usedMove;
    u8 moveSlot;
    u8 targetCount;
    u16 move;
    u16 actualMove;
    u16 encoreMove;

    prevStatus = func_ov093_021d5c08(mon);
    func_ov093_021bd1e0(flow->moveEffect);
    func_ov093_021be518(flow->unk4B0);
    work.called.move = 0;
    work.called.target = 6;
    encoreMove = func_ov093_021bde48(mon, action);
    if (encoreMove) {
        BattleAction_SetFightParam(action, encoreMove,
                                   func_ov093_021d80bc(flow->mainModule, flow->pokeCon, mon, encoreMove));
    }
    move = action->bits.move;
    moveSlot = func_ov093_021d5778(mon, move);
    actualMove = move;
    target = action->bits.target;
    usedMove = FALSE;
    work.result = 0;
    cond19 = CheckCondition(mon, 0x19);
    cond1a = CheckCondition(mon, 0x1a);
    MoveEvent_AddItem(mon, move, key);
    func_ov093_021be924(flow, mon, move);
    do {
        if (func_ov093_021c001c(flow, mon, move, cond19 || cond1a)) {
            break;
        }
        if (!func_ov093_021c62d8(flow, mon, move, target, &work.called)) {
            func_ov093_021c5604(flow, mon, move);
            ServerEvent_GetMoveParam(flow, move, mon, flow->unk1AB4);
            func_ov093_021ca7f4(flow, mon, target, flow->unk1AB4, flow->unk850);
            func_ov093_021c07ac(flow, mon, move, moveSlot, flow->unk850);
            usedMove = TRUE;
            func_ov093_021c0450(flow, mon, move, 0x1a);
            break;
        }
        hasDelegate = work.called.move != 0 ? TRUE : FALSE;
        if (hasDelegate) {
            func_ov093_021c5604(flow, mon, move);
            func_ov093_021bebd8(flow, mon, move, target);
            MoveEvent_AddItem(mon, work.called.move, key);
            actualMove = work.called.move;
            target = work.called.target;
        } else {
            actualMove = move;
        }
        ServerEvent_GetMoveParam(flow, move, mon, flow->unk1AB4);
        if (hasDelegate) {
            ServerEvent_GetMoveParam(flow, actualMove, mon, flow->unk1AB0);
            flow->unk1AB0->flags.raw |= 4;
            flow->unk1AB0->originalMove = move;
            if (func_ov093_021be3a8(flow, mon, flow->unk1AB0)) {
                func_ov093_021c07ac(flow, mon, move, moveSlot, flow->unk850);
                usedMove = TRUE;
                break;
            }
        } else {
            *flow->unk1AB0 = *flow->unk1AB4;
        }
        data_ov095_0689cb3d(flow->unk850);
        data_ov095_0689cb3d(flow->unk854);
        func_ov093_021ca7f4(flow, mon, target, flow->unk1AB0, flow->unk850);
        targetCount = data_ov095_0689cd41(flow->unk850);
        data_ov095_0689cee5(flow->unk850);
        data_ov095_0689cd79(flow->unk850, flow->unk854);
        data_ov095_0689cd49(flow->unk854, targetCount);
        if (!cond19 && !cond1a && moveSlot != 4) {
            func_ov093_021c07ac(flow, mon, move, moveSlot, flow->unk850);
            usedMove = TRUE;
        }
        func_ov093_021be66c(flow, mon, flow->unk1AB4, 0x22);
        func_ov093_021be620(flow, mon, flow->unk1AB0);
        if (func_ov093_021c6458(flow, mon, move, actualMove, &flow->message)) {
            BattleHandler_SetString(flow, &flow->message);
            BattleHandler_StrClear(&flow->message);
        } else {
            func_ov093_021c5604(flow, mon, actualMove);
        }
        flow->unk1F74 = actualMove;
        if (CheckCondition(mon, 0x22)) {
            ServerControl_CureCondition(flow, mon, 0x22, 0);
            func_ov093_021d5fc0(mon, 0xe);
        }
        if (func_ov093_021c024c(flow, mon, actualMove)) {
            break;
        }
        if (func_ov093_021be7e4(flow, mon, actualMove, target, &work)) {
            break;
        }
        if (func_ov093_021be6ec(flow, mon, actualMove, target)) {
            break;
        }
        data_ov095_0689d01d(&flow->wazaRec, actualMove, flow->unk10, GetMonID(mon));
        func_ov093_021be66c(flow, mon, flow->unk1AB0, 0x23);
        if (func_ov093_021beab4(flow, mon, actualMove, flow->unk854, flow->unk4AC)) {
            func_ov093_021be550(flow, mon, actualMove, flow->unk854);
        } else {
            work.result = func_ov093_021bec58(flow, mon, actualMove, flow->unk854);
        }
    } while (0);
    func_ov093_021d5fc0(mon, 3);
    if (usedMove || cond19 || actualMove == 0xa5) {
        func_ov093_021d6744(mon, target, work.result, flow->unk1AB0->type, actualMove, move);
        ServerCmd_PutMoveResult(flow->queue, GetMonID(mon), target, work.result, flow->unk1AB0->type, actualMove,
                                move);
    } else if (GetConditionCount(mon, 3)) {
        ServerControl_SetMonCounter(flow, mon, 3, 0);
    }
    if (prevStatus != 0x10) {
        if (func_ov093_021d5c08(mon) != 0x10) {
            func_ov093_021bfe60(flow, mon);
        }
    }
    if (GetTurnFlag(mon, 0xb)) {
        func_ov093_021cd4b0(flow->queue, 0x31, GetMonID(mon), 0);
        ServerControl_SkyDropCheckRelease(flow, mon, FALSE);
    }
    func_ov093_021be994(flow, mon, actualMove, work.result);
    if (work.called.move != 0) {
        func_ov093_021e0394(mon, work.called.move);
    }
    func_ov093_021e0394(mon, move);
    func_ov093_021be400(flow, actualMove);
}























BOOL func_ov093_021be3a8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param) {
    u16 move;
    u32 condition;

    move = param->move;
    condition = 0;
    if (CheckCondition(mon, 0xf) && getMoveFlag(move, 0xc)) {
        condition = 0xd;
    } else if (func_ov093_021ef948(2) && getMoveFlag(move, 9)) {
        condition = 0x14;
    }
    if (condition != 0) {
        func_ov093_021c0450(flow, mon, move, condition);
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021be400(BtlServerFlow *flow, u16 move) {
    u32 i;
    BattleMon *mon;
    BattleMon *target;
    u32 state;

    for (i = 0; i < flow->unk4B0->count; i++) {
        mon = GetPokeParam(flow->pokeCon, flow->unk4B0->monIds[i]);
        target = GetPokeParam(flow->pokeCon, flow->unk4B0->unk0D[i]);
        if (CanPokemonBattle(mon)) {
            state = PushState(&flow->actionState, 0xe49);
            func_ov093_021bea60(flow, mon, target, move);
            PopState(&flow->actionState, state, 0xe4b);
            ServerEvent_GetMoveParam(flow, move, mon, flow->unk1AB0);
            flow->unk1AB0->flags.unk0 = TRUE;
            func_ov093_021ca7f4(flow, mon, flow->unk4B0->targets[i], flow->unk1AB0, flow->unk864);
            func_ov093_021d7394(GetMonID(mon));
            MoveEvent_AddItem(mon, move, GetBattleMonStat(mon, 0xc));
            func_ov093_021bec58(flow, mon, move, flow->unk864);
            RemoveForce(mon, move);
            func_ov093_021d73d4(GetMonID(mon));
        }
    }
}

void func_ov093_021be518(BtlFlowReactionList *list) {
    u32 i;

    list->count = 0;
    for (i = 0; i < 6; i++) {
        list->monIds[i] = 0x1f;
    }
}

void func_ov093_021be52c(BtlFlowReactionList *list, u8 monId, u8 arg2, u8 target) {
    if (list->count < 6) {
        list->monIds[list->count] = monId;
        list->unk0D[list->count] = arg2;
        list->targets[list->count] = target;
        list->count++;
    }
}

void func_ov093_021be550(BtlServerFlow *flow, BattleMon *attacker, u16 move, void *targets) {
    BattleMon *mon;
    u32 state;

    if (flow->unk4AC->count != 0) {
        mon = GetPokeParam(flow->pokeCon, flow->unk4AC->monIds[0]);
        state = PushState(&flow->actionState, 0xe88);
        func_ov093_021bea14(flow, mon, attacker, move);
        PopState(&flow->actionState, state, 0xe8a);
        ServerEvent_GetMoveParam(flow, move, mon, flow->unk1AB0);
        func_ov093_021ca7f4(flow, mon, flow->unk4AC->targets[0], flow->unk1AB0, flow->unk864);
        if (CheckCondition(mon, 0xf) && getMoveFlag(move, 0xc)) {
            func_ov093_021c0520(flow, mon, move, 0xd);
            return;
        }
        MoveEvent_AddItem(mon, move, GetBattleMonStat(mon, 0xc));
        func_ov093_021bec58(flow, mon, move, flow->unk864);
        RemoveForce(mon, move);
    }
}

void func_ov093_021be620(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param) {
    BattleEventVar_Push(0xeab);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(0x16, param->type);
    BattleEvent_CallHandlers(flow, 0xa2);
    param->type = BattleEventVar_GetValue(0x16);
    BattleEventVar_Pop(0xeb1);
}

void func_ov093_021be66c(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 event) {
    u32 state;

    state = PushState(&flow->actionState, 0xebe);
    func_ov093_021be6a8(flow, mon, param, event);
    PopState(&flow->actionState, state, 0xec2);
}

void func_ov093_021be6a8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 event) {
    BattleEventVar_Push(0xecf);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEvent_CallHandlers(flow, event);
    BattleEventVar_Pop(0xed4);
}

// Moves that combine with each other when allies use them in the same turn
static const u16 data_ov093_021efdb8[3] = { MOVE_FIRE_PLEDGE, MOVE_WATER_PLEDGE, MOVE_GRASS_PLEDGE };

BOOL func_ov093_021be6ec(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target) {
    u32 i;
    u32 count;
    u8 monId;
    u8 partnerId;
    BattleMon *partner;
    u32 index;
    u32 minIndex;
    ActionOrderEntry *entry;
    ActionOrderEntry *found[2];

    for (i = 0; i < 3; i++) {
        if (move == data_ov093_021efdb8[i]) {
            break;
        }
    }
    if (i != 3 && !GetTurnFlag(mon, 0xa) && !func_ov093_021d6e74(mon)) {
        count = 0;
        monId = GetMonID(mon);
        for (i = 0; i < 3; i++) {
            if (move != data_ov093_021efdb8[i]) {
                found[count] = func_ov093_021bcb14(flow, data_ov093_021efdb8[i], monId, target);
                if (found[count] != NULL) {
                    count++;
                    if (count >= 2) {
                        break;
                    }
                }
            }
        }
        if (count != 0) {
            minIndex = 6;
            partner = found[0]->mon;
            for (i = 0; i < count; i++) {
                entry = found[i];
                index = func_ov093_021bcb68(flow, entry);
                if (index < minIndex) {
                    minIndex = index;
                    partner = entry->mon;
                }
            }
            partnerId = GetMonID(partner);
            func_ov093_021d5fc0(mon, 0xa);
            func_ov093_021d6e40(partner, monId, move);
            func_ov093_021cd64c(flow->queue, 0x5b, 0x47a, monId, partnerId, 0xffff0000);
            ActionOrder_InterruptReserve(flow, partnerId);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov093_021be7e4(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target, BtlFlowFightWork *work) {
    u32 state;
    BOOL result;
    u8 pos;

    state = PushState(&flow->actionState, 0xf36);
    result = func_ov093_021be878(flow, mon, target);
    work->result = 0;
    if (result) {
        pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
        if (BattleHandler_Result(flow) == 2) {
            func_ov093_021cd4b0(flow->queue, 0x30, pos, target, move, 0);
            func_ov093_021be8c4(flow, mon);
            work->result = 1;
        } else {
            func_ov093_021c5718(flow, mon, move);
        }
    }
    PopState(&flow->actionState, state, 0xf49);
    return result;
}

BOOL func_ov093_021be878(BtlServerFlow *flow, BattleMon *mon, u8 target) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0xf5b);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(3, monId);
    BattleEventVar_SetConstValue(0xd, target);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, 6);
    value = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0xf61);
    return value;
}

void func_ov093_021be8c4(BtlServerFlow *flow, BattleMon *mon) {
    u32 state;

    state = PushState(&flow->actionState, 0xf6f);
    func_ov093_021be8f4(flow, mon);
    PopState(&flow->actionState, state, 0xf74);
}

void func_ov093_021be8f4(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;

    BattleEventVar_Push(0xf80);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(3, monId);
    BattleEvent_CallHandlers(flow, 7);
    BattleEventVar_Pop(0xf83);
}

void func_ov093_021be924(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u32 state;

    state = PushState(&flow->actionState, 0xf91);
    func_ov093_021be95c(flow, mon, move);
    PopState(&flow->actionState, state, 0xf93);
}

void func_ov093_021be95c(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u8 monId;

    BattleEventVar_Push(0xfa0);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(3, monId);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEvent_CallHandlers(flow, 3);
    BattleEventVar_Pop(0xfa4);
}

void func_ov093_021be994(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 result) {
    u32 state;

    state = PushState(&flow->actionState, 0xfb1);
    func_ov093_021be9d0(flow, mon, move, result);
    PopState(&flow->actionState, state, 0xfb3);
}

void func_ov093_021be9d0(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 result) {
    u8 monId;

    BattleEventVar_Push(0xfc0);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetConstValue(0x51, result);
    BattleEvent_CallHandlers(flow, 4);
    BattleEventVar_Pop(0xfc5);
}

void func_ov093_021bea14(BtlServerFlow *flow, BattleMon *mon, BattleMon *attacker, u16 move) {
    u8 monId;

    BattleEventVar_Push(0xfd3);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    monId = GetMonID(attacker);
    BattleEventVar_SetConstValue(3, monId);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEvent_CallHandlers(flow, 8);
    BattleEventVar_Pop(0xfd8);
}

void func_ov093_021bea60(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move) {
    u8 monId;

    BattleEventVar_Push(0xfe5);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    monId = GetMonID(target);
    BattleEventVar_SetConstValue(3, monId);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, 9);
    BattleEventVar_Pop(0xfeb);
}

BOOL func_ov093_021beab4(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets, BtlFlowReactionList *list) {
    u8 monId;
    u8 target;

    monId = 0x1f;
    target = 0x1f;
    func_ov093_021beb14(flow, mon, move, targets, &monId, &target);
    if (monId != 0x1f) {
        list->targets[0] = target != 0x1f ? GetBattlePos(&flow->posPoke, target) : 6;
        list->monIds[0] = monId;
        list->count = 1;
        return TRUE;
    }
    return FALSE;
}




BOOL func_ov093_021beb14(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets, u8 *monId, u8 *target) {
    u32 count;
    u32 i;

    count = data_ov095_0689cd39(targets);
    BattleEventVar_Push(0x1028);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(5, count);
    for (i = 0; i < count; i++) {
        BattleEventVar_SetConstValue(i + 6, GetMonID(data_ov095_0689cc71(targets, i)));
    }
    BattleEventVar_SetRewriteOnceValue(2, 0x1f);
    BattleEventVar_SetValue(4, 0x1f);
    BattleEvent_CallHandlers(flow, 0x1a);
    *monId = BattleEventVar_GetValue(2);
    *target = BattleEventVar_GetValue(4);
    BattleEventVar_Pop(0x103b);
    if (*monId != 0x1f) {
        return TRUE;
    }
    return FALSE;
}







void func_ov093_021bebd8(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target) {
    func_ov093_021cd4b0(flow->queue, 0x30, MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon)), target,
                        move, 0);
}

BOOL func_ov093_021bec08(BtlServerFlow *flow, BattleMon *mon) {
    if (func_ov093_021b8ddc(GetMonID(mon)) == GetPlayerClientID(flow->mainModule)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021bec2c(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    if (func_ov093_021bec08(flow, mon)) {
        if (move == 0x96) {
            func_ov093_021ba1b4(flow->mainModule, 0x49);
        } else if (move == 0xa5) {
            func_ov093_021ba1b4(flow->mainModule, 0x4a);
        }
    }
}

BOOL func_ov093_021bec58(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets) {
    s32 quality;
    u8 monId;
    BOOL success;
    BOOL isDamage;
    BOOL isDamage2;
    u32 reserved;
    u32 state;
    u32 result;

    quality = PML_MoveGetQuality(move);
    monId = GetMonID(mon);
    success = TRUE;
    isDamage = FALSE;
    isDamage2 = FALSE;
    func_ov093_021bec2c(flow, mon, move);
    if (getMoveFlag(move, 1)) {
        switch (func_ov093_021bfc08(flow, mon, targets, move)) {
        case 0:
            break;
        case 1:
            return isDamage;
        case 2:
            return success;
        case 3:
            return isDamage;
        case 4:
            break;
        }
    }
    data_ov095_0689cb3d(flow->unk860);
    reserved = SCQUE_RESERVE_Pos(flow->queue, 0x30);
    func_ov093_021bd200(flow->moveEffect, flow, mon, targets);
    state = PushState(&flow->actionState, 0x10a8);
    result = func_ov093_021c65a8(flow, mon, move, targets);
    PopState(&flow->actionState, state, 0x10aa);
    if (result) {
        if (!flow->moveEffect->enabled) {
            flow->moveEffect->enabled = TRUE;
        }
        if (result == 2) {
            func_ov093_021bfbd0(flow, move, flow->moveEffect, reserved);
            return TRUE;
        }
    }
    switch (quality) {
    case 0:
    case 4:
    case 6:
    case 7:
    case 8:
        isDamage = TRUE;
        isDamage2 = TRUE;
        break;
    case 9:
        isDamage2 = TRUE;
        break;
    }
    data_ov095_0689cee5(targets);
    func_ov093_021bf04c(flow, flow->unk1AB0, mon, targets);
    func_ov093_021bf0e4(flow, mon, targets, flow->unk1AB0, isDamage);
    if (data_ov095_0689cd51(targets)) {
        if (!flow->unk78A_4) {
            func_ov093_021c5718(flow, mon, move);
        }
        success = FALSE;
    }
    if (success) {
        if (quality != 9) {
            func_ov093_021bf83c(flow, flow->unk1AB0, mon, targets);
        }
        func_ov093_021cc8a0(flow->unk1F88);
        if (isDamage2 || func_ov093_021bf168(flow, flow->unk1AB0, mon)) {
            func_ov093_021bf3d8(flow, flow->unk1AB0, mon, targets, flow->unk1F88);
        }
        func_ov093_021bf4ac(flow, flow->unk1AB0, mon, targets, flow->unk1F88);
        if (quality != 9) {
            func_ov093_021bf8d4(flow, flow->unk1AB0, mon, targets);
        }
        if (data_ov095_0689cd51(targets)) {
            func_ov093_021bf27c(flow, monId, move);
            success = FALSE;
        } else {
            func_ov093_021bf1b4(flow, flow->unk1AB0, mon, targets, isDamage2);
            if (data_ov095_0689cd51(targets)) {
                func_ov093_021c5718(flow, mon, move);
                func_ov093_021bf27c(flow, monId, move);
                success = FALSE;
            }
        }
    }
    if (success) {
        if (isDamage) {
            func_ov093_021c091c(flow, flow->unk1AB0, mon, targets, flow->unk1F88, 0);
        } else {
            switch (quality) {
            case 2:
                ServerControl_SimpleEffect(flow, flow->unk1AB0, mon, targets);
                break;
            case 1:
                ServerControl_SimpleCondition(flow, move, mon, targets);
                break;
            case 5:
                func_ov093_021c3184(flow, flow->unk1AB0, mon, targets);
                break;
            case 9:
                ServerControl_OHKO(flow, flow->unk1AB0, mon, targets);
                break;
            case 0xc:
                ServerControl_ForceSwitch(flow, move, mon, targets);
                break;
            case 3:
                func_ov093_021c3274(flow, move, mon, targets);
                break;
            case 0xa:
                ServerControl_FieldEffect(flow, flow->unk1AB0, mon);
                break;
            case 0xb:
            case 0xd:
                func_ov093_021c3d08(flow, flow->unk1AB0, mon, targets);
                break;
            }
        }
        if (flow->moveEffect->enabled) {
            if (!flow->moveEffect->unk05_1) {
                func_ov093_021bfbd0(flow, move, flow->moveEffect, reserved);
            }
            data_ov095_0689d051(&flow->wazaRec);
            if (!IsFainted(mon) && getMoveFlag(move, 2)) {
                scPut_SetContFlag(flow, mon, 0xc);
            }
            func_ov093_021bf244(flow, monId, move);
        } else {
            func_ov093_021bf27c(flow, monId, move);
            success = FALSE;
        }
    } else if (flow->moveEffect->enabled) {
        func_ov093_021bfbd0(flow, move, flow->moveEffect, reserved);
    }
    func_ov093_021bf2b4(flow, monId, move);
    ServerControl_CheckFainted(flow, mon);
    return success;
}



















void func_ov093_021bf04c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    BattleMon *target;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (func_ov093_021bfa00(flow, mon, target, param->move)) {
            data_ov095_0689cc15(targets, target);
            func_ov093_021c572c(flow, target, param->move);
            flow->unk78A_4 = TRUE;
        }
        target = data_ov095_0689cc8d(targets);
    }
}













void func_ov093_021bf0e4(BtlServerFlow *flow, BattleMon *mon, void *targets, BtlFlowMoveParam *param, BOOL flag) {
    BattleMon *target;

    if (data_ov095_0689cd41(targets) == 1) {
        target = data_ov095_0689cc71(targets, 0);
        if (target != NULL && GetMonID(target) == GetMonID(mon) && param->targetType != 7 &&
            (GetTurnFlag(mon, 0xc) || flag)) {
            data_ov095_0689cc15(targets, target);
        }
    }
}










BOOL func_ov093_021bf168(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon) {
    BOOL result;

    BattleEventVar_Push(0x1194);
    BattleEventVar_SetValue(3, GetMonID(mon));
    BattleEventVar_SetValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, 0x3d);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x119a);
    return result;
}

void func_ov093_021bf1b4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, BOOL flag) {
    BattleMon *target;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (mon != target && IsSubstituteActive(target) && !flag && !getMoveFlag(param->move, 0xd)) {
            data_ov095_0689cc15(targets, target);
        }
        target = data_ov095_0689cc8d(targets);
    }
}













void func_ov093_021bf244(BtlServerFlow *flow, u8 monId, u16 move) {
    u32 state;

    state = PushState(&flow->actionState, 0x11e7);
    func_ov093_021bf2ec(flow, monId, move, 0x25);
    PopState(&flow->actionState, state, 0x11e9);
}

void func_ov093_021bf27c(BtlServerFlow *flow, u8 monId, u16 move) {
    u32 state;

    state = PushState(&flow->actionState, 0x11f6);
    func_ov093_021bf2ec(flow, monId, move, 0x26);
    PopState(&flow->actionState, state, 0x11f8);
}

void func_ov093_021bf2b4(BtlServerFlow *flow, u8 monId, u16 move) {
    u32 state;

    state = PushState(&flow->actionState, 0x1205);
    func_ov093_021bf2ec(flow, monId, move, 0x27);
    PopState(&flow->actionState, state, 0x1207);
}

void func_ov093_021bf2ec(BtlServerFlow *flow, u8 monId, u16 move, u32 event) {
    BattleEventVar_Push(0x1214);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEvent_CallHandlers(flow, event);
    BattleEventVar_Pop(0x1218);
}

BOOL IsGuaranteedHit(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender) {
    u8 lockedId;

    if (data_ov095_0689d59d(&flow->posPoke, flow->mainModule, GetMonID(attacker)) &&
        GetBattleMonStat(attacker, 0x11) == 0x63) {
        return TRUE;
    }
    if (data_ov095_0689d59d(&flow->posPoke, flow->mainModule, GetMonID(defender)) &&
        GetBattleMonStat(defender, 0x11) == 0x63) {
        return TRUE;
    }
    if (CheckCondition(attacker, 0x1c)) {
        return TRUE;
    }
    if (CheckCondition(attacker, 0x1d)) {
        lockedId = GetDisabledMove(attacker, 0x1d);
        if (lockedId == GetMonID(defender)) {
            return TRUE;
        }
    }
    return FALSE;
}





void func_ov093_021bf3d8(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data) {
    BattleMon *target;
    BOOL hit;

    func_ov093_021cc8a0(data);
    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        hit = func_ov093_021c6e3c(flow, mon, target, param, TRUE);
        func_ov093_021cc8b0(data, GetMonID(target), hit);
        if (!hit) {
            data_ov095_0689cc15(targets, target);
            func_ov093_021c7c24(&flow->unk1F7C, flow, mon, target, FALSE);
            if (func_ov093_021b8ddc(GetMonID(mon)) == GetPlayerClientID(flow->mainModule)) {
                func_ov093_021ba1b4(flow->mainModule, 0x4b);
            }
        }
        target = data_ov095_0689cc8d(targets);
    }
}
















void func_ov093_021bf4ac(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data) {
    BattleMon *target;

    data_ov095_0689cc85(targets);
    for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
        if (!IsGuaranteedHit(flow, mon, target) && func_ov093_021bf6e8(flow, param, mon, target, data, 0x2b)) {
            data_ov095_0689cc15(targets, target);
        }
    }
    data_ov095_0689cc85(targets);
    for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
        if (func_ov093_021bf6e8(flow, param, mon, target, data, 0x2c)) {
            data_ov095_0689cc15(targets, target);
        }
    }
    if (getMoveFlag(param->move, 3)) {
        data_ov095_0689cc85(targets);
        for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
            if (GetTurnFlag(target, 7) && !func_ov093_021c6668(flow, mon, target, param->move)) {
                data_ov095_0689cc15(targets, target);
                func_ov093_021cd64c(flow->queue, 0x5b, 0x20b, GetMonID(target), 0xffff0000);
            }
        }
    }
    data_ov095_0689cc85(targets);
    for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
        if (func_ov093_021bf6e8(flow, param, mon, target, data, 0x2d)) {
            data_ov095_0689cc15(targets, target);
        }
    }
}
















































BOOL func_ov093_021bf6e8(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, BattleMon *target, void *data,
                         u32 event) {
    u32 state;
    BOOL result;
    BOOL silent;

    state = PushState(&flow->actionState, 0x12b0);
    silent = FALSE;
    result = func_ov093_021bf78c(flow, param, event, mon, target, data, &flow->message, &silent);
    if (result) {
        if (BattleHandler_StrIsEnabled(&flow->message)) {
            BattleHandler_SetString(flow, &flow->message);
            BattleHandler_StrClear(&flow->message);
        } else if (BattleHandler_Result(flow) == 0 && !silent) {
            func_ov093_021cd64c(flow->queue, 0x5b, 0xd2, GetMonID(target), 0xffff0000);
        }
    }
    PopState(&flow->actionState, state, 0x12ca);
    return result;
}

BOOL func_ov093_021bf78c(BtlServerFlow *flow, BtlFlowMoveParam *param, u32 event, BattleMon *mon, BattleMon *target,
                         void *data, BattleHandlerString *string, BOOL *silent) {
    u32 value;
    BOOL result;

    value = func_ov093_021cc8c0(data, GetMonID(target));
    BattleHandler_StrClear(string);
    BattleEventVar_Push(0x12e2);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x4e, param->flags.unk0);
    BattleEventVar_SetConstValue(0x3f, (intptr_t)string);
    BattleEventVar_SetConstValue(0x38, value);
    BattleEventVar_SetRewriteOnceValue(0x40, 0);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, event);
    result = BattleEventVar_GetValue(0x40);
    *silent = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x12ef);
    return result;
}


void func_ov093_021bf83c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    BattleMon *target;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (!IsGuaranteedHit(flow, mon, target) && func_ov093_021c6948(flow, mon, target, param->move)) {
            data_ov095_0689cc15(targets, target);
            func_ov093_021c572c(flow, target, param->move);
        }
        target = data_ov095_0689cc8d(targets);
    }
}













void func_ov093_021bf8d4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    BattleMon *target;

    if (data_ov095_0689cd41(targets) != 1 || data_ov095_0689cc71(targets, 0) != mon) {
        data_ov095_0689cc85(targets);
        target = data_ov095_0689cc8d(targets);
        while (target != NULL) {
            if (!func_ov093_021bf9a4(flow, mon, target, param) && !func_ov093_021bfa60(flow, mon, target, param)) {
                data_ov095_0689cc15(targets, target);
                func_ov093_021c572c(flow, target, param->move);
            }
            target = data_ov095_0689cc8d(targets);
        }
    }
}



















BOOL func_ov093_021bf9a4(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param) {
    BOOL result;

    BattleEventVar_Push(0x1341);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(0x51, 0);
    BattleEvent_CallHandlers(flow, 0x1c);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x1348);
    return result;
}

BOOL func_ov093_021bfa00(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move) {
    if (BtlSetup_GetBattleStyle(flow->mainModule) == 2) {
        u8 pos1, pos2;
        if (move != 0 && getMoveFlag(move, 0xb)) {
            return FALSE;
        }
        pos1 = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
        pos2 = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(target));
        if (!func_ov093_021b9a6c(pos1, pos2)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov093_021bfa60(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param) {
    u8 ignore;
    s8 accuracy;
    u32 base;
    s8 evasion;
    u32 ratio;
    s32 stage;
    u32 chance;

    if (IsGuaranteedHit(flow, mon, target)) {
        return TRUE;
    }
    if (func_ov093_021c69b8(flow, mon, target, param->move)) {
        return TRUE;
    }
    if (CheckCondition(target, 0x20)) {
        return TRUE;
    }
    base = func_ov093_021c6a04(flow, mon, target, param);

    BattleEventVar_Push(0x1396);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetRewriteOnceValue(0x4b, 0);
    BattleEventVar_SetValue(0x27, GetBattleMonStat(mon, 6));
    BattleEventVar_SetValue(0x28, GetBattleMonStat(target, 7));
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x29, 0x20000);
    BattleEvent_CallHandlers(flow, 0x33);
    accuracy = BattleEventVar_GetValue(0x27);
    evasion = BattleEventVar_GetValue(0x28);
    ignore = BattleEventVar_GetValue(0x4b);
    if ((CheckCondition(target, 0x11) && evasion > 6) || ignore) {
        evasion = 6;
    }
    if (GetTurnFlag(mon, 0xe)) {
        BattleEventVar_MulValue(0x35, 0x1333);
    }
    ratio = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x13af);

    stage = accuracy + 6 - evasion;
    if (stage < 0) {
        stage = 0;
    }
    if (stage > 12) {
        stage = 12;
    }
    chance = fixed_round(func_ov093_021d7908((u8)base, (u8)(s8)stage), ratio);
    if (chance > 100) {
        chance = 100;
    }
    if (ReturnZero(flow->mainModule, 6)) {
        chance = 100;
    }
    return BattleRandom(100) < (u8)chance;
}

void func_ov093_021bfbd0(BtlServerFlow *flow, u16 move, BattleMoveEffectState *effect, u32 reserved) {
    u32 shown = effect->unk00 ? effect->unk00 : move;
    func_ov093_021cd568(flow->queue, (u16)reserved, 0x30, effect->pos1, effect->pos2, shown, effect->index);
    effect->index = 0;
    effect->unk05_1 = 1;
}

u32 func_ov093_021bfc08(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move) {
    u32 state;
    u32 result;

    if (!CheckCondition(mon, 0x1a)) {
        u8 pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
        if (func_ov093_021c66ac(flow, mon, targets)) {
            func_ov093_021c5718(flow, mon, move);
            return 1;
        }
        if (!func_ov093_021c6720(flow, mon, move)) {
            if (func_ov093_021bfd24(flow, mon, pos, targets, move)) {
                BattleCondition cond = AddTurnCondition(2, move);
                ServerDisplay_AddCondition(flow, mon, 0x1a, cond);
            }
            return 2;
        }
        if (!func_ov093_021bfd24(flow, mon, pos, targets, move)) {
            return 1;
        }
        state = PushState(&flow->actionState, 0x1404);
        func_ov093_021c6878(flow, mon, move);
        PopState(&flow->actionState, state, 0x1406);
    }
    flow->moveEffect->index = 1;
    state = PushState(&flow->actionState, 0x140e);
    result = func_ov093_021c68a8(flow, mon, targets, move);
    PopState(&flow->actionState, state, 0x1410);
    func_ov093_021d5fc0(mon, 0xb);
    func_ov093_021bfe60(flow, mon);
    return result ? 4 : 3;
}

BOOL func_ov093_021bfd24(BtlServerFlow *flow, BattleMon *mon, u8 pos, void *targets, u16 move) {
    u32 state;
    BOOL result;
    BOOL failed = FALSE;
    u8 id = 0x1f;

    state = PushState(&flow->actionState, 0x1428);
    result = func_ov093_021c676c(flow, mon, targets, move, &id, &failed);
    if (result) {
        u8 targetPos = flow->unk77F;
        if (data_ov095_0689cd39(targets)) {
            targetPos = GetBattlePos(&flow->posPoke, GetMonID(data_ov095_0689cc71(targets, 0)));
        }
        func_ov093_021cd4b0(flow->queue, 0x30, pos, targetPos, move, 0);
    }
    PopState(&flow->actionState, state, 0x1435);
    if (result) {
        state = PushState(&flow->actionState, 0x1439);
        func_ov093_021c6848(flow, mon);
        PopState(&flow->actionState, state, 0x143b);
        if (IsSemiInvulnMove(mon)) {
            func_ov093_021cd4b0(flow->queue, 0x31, GetMonID(mon), 1);
        }
        if (id != 0x1f) {
            func_ov093_021cd4b0(flow->queue, 0x31, id, 1);
        }
    } else if (!failed) {
        func_ov093_021c5718(flow, mon, move);
    }
    return result;
}










void func_ov093_021bfe60(BtlServerFlow *flow, BattleMon *mon) {
    u32 flag;
    if (CheckCondition(mon, 0x1a)) {
        ServerControl_CureCondition(flow, mon, 0x1a, 0);
        func_ov093_021d5fc0(mon, 0xb);
    }
    while ((flag = func_ov093_021d5c08(mon)) != 0x10) {
        scPut_ResetContFlag(flow, mon, flag);
    }
}

BOOL func_ov093_021bfeac(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BOOL *failed) {
    *failed = FALSE;
    if (!IsFainted(target) && !IsSubstituteActive(target) && !IsSemiInvulnMove(target)) {
        if (!GetTurnFlag(target, 7)) {
            u8 targetId = GetMonID(target);
            BattleCondition cond;

            scPut_SetContFlag(flow, attacker, 3);
            scPut_SetContFlag(flow, target, 3);
            ServerControl_SetMonCounter(flow, attacker, 4, targetId + 1);
            cond = func_ov093_021e899c(GetMonID(attacker));
            ServerControl_AddCondition(flow, target, attacker, 0x21, cond, FALSE, FALSE, NULL);
            return TRUE;
        }
        func_ov093_021cd64c(flow->queue, 0x5b, 0x20b, GetMonID(target), 0xffff0000);
        *failed = TRUE;
    }
    return FALSE;
}

void ServerControl_SkyDropCheckRelease(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    u8 targetId = BtlFlow_GetSkyDropTarget(mon);

    if (targetId != 0x1f) {
        BattleMon *target = GetPokeParam(flow->pokeCon, targetId);
        if (CheckCondition(target, 0x21)) {
            ServerControl_CureCondition(flow, target, 0x21, 0);
            scPut_ResetContFlag(flow, target, 3);
            if (DoesBattleMonExist(&flow->posPoke, targetId)) {
                func_ov093_021cd4b0(flow->queue, 0x31, targetId, 0);
                ServerDisplay_SkyDropTargetAppear(flow, target, 0x465);
            }
        }
        ServerControl_SetMonCounter(flow, mon, 4, 0);
        if (!flag && !IsFainted(mon)) {
            func_ov093_021cd4b0(flow->queue, 0x31, GetMonID(mon), 0);
        }
    }
}




BOOL func_ov093_021c001c(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 flag) {
    u32 status;
    BOOL thawedByMove = FALSE;
    u32 cause = 0;

    func_ov093_021c03d8(flow, mon, move);
    status = GetBattleMonStatus(mon);
    do {
        if (status == 2 && !func_ov093_021c0274(flow, mon, move, 2)) {
            cause = 2;
            break;
        }
        thawedByMove = func_ov093_021c0404(flow, mon, move);
        status = GetBattleMonStatus(mon);
        if (status == 3 && !thawedByMove) {
            cause = 4;
            break;
        }
        if (flow->unk1F78 == 4) {
            cause = 0x16;
            break;
        }
        if (flow->unk1F78 == 2) {
            cause = 0x17;
            break;
        }
        if (!flag) {
            u8 slot = func_ov093_021d5778(mon, move);
            if (slot != 4 && GetMovePP(mon, slot) == 0) {
                cause = 1;
                break;
            }
        }
        cause = func_ov093_021c02cc(flow, mon, move, 0x1e);
        if (cause != 0) {
            break;
        }
        if (GetTurnFlag(mon, 6)) {
            cause = 7;
            break;
        }
        if (GetTurnFlag(mon, 4)) {
            cause = 6;
            break;
        }
        if (CheckCondition(mon, 0xd) && move != 0xa5) {
            BattleConditionCont cont = GetConditionContinuationParam(mon, 0xd);
            if (move == Condition_GetParam(cont)) {
                cause = 9;
                break;
            }
        }
        if (CheckCondition(mon, 0xf) && getMoveFlag(move, 0xc)) {
            cause = 0xd;
            break;
        }
        if (func_ov093_021ef948(2) && getMoveFlag(move, 9)) {
            cause = 0x14;
            break;
        }
        if (move != 0xa5) {
            if (CheckCondition(mon, 0x17)) {
                u16 prev = GetPreviousMoveUsed(mon);
                if (prev != 0xa5 && prev != move) {
                    cause = 0x11;
                    break;
                }
            }
            if (CheckCondition(mon, 0x19) && move != GetPreviousMoveID(mon)) {
                cause = 0x11;
                break;
            }
            if (CheckCondition(mon, 0xb) && !PML_MoveIsDamaging(move)) {
                cause = 0xa;
                break;
            }
            if (func_ov093_021ef948(3) && func_ov093_021ef91c(flow->pokeCon, mon, move)) {
                cause = 0xc;
                break;
            }
        }
        if (func_ov093_021c031c(flow, mon)) {
            cause = 5;
            break;
        }
        if (status == 1 && RollEffectChance(0x19)) {
            cause = 3;
            break;
        }
        if (func_ov093_021c03ac(flow, mon)) {
            cause = 8;
            break;
        }
    } while (0);

    if (cause != 0) {
        func_ov093_021c0450(flow, mon, move, cause);
        return TRUE;
    }
    if (status == 2) {
        func_ov093_021c0520(flow, mon, move, 2);
        ServerDisplay_AddEffectAtPosition(flow, mon, 0x256);
    } else if (thawedByMove && status == 3) {
        func_ov093_021c557c(flow, mon, status, FALSE);
        func_ov093_021cd64c(flow->queue, 0x5b, 0x12f, GetMonID(mon), move, 0xffff0000);
    }
    return FALSE;
}

BOOL func_ov093_021c024c(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u32 cause;
    if (move != 0xa5) {
        cause = func_ov093_021c02cc(flow, mon, move, 0x1f);
        if (cause != 0) {
            func_ov093_021c0450(flow, mon, move, cause);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov093_021c0274(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 status) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x15a1);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x22, status);
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEvent_CallHandlers(flow, 0x1d);
    value = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x15a8);
    return value;
}

u32 func_ov093_021c02cc(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 event) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x15bb);
    BattleEventVar_SetConstValue(0x12, move);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetRewriteOnceValue(0x22, 0);
    BattleEvent_CallHandlers(flow, event);
    value = BattleEventVar_GetValue(0x22);
    BattleEventVar_Pop(0x15c1);
    return value;
}

BOOL func_ov093_021c031c(BtlServerFlow *flow, BattleMon *mon) {
    if (flow->unk1F78 == 3) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0xbc, GetMonID(mon), 0xffff0000);
        return TRUE;
    }
    if (CheckCondition(mon, 6)) {
        if (func_ov093_021d61a8(mon)) {
            ServerControl_CureCondition(flow, mon, 6, 0);
            func_ov093_021cd64c(flow->queue, 0x5b, 0x15f, GetMonID(mon), 0xffff0000);
            return FALSE;
        }
        func_ov093_021c54c8(flow, mon);
        return RollEffectChance(0x32);
    }
    return FALSE;
}

BOOL func_ov093_021c03ac(BtlServerFlow *flow, BattleMon *mon) {
    if (CheckCondition(mon, 7)) {
        func_ov093_021c54fc(flow, mon);
        if (RollEffectChance(0x32)) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov093_021c03d8(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u32 status = GetBattleMonStatus(mon);
    if (status == 2 && func_ov093_021d6130(mon)) {
        func_ov093_021c557c(flow, mon, status, TRUE);
    }
}

BOOL func_ov093_021c0404(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u32 status = GetBattleMonStatus(mon);
    BOOL thawed = FALSE;
    BOOL thawedByMove = FALSE;
    if (status == 3) {
        if (getMoveFlag(move, 0xa)) {
            thawedByMove = TRUE;
        } else if (RollEffectChance(0x14)) {
            thawed = TRUE;
        }
    }
    if (thawed) {
        func_ov093_021c557c(flow, mon, status, TRUE);
    }
    return thawedByMove;
}

void func_ov093_021c0450(BtlServerFlow *flow, BattleMon *mon, u16 move, s32 cause) {
    u32 state;

    if (cause == 5) {
        func_ov093_021c63d0(flow, mon);
        if (ServerControl_CheckFainted(flow, mon) && ServerControl_CheckMatchup(flow)) {
            return;
        }
    } else {
        func_ov093_021c0520(flow, mon, move, cause);
        switch (cause) {
        case 2:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x256);
            break;
        case 3:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x25a);
            break;
        case 4:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x259);
            break;
        case 0x17: {
            BattleCondition cond = func_ov093_021d7d18(2);
            func_ov093_021cd4b0(flow->queue, 0x31, GetMonID(mon), 0);
            ServerDisplay_AddCondition(flow, mon, 2, cond);
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x256);
            break;
        }
        }
    }
    state = PushState(&flow->actionState, 0x1660);
    func_ov093_021c6564(flow, mon, move, cause);
    PopState(&flow->actionState, state, 0x1662);
}

void func_ov093_021c0520(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 cause) {
    u8 monId = GetMonID(mon);

    switch (cause) {
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x135, monId, 0xffff0000);
        break;
    case 3:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x114, monId, 0xffff0000);
        break;
    case 4:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x123, monId, 0xffff0000);
        break;
    case 6:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x16b, monId, 0xffff0000);
        break;
    case 7:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x16e, monId, 0xffff0000);
        break;
    case 8:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x150, monId, 0xffff0000);
        break;
    case 9:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x253, monId, move, 0xffff0000);
        break;
    case 0x12:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x1bd, monId, 0xffff0000);
        break;
    case 0xa:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x23b, monId, move, 0xffff0000);
        break;
    case 0xb:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x244, monId, move, 0xffff0000);
        break;
    case 0xc:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x24d, monId, move, 0xffff0000);
        break;
    case 0xd:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x37a, monId, move, 0xffff0000);
        break;
    case 0xe:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x37d, monId, 0xffff0000);
        break;
    case 0xf:
        func_ov093_021cd4b0(flow->queue, 0x57, monId);
        func_ov093_021cd64c(flow->queue, 0x5b, 0x1c3, monId, 0xffff0000);
        func_ov093_021cd4b0(flow->queue, 0x58, monId);
        break;
    case 0x14:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x43e, monId, move, 0xffff0000);
        break;
    case 0x16:
        if (CheckCondition(mon, 2)) {
            func_ov093_021cd64c(flow->queue, 0x5a, 0xc2, monId, 0xffff0000);
        } else {
            u8 index = BattleRandom(4);
            func_ov093_021cd64c(flow->queue, 0x5a, 0xbc + index, monId, 0xffff0000);
        }
        break;
    case 0x17:
        func_ov093_021cd64c(flow->queue, 0x5a, 0xc1, monId, 0xffff0000);
        break;
    case 1:
        func_ov093_021c5604(flow, mon, move);
        func_ov093_021cd64c(flow->queue, 0x5a, 0x52, monId, 0xffff0000);
        break;
    case 0x13:
    case 0x19:
        break;
    default:
        func_ov093_021cd64c(flow->queue, 0x5a, 0x47, monId, 0xffff0000);
        break;
    }
}

void ServerControl_SetMonCounter(BtlServerFlow *flow, BattleMon *mon, u32 counter, u8 value) {
    func_ov093_021cd4b0(flow->queue, 0x25, GetMonID(mon), (u8)counter, value);
    COUNTER_Set(mon, counter, value);
}

void func_ov093_021c07ac(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 moveSlot, void *targets) {
    u8 amount = func_ov093_021c07d4(flow, mon, moveSlot, move, targets);
    ServerControl_DecrementPP(flow, mon, moveSlot, amount);
}

u32 func_ov093_021c07d4(BtlServerFlow *flow, BattleMon *mon, u8 moveSlot, u16 move, void *targets) {
    u32 amount;
    BattleMon *target;
    u32 i;

    BattleEventVar_Push(0x16ef);
    i = 0;
    BattleEventVar_SetConstValue(5, data_ov095_0689cd39(targets));
    target = data_ov095_0689cc71(targets, 0);
    if (target != NULL) {
        do {
            BattleEventVar_SetConstValue(i + 6, GetMonID(target));
            i++;
            target = data_ov095_0689cc71(targets, i);
        } while (target != NULL);
    }
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetConstValue(0x17, moveSlot);
    BattleEventVar_SetValue(0x20, 1);
    BattleEvent_CallHandlers(flow, 0x4e);
    amount = BattleEventVar_GetValue(0x20);
    BattleEventVar_Pop(0x1704);
    return amount;
}









BOOL ServerControl_DecrementPP(BtlServerFlow *flow, BattleMon *mon, u8 moveIndex, u8 amount) {
    u8 pp = GetMovePP(mon, moveIndex);
    if (amount >= pp) {
        amount = pp;
    }
    if (amount != 0) {
        func_ov093_021c08cc(flow, mon, moveIndex, amount);
        return TRUE;
    }
    return FALSE;
}


void func_ov093_021c08cc(BtlServerFlow *flow, BattleMon *mon, u8 moveIndex, u8 amount) {
    if (!ReturnZero(flow->mainModule, 5)) {
        u8 monId = GetMonID(mon);
        func_ov093_021d5608(mon, moveIndex, amount);
        func_ov093_021cd4b0(flow->queue, 4, monId, moveIndex, amount);
        func_ov093_021d56cc(mon, moveIndex);
        func_ov093_021cd4b0(flow->queue, 6, monId, moveIndex);
    }
}

void func_ov093_021c091c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data,
                         u32 arg6) {
    u32 reserved;
    u32 damage;
    u32 showEffect;
    BattleMoveEffectState *effect;

    damage = 0;
    showEffect = FALSE;
    flow->unk1FE8[1] = damage;
    func_ov093_021c19d8(flow, mon, param);
    data_ov095_0689cb3d(flow->unk860);
    func_ov093_021c71e0(flow, mon, param->move, flow->unk4B4);
    func_ov093_021c14e0(flow, mon, param, targets);
    if (data_ov095_0689cd39(targets)) {
        if (func_ov093_021c1194(flow->unk4B4) && data_ov095_0689cd41(targets) == 1) {
            damage = func_ov093_021c0f24(flow, param, mon, targets, data);
        } else {
            damage = func_ov093_021c0d90(flow, param, mon, targets, data, &reserved, arg6);
            showEffect = TRUE;
        }
        ServerControl_CalcRecoil(flow, mon, param->move, damage);
    }
    func_ov093_021c1a54(flow, param, mon, flow->unk860, damage, arg6);
    if (showEffect) {
        effect = flow->moveEffect;
        if (!effect->enabled) {
            effect->enabled = 1;
            effect->unk05_1 = 1;
        }
        func_ov093_021bfbd0(flow, param->move, flow->moveEffect, reserved);
    }
}








void func_ov093_021c0a4c(BtlServerFlow *flow, BattleMon *attacker, void *targets, const BtlFlowMoveParam *param,
                         void *effectiveness, u32 arg5, BtlFlowDamageList *list) {
    BattleMon *target;
    u32 i;
    u16 damage;
    u16 adjusted;

    list->count = 0;
    i = 0;
    list->substituteCount = 0;
    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        list->entries[i].monId = GetMonID(target);
        list->entries[i].critical = func_ov093_021c6bf8(flow, attacker, target, param->move);
        list->entries[i].effectiveness = func_ov093_021cc8b8(effectiveness, list->entries[i].monId);
        list->entries[i].fixedDamage = ServerEvent_CalcDamage(flow, attacker, target, param,
                                                              list->entries[i].effectiveness, arg5,
                                                              list->entries[i].critical, FALSE, &damage);
        list->entries[i].damage = damage;
        if (list->entries[i].fixedDamage) {
            list->entries[i].critical = FALSE;
            list->entries[i].effectiveness = 3;
        }
        list->entries[i].substitute = IsSubstituteActive(target);
        if (!list->entries[i].substitute) {
            adjusted = func_ov093_021c15d4(target, list->entries[i].damage);
            list->entries[i].unk9 = func_ov093_021c1678(flow, attacker, target, 1, &adjusted);
            list->entries[i].damage = adjusted;
            list->count++;
        } else {
            list->entries[i].damage = damage;
            list->entries[i].unk9 = 0;
            list->substituteCount++;
        }
        func_ov093_021c7c24(&flow->unk1F7C, flow, attacker, target, list->entries[i].effectiveness);
        i++;
        target = data_ov095_0689cc8d(targets);
    }
    list->total = i;
}













u32 func_ov093_021c0c34(BtlFlowDamageList *list) {
    return list->total;
}

u32 func_ov093_021c0c38(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons, u16 *damages,
                        u32 *effectiveness, u8 *critical) {
    u32 i;
    u32 count;

    for (i = 0, count = 0; i < list->total; i++) {
        if (list->entries[i].substitute) {
            mons[count] = GetPokeParam(flow->pokeCon, list->entries[i].monId);
            damages[count] = list->entries[i].damage;
            effectiveness[count] = list->entries[i].effectiveness;
            critical[count] = list->entries[i].critical;
            count++;
        }
    }
    return count;
}

u32 func_ov093_021c0cb4(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons) {
    u32 i;
    for (i = 0; i < list->total; i++) {
        mons[i] = GetPokeParam(flow->pokeCon, list->entries[i].monId);
    }
    return list->total;
}

u32 func_ov093_021c0ce8(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons, u16 *damages,
                        u32 *effectiveness, u8 *critical, u8 *unk9) {
    u32 i;
    u32 count;

    for (i = 0, count = 0; i < list->total; i++) {
        if (!list->entries[i].substitute) {
            mons[count] = GetPokeParam(flow->pokeCon, list->entries[i].monId);
            damages[count] = list->entries[i].damage;
            effectiveness[count] = list->entries[i].effectiveness;
            critical[count] = list->entries[i].critical;
            unk9[count] = list->entries[i].unk9;
            count++;
        }
    }
    return count;
}

u32 func_ov093_021c0d70(const BtlFlowDamageList *list) {
    u32 i;
    u32 total = 0;
    for (i = 0; i < list->total; i++) {
        total += list->entries[i].damage;
    }
    return total;
}

u32 func_ov093_021c0d90(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, void *targets, void *data,
                        u32 *reserved, u32 arg6) {
    BtlFlowDamageFlags flags;
    u32 ratio;
    u32 damage;

    if (data_ov095_0689cd41(targets) == 1) {
        ratio = 0x1000;
    } else {
        ratio = 0xc00;
    }
    damage = 0;
    flags.multipleTargets = data_ov095_0689cd39(targets) > 1;
    flags.unk1 = arg6;
    data_ov095_0689cdcd(targets, attacker, flow->unk858);
    data_ov095_0689ce59(targets, attacker, flow->unk85C);
    func_ov093_021c0a4c(flow, attacker, flow->unk858, param, data, ratio, flow->unk86C);
    func_ov093_021c0a4c(flow, attacker, flow->unk85C, param, data, ratio, flow->unk870);
    *reserved = SCQUE_RESERVE_Pos(flow->queue, 0x30);
    if (data_ov095_0689cd39(flow->unk858)) {
        damage += func_ov093_021c11a4(flow, param, attacker, flow->unk858, flow->unk86C, flow->unk4B4, ratio, flags);
        if (damage != 0 && func_ov093_021bec08(flow, attacker)) {
            func_ov093_021ba1b4(flow->mainModule, 0x4d);
        }
    }
    if (data_ov095_0689cd39(flow->unk85C)) {
        damage += func_ov093_021c11a4(flow, param, attacker, flow->unk85C, flow->unk870, flow->unk4B4, ratio, flags);
    }
    return damage;
}



















u32 func_ov093_021c0f24(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, void *targets, void *data) {
    u32 hit;
    u32 hits;
    u32 damage = 0;
    u32 status;
    u8 targetPos = 6;
    BattleMon *target = data_ov095_0689cc71(targets, 0);

    flow->unk789 = GetMonID(target);
    if (func_ov093_021b8590(flow->mainModule)) {
        targetPos = GetBattlePos(&flow->posPoke, GetMonID(target));
        if (targetPos != 6) {
            func_ov093_021cd4b0(flow->queue, 0x4e, GetBattlePos(&flow->posPoke, GetMonID(attacker)), targetPos, 0x27e);
        }
    }
    {
        BtlFlowDamageFlags flags = { 0 };
        for (hit = 0, hits = 0; hit < flow->unk4B4->count; hit++) {
            BattleMoveEffectState *effect;

            status = GetBattleMonStatus(attacker);
            effect = flow->moveEffect;
            if (!effect->enabled) {
                effect->enabled = 1;
                effect->unk05_1 = 1;
            }
            func_ov093_021cd4b0(flow->queue, 0x30, flow->moveEffect->pos1, flow->moveEffect->pos2, param->move, 0);
            func_ov093_021c0a4c(flow, attacker, targets, param, data, 0x1000, flow->unk870);
            damage += func_ov093_021c11a4(flow, param, attacker, targets, flow->unk870, flow->unk4B4, 0x1000, flags);
            hits++;
            if (IsFainted(target) || IsFainted(attacker)) {
                break;
            }
            ServerControl_CheckItemReaction(flow, target, 1);
            if (GetBattleMonStatus(attacker) == 2 && status != 2) {
                break;
            }
            if (flow->unk4B4->unk02 && !func_ov093_021bfa60(flow, attacker, target, param)) {
                break;
            }
        }
    }
    if (hits != 0) {
        func_ov093_021c15ac(flow, flow->unk4B4->unk05);
        func_ov093_021cd64c(flow->queue, 0x5a, 0x20, hits, 0xffff0000);
    }
    if (IsFainted(target)) {
        func_ov093_021cd64c(flow->queue, 0x5b, 0, GetMonID(target), 0xffff0000);
    }
    if (targetPos != 6) {
        func_ov093_021cd4b0(flow->queue, 0x4d, GetBattlePos(&flow->posPoke, GetMonID(attacker)), 0x291);
    }
    flow->unk789 = 0x1f;
    return damage;
}













BOOL func_ov093_021c1184(BtlFlowHitWork *hitWork) {
    if (hitWork->unk01 == 0) {
        return TRUE;
    }
    return FALSE;
}

u8 func_ov093_021c1194(BtlFlowHitWork *hitWork) {
    return hitWork->unk03;
}

void func_ov093_021c1198(BtlFlowHitWork *hitWork, u32 value) {
    if (hitWork->unk05 == 3) {
        hitWork->unk05 = value;
    }
}

u32 func_ov093_021c11a4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, void *monSet,
                        BtlFlowDamageList *list, BtlFlowHitWork *hitWork, u32 ratio, BtlFlowDamageFlags flags) {
    u8 total = func_ov093_021c0c34(list);
    if (total) {
        u32 damage = func_ov093_021c11f0(flow, attacker, monSet, list, param, hitWork, ratio, flags);
        func_ov093_021c18d4(flow, attacker, param, damage);
        return damage;
    }
    return 0;
}

u32 func_ov093_021c11f0(BtlServerFlow *flow, BattleMon *attacker, void *monSet, BtlFlowDamageList *list,
                        BtlFlowMoveParam *param, BtlFlowHitWork *hitWork, u32 ratio, BtlFlowDamageFlags flags) {
    int i;
    u8 hitCount;
    u32 total;
    u8 attackerPos;

    attackerPos = GetBattlePos(&flow->posPoke, GetMonID(attacker));
    total = func_ov093_021c0d70(list);
    hitCount = func_ov093_021c0c38(flow, list, data_ov093_021f3b38, data_ov093_021f3b26, data_ov093_021f3b2c,
                                   data_ov093_021f3b20);
    for (i = 0; i < hitCount; i++) {
        if (IsSubstituteActive(data_ov093_021f3b38[i])) {
            u16 dealt = func_ov093_021c4104(flow, attacker, data_ov093_021f3b38[i], data_ov093_021f3b26[i],
                                            data_ov093_021f3b2c[i], data_ov093_021f3b20[i], param);
            data_ov095_0689cbb9(flow->unk860, data_ov093_021f3b38[i], dealt, 1);
            total -= data_ov093_021f3b26[i] - dealt;
        }
    }
    if (func_ov093_021c1184(hitWork)) {
        u8 count = func_ov093_021c0cb4(flow, list, data_ov093_021f3b38);
        for (i = 0; i < count; i++) {
            func_ov093_021c15e8(flow, attacker, data_ov093_021f3b38[i], param);
        }
    }
    hitWork->unk01++;
    hitCount = func_ov093_021c0ce8(flow, list, data_ov093_021f3b38, data_ov093_021f3b26, data_ov093_021f3b2c,
                                   data_ov093_021f3b20, data_ov093_021f3b23);
    if (hitCount == 0) {
        return total;
    }
    func_ov093_021c1788(flow, param, attacker, hitCount, data_ov093_021f3b38);
    func_ov093_021c5798(flow, param, hitCount, data_ov093_021f3b2c, data_ov093_021f3b38, data_ov093_021f3b26,
                        data_ov093_021f3b20, flags.multipleTargets);
    if (!func_ov093_021c1194(hitWork)) {
        func_ov093_021c5840(flow, hitCount, data_ov093_021f3b2c, data_ov093_021f3b38, flags.multipleTargets);
    } else {
        func_ov093_021c1198(hitWork, data_ov093_021f3b2c[0]);
    }
    func_ov093_021c59c4(flow, hitCount, data_ov093_021f3b38, data_ov093_021f3b20, flags.multipleTargets);
    for (i = 0; i < hitCount; i++) {
        data_ov095_0689cbb9(flow->unk860, data_ov093_021f3b38[i], data_ov093_021f3b26[i], 0);
        func_ov093_021c1828(flow, attackerPos, attacker, data_ov093_021f3b38[i], param, data_ov093_021f3b26[i]);
        func_ov093_021d5fc0(data_ov093_021f3b38[i], 2);
    }
    for (i = 0; i < hitCount; i++) {
        if (data_ov093_021f3b23[i]) {
            func_ov093_021c16f8(flow, data_ov093_021f3b38[i], data_ov093_021f3b23[i]);
        }
    }
    for (i = 0; i < hitCount; i++) {
        func_ov093_021c1ccc(flow, param, attacker, data_ov093_021f3b38[i]);
        ServerControl_DamageDrain(flow, param, attacker, data_ov093_021f3b38[i], data_ov093_021f3b26[i]);
        func_ov093_021c1880(flow, param, attacker, data_ov093_021f3b38[i], data_ov093_021f3b26[i], 0);
        func_ov093_021c4218(flow, attacker, data_ov093_021f3b38[i], param, data_ov093_021f3b2c[i],
                            data_ov093_021f3b26[i], data_ov093_021f3b20[i], 0);
    }
    for (i = 0; i < hitCount; i++) {
        ServerControl_CheckFainted(flow, data_ov093_021f3b38[i]);
    }
    ServerControl_CheckFainted(flow, attacker);
    return total;
}










void func_ov093_021c14e0(BtlServerFlow *flow, BattleMon *attacker, BtlFlowMoveParam *param, void *targets) {
    BattleMon *target;
    u32 state;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (func_ov093_021c6b5c(flow, attacker, target, param)) {
            if (!flow->moveEffect->enabled) {
                flow->moveEffect->enabled = TRUE;
            }
            state = PushState(&flow->actionState, 0x197b);
            func_ov093_021c6bb8(flow, attacker, target);
            PopState(&flow->actionState, state, 0x197f);
            data_ov095_0689cc15(targets, target);
        }
        target = data_ov095_0689cc8d(targets);
    }
}













void func_ov093_021c15ac(BtlServerFlow *flow, s32 effectiveness) {
    if (effectiveness < 3) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x4f, 0xffff0000);
    } else if (effectiveness > 3) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x4e, 0xffff0000);
    }
}

u32 func_ov093_021c15d4(BattleMon *mon, u32 damage) {
    u32 hp = GetBattleMonStat(mon, 0xd);
    if (damage > hp) {
        damage = hp;
    }
    return damage;
}

void func_ov093_021c15e8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param) {
    u32 state = PushState(&flow->actionState, 0x19aa);
    func_ov093_021c1624(flow, attacker, target, param);
    PopState(&flow->actionState, state, 0x19ae);
}

void func_ov093_021c1624(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param) {
    BattleEventVar_Push(0x19bd);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEvent_CallHandlers(flow, 0x45);
    BattleEventVar_Pop(0x19c4);
}

u32 func_ov093_021c1678(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 arg3, u16 *damage) {
    u32 cause;
    int hp = GetBattleMonStat(target, 0xd);

    if (hp > *damage) {
        return 0;
    }
    BattleEventVar_Push(0x19df);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x51, arg3);
    BattleEventVar_SetRewriteOnceValue(0x3a, 0);
    BattleEvent_CallHandlers(flow, 0x74);
    cause = BattleEventVar_GetValue(0x3a);
    BattleEventVar_Pop(0x19e6);
    if (cause != 0) {
        *damage = GetBattleMonStat(target, 0xd) - 1;
    }
    return cause;
}

void func_ov093_021c16f8(BtlServerFlow *flow, BattleMon *mon, u32 cause) {
    u32 state;
    u8 monId = GetMonID(mon);

    switch (cause) {
    case 1:
        func_ov093_021cd64c(flow->queue, 0x5b, 0x202, monId, 0xffff0000);
        break;
    case 3:
    default:
        state = PushState(&flow->actionState, 0x1a04);
        func_ov093_021c1758(flow, mon, cause);
        PopState(&flow->actionState, state, 0x1a07);
        break;
    }
}

void func_ov093_021c1758(BtlServerFlow *flow, BattleMon *mon, u32 cause) {
    BattleEventVar_Push(0x1a17);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x75);
    BattleEventVar_Pop(0x1a1a);
}

void func_ov093_021c1788(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, u8 count,
                         BattleMon **mons) {
    u32 state = PushState(&flow->actionState, 0x1a29);
    func_ov093_021c17cc(flow, param, attacker, count, mons);
    PopState(&flow->actionState, state, 0x1a2e);
}

void func_ov093_021c17cc(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, u8 count,
                         BattleMon **mons) {
    u32 i;

    BattleEventVar_Push(0x1a3f);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(5, count);
    for (i = 0; i < count; i++) {
        BattleEventVar_SetConstValue(6 + i, GetMonID(mons[i]));
    }
    BattleEvent_CallHandlers(flow, 0x44);
    BattleEventVar_Pop(0x1a47);
}

void func_ov093_021c1828(BtlServerFlow *flow, u8 attackerPos, BattleMon *attacker, BattleMon *target,
                         BtlFlowMoveParam *param, u16 damage) {
    u8 attackerId = GetMonID(attacker);
    u8 targetId = GetMonID(target);
    BattleMonDamageRecord record;

    BattleMonDamageRecord_Init(&record, attackerId, attackerPos, param->move, param->type, damage);
    func_ov093_021d6848(target, &record);
    func_ov093_021cd4b0(flow->queue, 0x2d, targetId, attackerId, attackerPos, param->type, param->move, damage);
}

void func_ov093_021c1880(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                         u16 damage, BOOL flag) {
    switch (PML_MoveGetQuality(param->move)) {
    case 7:
        ServerEvent_DamageAddEffect(flow, param, attacker, attacker);
        break;
    case 6:
        if (!flag) {
            ServerEvent_DamageAddEffect(flow, param, attacker, target);
        }
        break;
    case 4:
        if (!flag) {
            ServerControl_DamageAddCondition(flow, param, attacker, target);
        }
        break;
    }
}

void func_ov093_021c18d4(BtlServerFlow *flow, BattleMon *attacker, BtlFlowMoveParam *param, u32 damage) {
    u32 state = PushState(&flow->actionState, 0x1a92);
    func_ov093_021c1910(flow, attacker, param, damage);
    PopState(&flow->actionState, state, 0x1a95);
}

void func_ov093_021c1910(BtlServerFlow *flow, BattleMon *attacker, BtlFlowMoveParam *param, u32 damage) {
    BattleEventVar_Push(0x1aa7);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x32, damage);
    BattleEvent_CallHandlers(flow, 0x4d);
    BattleEventVar_Pop(0x1aad);
}

void ServerControl_CheckItemReaction(BtlServerFlow *flow, BattleMon *mon, u32 reaction) {
    if (GetBattleMonHeldItem(mon) != 0) {
        u32 state = PushState(&flow->actionState, 0x1abc);
        ServerEvent_CheckItemReaction(flow, mon, reaction);
        PopState(&flow->actionState, state, 0x1ac2);
    }
}

void ServerEvent_CheckItemReaction(BtlServerFlow *flow, BattleMon *mon, u32 reaction) {
    u8 monId;

    BattleEventVar_Push(0x1acf);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x2e, reaction);
    BattleEvent_CallHandlers(flow, 0x91);
    BattleEventVar_Pop(0x1ad3);
}

void func_ov093_021c19d8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param) {
    u32 state = PushState(&flow->actionState, 0x1ada);
    func_ov093_021c1a10(flow, mon, param);
    PopState(&flow->actionState, state, 0x1adc);
}

void func_ov093_021c1a10(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param) {
    BattleEventVar_Push(0x1aea);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEvent_CallHandlers(flow, 0x81);
    BattleEventVar_Pop(0x1af1);
}

void func_ov093_021c1a54(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *monSet, u32 damage,
                         u32 arg5) {
    u32 state;
    BattleMon *target;

    func_ov093_021c1ce4(flow, param, mon, monSet);
    state = PushState(&flow->actionState, 0x1b07);
    func_ov093_021c1b5c(flow, mon, monSet, param, arg5, FALSE, 0x84);
    func_ov093_021c1b5c(flow, mon, monSet, param, arg5, TRUE, 0x83);
    func_ov093_021c1b5c(flow, mon, monSet, param, arg5, FALSE, 0x85);
    data_ov095_0689cc85(monSet);
    for (target = data_ov095_0689cc8d(monSet); target != NULL; target = data_ov095_0689cc8d(monSet)) {
        ServerControl_CheckItemReaction(flow, target, 1);
    }
    func_ov093_021c1b5c(flow, mon, monSet, param, arg5, FALSE, 0x86);
    func_ov093_021c1b5c(flow, mon, monSet, param, arg5, TRUE, 0x87);
    func_ov093_021c1c88(flow, mon, monSet, param, arg5);
    PopState(&flow->actionState, state, 0x1b1b);
}









void func_ov093_021c1b5c(BtlServerFlow *flow, BattleMon *mon, void *monSet, BtlFlowMoveParam *param, u32 arg4,
                         BOOL flag, u32 event) {
    u32 value;
    u32 total;
    u32 count;
    u32 i;
    u32 hits;
    BattleMon *target;

    count = data_ov095_0689cd39(monSet);
    total = 0;
    hits = 0;
    BattleEventVar_Push(0x1b2b);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x4d, arg4);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    for (i = 0; i < count; i++) {
        BOOL found;
        target = data_ov095_0689cc71(monSet, i);
        if (flag) {
            found = data_ov095_0689ccf9(monSet, target, &value);
        } else {
            found = data_ov095_0689ccb5(monSet, target, &value);
        }
        if (found) {
            total += value;
            BattleEventVar_SetConstValue(6 + hits, GetMonID(target));
            hits++;
        }
    }
    if (hits != 0) {
        BattleEventVar_SetConstValue(5, hits);
        BattleEventVar_SetRewriteOnceValue(0x47, 0);
        BattleEventVar_SetRewriteOnceValue(0x48, 0);
        BattleEventVar_SetConstValue(0x32, total);
        BattleEvent_CallHandlers(flow, 0x82);
        if (!BattleEventVar_GetValue(0x48)) {
            BattleEvent_CallHandlers(flow, event);
        }
    }
    BattleEventVar_Pop(0x1b52);
}














void func_ov093_021c1c88(BtlServerFlow *flow, BattleMon *mon, void *monSet, BtlFlowMoveParam *param, u32 arg4) {
    BattleEventVar_Push(0x1b61);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x4d, arg4);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEvent_CallHandlers(flow, 0x88);
    BattleEventVar_Pop(0x1b66);
}

void func_ov093_021c1ccc(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target) {
    u32 chance = func_ov093_021c7664(flow, param->move, attacker);
    ServerControl_FlinchCore(flow, target, chance);
}


void func_ov093_021c1ce4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *monSet) {
    BattleMon *target;

    if (param->type == 9) {
        data_ov095_0689cc85(monSet);
        target = data_ov095_0689cc8d(monSet);
        while (target != NULL) {
            if (GetBattleMonStatus(target) == 3 && !IsSubstituteActive(target)) {
                func_ov093_021c557c(flow, target, 3, TRUE);
            }
            target = data_ov095_0689cc8d(monSet);
        }
    }
}






BOOL ServerControl_FlinchCore(BtlServerFlow *flow, BattleMon *mon, u32 chance) {
    if (GetTurnFlag(mon, 5)) {
        func_ov093_021d5fc0(mon, 6);
        return TRUE;
    }
    if (func_ov093_021c76f4(flow, mon, chance)) {
        func_ov093_021d5fc0(mon, 4);
        return TRUE;
    }
    if (chance >= 100) {
        u32 state = PushState(&flow->actionState, 0x1bad);
        func_ov093_021c7780(flow, mon);
        PopState(&flow->actionState, state, 0x1baf);
    }
    return FALSE;
}





void ServerControl_DamageDrain(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                               u32 damage) {
    if (PML_MoveGetQuality(param->move) == 8) {
        if (ServerControl_DrainCore(flow, attacker, target,
                                    MultiplyValueByRatio(damage, PML_MoveGetParam(param->move, 0x19)))) {
            ServerDisplay_SkyDropTargetAppear(flow, target, 0x383);
        }
    }
}

BOOL ServerControl_DrainCore(BtlServerFlow *flow, BattleMon *mon, BattleMon *source, u16 amount) {
    u32 state;
    state = PushState(&flow->actionState, 0x1bf2);
    BOOL result = FALSE;
    amount = func_ov093_021c78f4(flow, mon, source, amount);
    if (amount != 0 && !ServerControl_RecoverHPCheckFail(flow, mon)) {
        result = ServerControl_RecoverHP(flow, mon, amount, TRUE);
    }
    PopState(&flow->actionState, state, 0x1c00);
    return result;
}

BOOL ServerEvent_CalcDamage(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender,
                            const BtlFlowMoveParam *param, u32 effectiveness, u32 ratio, BOOL critical,
                            BOOL fixedRoll, u16 *damage) {
    u32 power;
    u32 attack;
    u32 defense;
    u32 level;
    u32 value;
    u32 category;
    u32 mod;
    BOOL fixed;
    u32 result;

    category = PML_MoveGetCategory(param->move);
    fixed = FALSE;
    BattleEventVar_Push(0x1c21);
    BattleEventVar_SetConstValue(0x38, effectiveness);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(defender));
    BattleEventVar_SetConstValue(0x45, critical);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x1a, category);
    BattleEventVar_SetValue(0x37, 0);
    BattleEvent_CallHandlers(flow, 0x46);
    result = BattleEventVar_GetValue(0x37);
    if (result != 0) {
        fixed = TRUE;
    } else {
        power = func_ov093_021c728c(flow, attacker, defender, param);
        attack = func_ov093_021c7374(flow, attacker, defender, param, critical);
        defense = func_ov093_021c7474(flow, attacker, defender, param, critical);
        level = (u8)GetBattleMonStat(attacker, 0xf);
        value = CalcBaseDamage(power, attack, level, defense);
        if (ratio != 0x1000) {
            value = fixed_round(value, ratio);
        }
        mod = WeatherPowerMod(ServerEvent_GetWeather(flow), param->type);
        if (mod != 0x1000) {
            value = fixed_round(value, mod);
        }
        if (critical) {
            value *= 2;
        }
        if (!ReturnZero(flow->mainModule, 7)) {
            u32 roll;
            if (fixedRoll) {
                roll = 0x55;
            } else {
                roll = (u16)(100 - BattleRandom(16));
            }
            value = value * roll / 100;
        }
        if (param->type != 0x11) {
            value = fixed_round(value, func_ov093_021c75ec(flow, attacker, param->type));
        }
        value = TypeEffectivenessPowerMod(value, effectiveness);
        if (category == 1 && GetBattleMonStatus(attacker) == 4 && GetBattleMonStat(attacker, 0x11) != 0x3e) {
            value = value * 50 / 100;
        }
        if (value == 0) {
            value = 1;
        }
        BattleEventVar_SetMulValue(0x35, 0x1000, 0x29, 0x20000);
        BattleEventVar_SetValue(0x32, value);
        BattleEvent_CallHandlers(flow, 0x47);
        mod = BattleEventVar_GetValue(0x35);
        result = fixed_round(BattleEventVar_GetValue(0x32), mod);
    }
    BattleEvent_CallHandlers(flow, 0x48);
    BattleEventVar_Pop(0x1c8d);
    *damage = result;
    return fixed;
}



void ServerControl_CalcRecoil(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 damage) {
    BOOL forced;

    if (!IsFainted(mon)) {
        s32 recoil = ServerEvent_CalcRecoil(flow, mon, move, damage, &forced);
        if (recoil != 0) {
            BattleHandler_StrSetup(&flow->message, 2, 0x17a);
            BattleHandler_AddArg(&flow->message, GetMonID(mon));
            if (forced || ServerControl_CheckSimpleDamageEnabled(flow, mon, recoil)) {
                ServerControl_SimpleDamageCore(flow, mon, recoil, &flow->message);
            }
        }
    }
}

BOOL ServerControl_CheckSimpleDamageEnabled(BtlServerFlow *flow, BattleMon *mon, u32 damage) {
    if (func_ov093_021c7330(flow, mon)) {
        return TRUE;
    }
    return FALSE;
}


BOOL ServerControl_SimpleDamageCore(BtlServerFlow *flow, BattleMon *mon, u32 damage, BattleHandlerString *string) {
    s32 change = -damage;
    if (change != 0) {
        ServerDisplay_SimpleHP(flow, mon, change, TRUE);
        func_ov093_021d5fc0(mon, 2);
        if (string != NULL) {
            BattleHandler_SetString(flow, string);
            BattleHandler_StrClear(string);
        }
        ServerControl_CheckItemReaction(flow, mon, 1);
        if (ServerControl_CheckFainted(flow, mon)) {
            ServerControl_CheckMatchup(flow);
            return TRUE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL ServerControl_UseHeldItem(BtlServerFlow *flow, BattleMon *mon) {
    u32 item = GetBattleMonHeldItem(mon);
    BOOL result = FALSE;

    if (DoesBattleMonExist(&flow->posPoke, GetMonID(mon))) {
        u32 state = PushState(&flow->actionState, 0x1cea);
        if (!ServerEvent_CheckHeldItemFail(flow, mon, item)) {
            result = TRUE;
        }
        if (result) {
            u32 itemState;
            u32 reserved;

            reserved = SCQUE_RESERVE_Pos(flow->queue, 0x42);
            itemState = PushStateUseItem(&flow->actionState, item, 0x1cf4);
            ServerEvent_EquipItem(flow, mon);
            if (BattleHandler_Result(flow) != 2) {
                result = FALSE;
            }
            PopState(&flow->actionState, itemState, 0x1cf9);
            if (result) {
                func_ov093_021cd568(flow->queue, reserved, 0x42, GetMonID(mon));
                if (ItemGetParam(item, 0x10)) {
                    ServerControl_ChangeHeldItem(flow, mon, 0, TRUE);
                }
            }
        }
        PopState(&flow->actionState, state, 0x1d06);
    }
    return result;
}




BOOL ServerEvent_CheckHeldItemFail(BtlServerFlow *flow, BattleMon *mon, u16 item) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x1d19);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x2d, item);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEvent_CallHandlers(flow, 0x90);
    value = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x1d1f);
    return value;
}

void ServerControl_ChangeHeldItem(BtlServerFlow *flow, BattleMon *mon, u16 item, BOOL consume) {
    u32 state;
    u16 oldItem;
    u8 monId;

    monId = GetMonID(mon);
    oldItem = GetBattleMonHeldItem(mon);
    state = PushState(&flow->actionState, 0x1d2d);
    func_ov093_021c7ad0(flow, mon, item);
    PopState(&flow->actionState, state, 0x1d2f);
    if (item == 0) {
        scPut_SetContFlag(flow, mon, 0xf);
    }
    ItemEvent_RemoveItem(mon);
    func_ov093_021cd4b0(flow->queue, 0x1e, monId, item);
    SetItem(mon, item);
    if (item != 0) {
        ItemEvent_AddItem(mon);
    }
    state = PushState(&flow->actionState, 0x1d3d);
    func_ov093_021c7b0c(flow, mon);
    PopState(&flow->actionState, state, 0x1d3f);
    if (consume) {
        ConsumeItem(mon, oldItem);
        func_ov093_021cd4b0(flow->queue, 0x17, monId, oldItem);
        ServerDisplay_SetTurnFlag(flow, mon, 8);
    }
}


void ServerControl_FaintPokemon(BtlServerFlow *flow, BattleMon *mon) {
    ServerDisplay_FaintPokemon(flow, mon, 0);
    ServerControl_CheckFainted(flow, mon);
}

void ServerControl_DamageAddCondition(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *attacker,
                                      BattleMon *target) {
    BattleCondition value;
    u32 condition = ServerEvent_CheckMoveAddCondition(flow, param->move, attacker, target, &value);
    if (condition != 0 && !IsFainted(target)) {
        ServerControl_MoveConditionCore(flow, attacker, target, param->move, condition, value, FALSE);
    }
}

u32 ServerEvent_CheckMoveAddCondition(BtlServerFlow *flow, u16 move, BattleMon *attacker, BattleMon *target,
                                      BattleCondition *value) {
    u32 condition;
    MoveConditionParam param;
    BattleCondition result;
    u8 chance;
    u8 failed;

    condition = PML_MoveGetParam(move, 0xb);
    param = func_0201bf58(move);
    chance = PML_MoveGetParam(move, 0xc);
    func_ov093_021d7c70(param, attacker, &result);
    BattleEventVar_Push(0x1d7c);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetValue(0x1d, condition);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEventVar_SetRewriteOnceValue(0x1e, result.raw);
    BattleEventVar_SetValue(0x26, chance);
    BattleEvent_CallHandlers(flow, 0x64);
    condition = BattleEventVar_GetValue(0x1d);
    chance = BattleEventVar_GetValue(0x26);
    failed = BattleEventVar_GetValue(0x41);
    if (condition == 0xffff) {
        condition = 0;
    }
    result.raw = BattleEventVar_GetValue(0x1e);
    BattleEventVar_Pop(0x1d8f);
    if (!failed && condition != 0) {
        BOOL hit = BattleRandom(100) < chance ? TRUE : FALSE;
        if (hit) {
            *value = result;
            return condition;
        }
        if (ReturnZero(flow->mainModule, 0)) {
            *value = result;
            return condition;
        }
    }
    return 0;
}

void ServerControl_SimpleCondition(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets) {
    u32 condition;
    MoveConditionParam param;
    BOOL success;
    BattleMon *target;

    condition = PML_MoveGetParam(move, 0xb);
    param = func_0201bf58(move);
    success = FALSE;
    if (data_ov095_0689cd39(targets)) {
        data_ov095_0689cc85(targets);
        for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
            BattleCondition value;
            func_ov093_021d7c70(param, mon, &value);
            if (ServerControl_MoveConditionCore(flow, mon, target, move, condition, value, TRUE)) {
                BattleMoveEffectState *effect = flow->moveEffect;
                if (!effect->enabled) {
                    effect->enabled = 1;
                }
                success = TRUE;
            }
        }
        if (!success && !flow->unk78A_4) {
            func_ov093_021c5718(flow, mon, move);
        }
    } else {
        func_ov093_021c5718(flow, mon, move);
    }
}













BOOL ServerControl_MoveConditionCore(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move,
                                     u32 condition, BattleCondition value, BOOL flag) {
    BOOL defaultMessage;

    BattleHandler_StrClear(&flow->message);
    if (condition == 0xffff) {
        condition = ServerEvent_DecideSpecialMoveCondition(flow, attacker, target, &flow->message);
        if (condition == 0 || condition == 0xffff) {
            return FALSE;
        }
    } else {
        ServerEvent_AddMoveConditionString(flow, condition, attacker, target, &flow->message);
    }
    ServerEvent_MoveConditionContinue(flow, attacker, target, condition, &value);
    defaultMessage = BattleHandler_StrIsEnabled(&flow->message) ? FALSE : TRUE;
    if (ServerEvent_AddCondition(flow, target, attacker, condition, value, flag, defaultMessage)) {
        if (!defaultMessage) {
            BattleHandler_SetString(flow, &flow->message);
            BattleHandler_StrClear(&flow->message);
        }
        return TRUE;
    }
    return FALSE;
}

u32 ServerEvent_DecideSpecialMoveCondition(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target,
                                           BattleHandlerString *string) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x1e01);
    monId = GetMonID(attacker);
    BattleEventVar_SetConstValue(3, monId);
    monId = GetMonID(target);
    BattleEventVar_SetConstValue(4, monId);
    BattleEventVar_SetConstValue(0x3f, (intptr_t)string);
    BattleEventVar_SetValue(0x1d, 0);
    BattleEvent_CallHandlers(flow, 0x60);
    value = BattleEventVar_GetValue(0x1d);
    BattleEventVar_Pop(0x1e08);
    return value;
}

void ServerEvent_AddMoveConditionString(BtlServerFlow *flow, u32 condition, BattleMon *attacker, BattleMon *target,
                                        BattleHandlerString *string) {
    u8 monId;

    BattleEventVar_Push(0x1e18);
    monId = GetMonID(attacker);
    BattleEventVar_SetConstValue(3, monId);
    monId = GetMonID(target);
    BattleEventVar_SetConstValue(4, monId);
    BattleEventVar_SetConstValue(0x3f, (intptr_t)string);
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEvent_CallHandlers(flow, 0x61);
    BattleEventVar_Pop(0x1e1e);
}

void ServerEvent_MoveConditionContinue(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 condition,
                                       BattleCondition *value) {
    BattleEventVar_Push(0x1e2e);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEventVar_SetValue(0x1e, value->raw);
    BattleEvent_CallHandlers(flow, 0x62);
    value->raw = BattleEventVar_GetValue(0x1e);
    BattleEventVar_Pop(0x1e35);
}

BOOL ServerEvent_AddCondition(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                              BattleCondition value, BOOL flag, BOOL defaultMessage) {
    if (!ServerControl_AddConditionCheckFail(flow, target, attacker, condition, value, 0, flag)) {
        ServerControl_AddCondition(flow, target, attacker, condition, value, defaultMessage, FALSE, NULL);
        return TRUE;
    }
    return FALSE;
}

BOOL ServerControl_AddConditionCheckFail(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                         BattleCondition value, u8 overwrite, BOOL showFail) {
    u32 state;
    BOOL failed;
    u32 cause = AddConditionCheckFailOverwrite(flow, target, condition, value, overwrite);

    if (cause != 0) {
        if (showFail) {
            AddConditionCheckFailStandard(flow, target, cause, condition);
        }
        return TRUE;
    }
    state = PushState(&flow->actionState, 0x1e63);
    failed = ServerEvent_MoveConditionCheckFail(flow, attacker, target, condition);
    if (failed && showFail) {
        ServerEvent_AddConditionFailed(flow, target, attacker, condition);
        flow->unk78A_4 = 1;
    }
    PopState(&flow->actionState, state, 0x1e70);
    return failed;
}

u32 AddConditionCheckFailOverwrite(BtlServerFlow *flow, BattleMon *mon, s32 condition, BattleCondition value,
                                   u8 overwrite) {
    if (CheckCondition(mon, condition) && overwrite != 2) {
        return 1;
    }
    if (condition < 6 && GetBattleMonStatus(mon) != 0 && overwrite == 0) {
        return 3;
    }
    if (ServerEvent_GetWeather(flow) == 1 && condition == 3) {
        return 3;
    }
    if (condition == 5) {
        PokeTypePair type = GetPokeType(mon);
        if (func_ov093_021e8d24(type, 8) || func_ov093_021e8d24(type, 3)) {
            return 2;
        }
    }
    if (condition == 4 && func_ov093_021e8d24(GetPokeType(mon), 9)) {
        return 2;
    }
    if (condition == 3 && func_ov093_021e8d24(GetPokeType(mon), 0xe)) {
        return 2;
    }
    if (condition == 0x12 && func_ov093_021e8d24(GetPokeType(mon), 0xb)) {
        return 2;
    }
    if (condition == 0xe && GetBattleMonStatus(mon) != 0) {
        return 3;
    }
    if (condition == 0x10 && GetBattleMonStat(mon, 0x10) == 0x79) {
        return 3;
    }
    return 0;
}

void ServerControl_AddCondition(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                BattleCondition value, BOOL showMessage, BOOL skipItemReaction,
                                const BattleHandlerString *string) {
    u32 state;

    ServerDisplay_AddCondition(flow, target, condition, value);
    switch (condition) {
    case 5:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x257);
        break;
    case 4:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x258);
        break;
    case 1:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x25a);
        break;
    case 3:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x259);
        break;
    case 2:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x256);
        break;
    case 6:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x25b);
        break;
    case 7:
        ServerDisplay_AddEffectAtPosition(flow, target, 0x25c);
        break;
    }
    if (showMessage) {
        data_ov095_0689c365(condition, value, target, &flow->message);
        BattleHandler_SetString(flow, &flow->message);
        BattleHandler_StrClear(&flow->message);
    } else if (string != NULL) {
        BattleHandler_SetString(flow, string);
    }
    if (condition == 3 && GetBattleMonSpecies(target) == 0x1ec && GetBattleMonStat(target, 0x13) == 1) {
        AbilityEvent_RemoveItem(target);
        ChangeForm(target, 0);
        func_ov093_021cd4b0(flow->queue, 0x4f, GetMonID(target), 0);
        AbilityEvent_AddItem(target);
        ServerDisplay_SkyDropTargetAppear(flow, target, 0xde);
    }
    state = PushState(&flow->actionState, 0x1f00);
    if (IsBasicStatus(condition)) {
        ServerEvent_ConditionConfirmed(flow, target, attacker, condition, value);
    } else if (condition == 0x10) {
        ServerEvent_GastroAcidConfirmed(flow, target);
        if (GetBattleMonStat(target, 0x11) == 0x7f) {
            ServerControl_UnnerveAction(flow, target);
        }
    } else {
        ServerEvent_MoveStatusConfirmed(flow, target, attacker, condition);
    }
    PopState(&flow->actionState, state, 0x1f0f);
    if (!skipItemReaction) {
        ServerControl_CheckItemReaction(flow, target, 3);
    }
}

u32 ServerEvent_GetWeather(BtlServerFlow *flow) {
    u8 weather;
    u32 result;
    u32 failed;

    BattleEventVar_Push(0x1f23);
    result = 0;
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x7a);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x1f27);
    if (failed == 0) {
        weather = func_ov093_021ef884();
        result = weather;
    }
    return result;
}




fx32 ServerEvent_GetWeightRatio(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId;
    u32 value;

    BattleEventVar_Push(0x1f3d);
    monId = GetMonID(mon);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEvent_CallHandlers(flow, 0x7b);
    value = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x1f42);
    return value;
}

BOOL ServerEvent_MoveConditionCheckFail(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 condition) {
    BOOL failed;
    BattleEventVar_Push(0x1f55);
    BattleEventVar_SetConstValue(3, attacker != NULL ? GetMonID(attacker) : 0x1f);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEvent_CallHandlers(flow, 0x65);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x1f5c);
    return failed;
}

void ServerEvent_AddConditionFailed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition) {
    u8 monId;

    BattleEventVar_Push(0x1f6a);
    monId = GetMonID(target);
    BattleEventVar_SetValue(4, monId);
    monId = GetMonID(attacker);
    BattleEventVar_SetValue(3, monId);
    BattleEventVar_SetValue(0x1d, condition);
    BattleEvent_CallHandlers(flow, 0x67);
    BattleEventVar_Pop(0x1f6f);
}

void ServerEvent_ConditionConfirmed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                    BattleCondition value) {
    BattleEventVar_Push(0x1f7e);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(3, (u8)(attacker != NULL ? GetMonID(attacker) : 0x1f));
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEventVar_SetConstValue(0x1e, value.raw);
    BattleEvent_CallHandlers(flow, 0x68);
    BattleEventVar_Pop(0x1f87);
}

void ServerEvent_MoveStatusConfirmed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition) {
    BattleEventVar_Push(0x1f95);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(3, (u8)(attacker != NULL ? GetMonID(attacker) : 0x1f));
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEvent_CallHandlers(flow, 0x69);
    BattleEventVar_Pop(0x1f9d);
}

void ServerEvent_GastroAcidConfirmed(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x1fa9);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_ForceCallHandlers(flow, 0x6a);
    BattleEventVar_Pop(0x1fac);
}

void ServerEvent_DamageAddEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target) {
    if (!IsFainted(target) && ServerEvent_RollStatDropEffectChance(flow, param, attacker, target)) {
        func_ov093_021c2e64(flow, param, attacker, target, 0);
    }
}

BOOL ServerEvent_RollStatDropEffectChance(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker,
                                          BattleMon *target) {
    BOOL hit;
    u8 failed;
    u8 chance = PML_MoveGetParam(param->move, 0x12);

    BattleEventVar_Push(0x1fce);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    hit = FALSE;
    BattleEventVar_SetValue(0x26, chance);
    BattleEvent_CallHandlers(flow, 0x51);
    failed = BattleEventVar_GetValue(0x41);
    chance = BattleEventVar_GetValue(0x26);
    BattleEventVar_Pop(0x1fd8);
    if (!failed) {
        if (BattleRandom(100) < chance) {
            hit = TRUE;
        }
        if (hit) {
            return TRUE;
        }
        if (ReturnZero(flow->mainModule, 0)) {
            return TRUE;
        }
    }
    return FALSE;
}

void ServerControl_SimpleEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, void *targets) {
    BattleMon *target;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (func_ov093_021c2e64(flow, param, attacker, target, TRUE)) {
            if (!flow->moveEffect->enabled) {
                flow->moveEffect->enabled = TRUE;
            }
        }
        target = data_ov095_0689cc8d(targets);
    }
}


#pragma thumb on










u32 func_ov093_021c2e4c(BtlServerFlow *flow) {
    flow->unk778++;
    if (flow->unk778 == 0) {
        flow->unk778 = 1;
    }
    return flow->unk778;
}

BOOL func_ov093_021c2e64(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                         BOOL showFail) {
    MoveStatChangeStat stat;
    s32 change;
    BOOL result;
    u32 count;
    u32 i;
    u32 serial;
    BOOL changed;
    u8 attackerId;

    result = FALSE;
    attackerId = GetMonID(attacker);
    serial = func_ov093_021c2e4c(flow);
    count = PML_MoveGetStatChangeStat(param->move);
    for (i = 0; i < count; i++) {
        changed = FALSE;
        ServerEvent_GetMoveStatChangeValue(flow, param->move, i, attacker, target, &stat, &change);
        if (stat != MOVE_STAT_CHANGE_NONE) {
            if (stat != MOVE_STAT_CHANGE_ALL) {
                changed = func_ov093_021c3008(flow, attackerId, target, stat, change, attackerId, changed, serial,
                                              showFail, TRUE);
            } else {
                u8 s;
                for (s = 1; s < 6; s++) {
                    if (func_ov093_021c3008(flow, attackerId, target, s, change, attackerId, 0, serial, showFail,
                                            TRUE)) {
                        changed = TRUE;
                    }
                }
            }
            if (changed) {
                u32 state = PushState(&flow->actionState, 0x2037);
                func_ov093_021c78a8(flow, target, param->move, stat, change);
                PopState(&flow->actionState, state, 0x2039);
                result = TRUE;
            }
        }
    }
    return result;
}


void ServerEvent_GetMoveStatChangeValue(BtlServerFlow *flow, u16 move, u32 index, BattleMon *attacker,
                                        BattleMon *target, MoveStatChangeStat *stat, s32 *change) {
    u8 multiplier;

    *stat = PML_MoveGetStatChangeStage(move, index, change);
    BattleEventVar_Push(0x2053);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetValue(0x1f, *stat);
    BattleEventVar_SetValue(0x20, *change);
    BattleEventVar_SetValue(0x35, 1);
    BattleEvent_CallHandlers(flow, 0x59);
    *stat = BattleEventVar_GetValue(0x1f);
    *change = BattleEventVar_GetValue(0x20);
    multiplier = BattleEventVar_GetValue(0x35);
    if (multiplier > 1) {
        *change *= multiplier;
    }
    BattleEventVar_Pop(0x2062);
    if (*stat == 8) {
        *stat = 0;
    }
}

BOOL func_ov093_021c3008(BtlServerFlow *flow, u8 monId, BattleMon *mon, MoveStatChangeStat stat, s32 change,
                         u8 attackerId, u16 context, u32 value, BOOL showFail, BOOL flag) {
    BOOL result;

    change = ServerEvent_CheckSubstituteInteraction(flow, mon, stat, attackerId, context, change);
    if (!IsStatChangeValid(mon, stat, change)) {
        if (showFail) {
            func_ov093_021c5a4c(flow, mon, stat, change);
            flow->unk78A_4 = 1;
        }
        return FALSE;
    }
    if (IsSubstituteActive(mon)) {
        u8 targetId = GetMonID(mon);
        if (monId != targetId) {
            if (showFail) {
                func_ov093_021cd64c(flow->queue, 0x5b, 0xd2, targetId, 0xffff0000);
            }
            return FALSE;
        }
    }
    result = TRUE;
    if (func_ov093_021c77b0(flow, mon, stat, monId, change, value)) {
        u32 state;
        func_ov093_021c5a8c(flow, mon, stat, change, context, flag);
        state = PushState(&flow->actionState, 0x209d);
        func_ov093_021c785c(flow, monId, mon, stat, change);
        PopState(&flow->actionState, state, 0x209f);
    } else {
        if (showFail) {
            u32 state = PushState(&flow->actionState, 0x20a6);
            func_ov093_021c7820(flow, mon, value);
            PopState(&flow->actionState, state, 0x20a8);
        }
        result = FALSE;
    }
    return result;
}


s32 ServerEvent_CheckSubstituteInteraction(BtlServerFlow *flow, BattleMon *mon, u32 stat, u8 attackerId, u16 context,
                                           s32 change) {
    BattleEventVar_Push(0x20c1);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(3, attackerId);
    BattleEventVar_SetConstValue(0x1f, stat);
    BattleEventVar_SetRewriteOnceValue(0x20, change);
    BattleEvent_CallHandlers(flow, 0x5a);
    change = BattleEventVar_GetValue(0x20);
    BattleEventVar_Pop(0x20c8);
    return change;
}

void func_ov093_021c3184(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    u32 condition;
    MoveConditionParam conditionParam;
    BattleCondition value;
    BattleMon *target;

    condition = PML_MoveGetParam(param->move, 0xb);
    conditionParam = func_0201bf58(param->move);
    func_ov093_021d7c70(conditionParam, mon, &value);
    data_ov095_0689cc85(targets);
    for (target = data_ov095_0689cc8d(targets); target != NULL; target = data_ov095_0689cc8d(targets)) {
        BattleMoveEffectState *effect;

        // 021a6914 only reads param, but it matches only with it non-const, and this function only with it const
        if (func_ov093_021c2e64(flow, (BtlFlowMoveParam *)param, mon, target, TRUE)) {
            effect = flow->moveEffect;
            if (!effect->enabled) {
                effect->enabled = 1;
            }
        }
        if (ServerControl_MoveConditionCore(flow, mon, target, param->move, condition, value, TRUE)) {
            effect = flow->moveEffect;
            if (!effect->enabled) {
                effect->enabled = 1;
            }
        }
    }
}







void func_ov093_021c3274(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets) {
    BattleMon *target;
    u8 targetId;
    BattleMoveEffectState *effect;

    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        targetId = GetMonID(target);
        if (!ServerControl_RecoverHPCheckFail(flow, target)) {
            if (ServerControl_RecoverHP(flow, target, func_ov093_021c79e0(flow, move, target), TRUE)) {
                effect = flow->moveEffect;
                if (!effect->enabled) {
                    effect->enabled = 1;
                }
                func_ov093_021cd64c(flow->queue, 0x5b, 0x183, targetId, 0xffff0000);
            }
        } else if (IsMonFullHP(target)) {
            func_ov093_021cd64c(flow->queue, 0x5b, 0x37d, targetId, 0xffff0000);
        }
        target = data_ov095_0689cc8d(targets);
    }
}












BOOL ServerControl_RecoverHP(BtlServerFlow *flow, BattleMon *mon, u16 amount, BOOL flag) {
    if (!ServerControl_RecoverHPCheckFailSpecial(flow, mon, flag)) {
        ServerControl_RecoverHPCore(flow, mon, amount);
        return TRUE;
    }
    return FALSE;
}

BOOL ServerControl_RecoverHPCheckFail(BtlServerFlow *flow, BattleMon *mon) {
    if (!CanPokemonBattle(mon)) {
        return TRUE;
    }
    if (IsMonFullHP(mon)) {
        return TRUE;
    }
    return FALSE;
}

BOOL ServerControl_RecoverHPCheckFailSpecial(BtlServerFlow *flow, BattleMon *mon, BOOL showMessage) {
    if (CheckCondition(mon, 0xf)) {
        if (showMessage) {
            func_ov093_021cd64c(flow->queue, 0x5b, 0x377, GetMonID(mon), 0xffff0000);
            flow->unk78A_5 = 1;
        }
        return TRUE;
    }
    return FALSE;
}

void ServerControl_RecoverHPCore(BtlServerFlow *flow, BattleMon *mon, u16 amount) {
    u8 pos = GetBattlePos(&flow->posPoke, GetMonID(mon));
    if (pos != 6) {
        func_ov093_021cd4b0(flow->queue, 0x4d, pos, 0x263);
    }
    ServerDisplay_SimpleHP(flow, mon, amount, TRUE);
}




void ServerControl_OHKO(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    u32 i = 0;
    u8 attackerLevel = GetBattleMonStat(mon, 0xf);
    BattleMon *target;

    while (TRUE) {
        u8 targetId;
        u32 effectiveness;
        u32 state;

        target = data_ov095_0689cc71(targets, i++);
        if (target == NULL) {
            break;
        }
        if (IsFainted(target)) {
            continue;
        }
        targetId = GetMonID(target);
        if (!IsGuaranteedHit(flow, mon, target) && func_ov093_021c6948(flow, mon, target, param->move)) {
            func_ov093_021c572c(flow, target, param->move);
            continue;
        }
        if (func_ov093_021bfa00(flow, mon, target, param->move)) {
            func_ov093_021c572c(flow, target, param->move);
            continue;
        }
        if (attackerLevel < (u8)GetBattleMonStat(target, 0xf)) {
            func_ov093_021c5774(flow, target);
            continue;
        }
        effectiveness = func_ov093_021cc8b8(flow->unk1F88, targetId);
        if (effectiveness == 0) {
            func_ov093_021c5774(flow, target);
            continue;
        }
        state = PushState(&flow->actionState, 0x216b);
        if (func_ov093_021c6a9c(flow, mon, target, param->move)) {
            u8 attackerPos = GetBattlePos(&flow->posPoke, GetMonID(mon));
            u16 damage = GetBattleMonStat(target, 0xd);
            u32 critical = func_ov093_021d7ad4(effectiveness);
            BOOL substitute = FALSE;
            BOOL endured = FALSE;
            BattleMoveEffectState *effect = flow->moveEffect;

            if (!effect->enabled) {
                effect->enabled = 1;
            }
            if (IsSubstituteActive(target)) {
                damage = func_ov093_021c3720(flow, target, param, critical);
                data_ov095_0689cbb9(targets, target, damage, TRUE);
                substitute = TRUE;
            } else {
                u32 cause = func_ov093_021c1678(flow, mon, target, 1, &damage);
                if (cause == 0) {
                    ServerControl_OHKOSuccess(flow, target, param, critical);
                } else {
                    func_ov093_021c36e8(flow, target, param, critical, cause, damage);
                    data_ov095_0689cbb9(targets, target, damage, FALSE);
                    endured = TRUE;
                }
            }
            if (!substitute) {
                func_ov093_021c1828(flow, attackerPos, mon, target, param, damage);
            }
            func_ov093_021c4218(flow, mon, target, param, effectiveness, damage, 0, substitute);
            if (endured || substitute) {
                func_ov093_021c1c88(flow, mon, targets, param, 0);
            }
            ServerControl_CheckFainted(flow, target);
        } else if (!BattleHandler_Result(flow)) {
            func_ov093_021c572c(flow, target, param->move);
        }
        PopState(&flow->actionState, state, 0x219c);
    }
}













void ServerControl_OHKOSuccess(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical) {
    u8 monId = GetMonID(mon);
    HPZero(mon);
    func_ov093_021cd4b0(flow->queue, 3, monId);
    func_ov093_021cd4b0(flow->queue, 0x32, monId, (u8)critical, param->move);
    func_ov093_021cd4b0(flow->queue, 0x34, monId);
}

void func_ov093_021c36e8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical, u32 cause,
                         u16 damage) {
    u16 move;
    u8 monId;

    func_ov093_021c604c(flow, mon, damage);
    move = param->move;
    monId = GetMonID(mon);
    func_ov093_021cd4b0(flow->queue, 0x32, monId, (u8)critical, move);
    func_ov093_021c16f8(flow, mon, cause);
}


u16 func_ov093_021c3720(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical) {
    u16 damage;
    u8 monId;

    monId = GetMonID(mon);
    damage = func_ov093_021d6d90(mon);
    func_ov093_021cd4b0(flow->queue, 0x34, monId);
    func_ov093_021cd4b0(flow->queue, 0x32, monId, (u8)critical, param->move);
    func_ov093_021c41c0(flow, mon);
    return damage;
}

void ServerControl_ForceSwitch(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets) {
    BOOL failed;
    BattleMon *target;

    failed = FALSE;
    data_ov095_0689cc85(targets);
    target = data_ov095_0689cc8d(targets);
    while (target != NULL) {
        if (ServerControl_ForceSwitchCore(flow, mon, target, 0, &failed, 0, 0, 0)) {
            if (!flow->moveEffect->enabled) {
                flow->moveEffect->enabled = TRUE;
            }
        } else if (!failed) {
            func_ov093_021c5718(flow, mon, move);
        }
        target = data_ov095_0689cc8d(targets);
    }
}









BOOL ServerControl_ForceSwitchCore(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BOOL forced,
                                   BOOL *failed, u16 effect, BOOL ignoreLevel, const BattleHandlerString *string) {
    u8 clientPos[2];
    u8 pos;
    u32 state;
    u8 blocked;
    u32 mode;

    *failed = FALSE;
    if (forced) {
        mode = 1;
    } else {
        mode = func_ov093_021c39cc(flow);
    }
    if (mode == 2) {
        return FALSE;
    }
    pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(target));
    if (pos == 6) {
        return FALSE;
    }
    if (IsFainted(target)) {
        return FALSE;
    }
    if (CheckCondition(target, 0x21)) {
        return FALSE;
    }
    if (BtlFlow_GetSkyDropTarget(target) != 0x1f) {
        return FALSE;
    }
    state = PushState(&flow->actionState, 0x221c);
    blocked = func_ov093_021c3a4c(flow, attacker, target);
    if (blocked && BattleHandler_Result(flow)) {
        *failed = TRUE;
    }
    PopState(&flow->actionState, state, 0x2224);
    if (blocked) {
        return FALSE;
    }
    func_ov093_021b8e28(flow->mainModule, pos, &clientPos[1], &clientPos[0]);
    if (mode == 1) {
        s32 slot = func_ov093_021c39f4(flow, func_ov093_021bb85c(flow->server, clientPos[1]));
        if (slot >= 0) {
            u8 newMonId = GetMonID(func_ov093_021b990c(flow->pokeCon, clientPos[1], slot));
            ServerControl_SwitchOutCore(flow, target, effect);
            if (string != NULL) {
                BattleHandler_SetString(flow, string);
            }
            ServerControl_SwitchInFillSlot(flow, clientPos[1], clientPos[0], slot, FALSE);
            func_ov093_021cd64c(flow->queue, 0x5b, 0x34d, newMonId, 0xffff0000);
            ServerControl_AfterSwitchIn(flow);
        } else {
            return FALSE;
        }
    } else {
        u8 attackerClient = func_ov093_021b8ddc(GetMonID(attacker));
        if (!ignoreLevel) {
            u8 attackerLevel = GetBattleMonStat(attacker, 0xf);
            if ((u8)GetBattleMonStat(target, 0xf) > attackerLevel) {
                return FALSE;
            }
        }
        func_ov093_021d8244(&flow->clientIdList, attackerClient);
        ServerControl_SwitchOutCore(flow, target, effect);
        flow->unk14 = 5;
    }
    return TRUE;
}


u32 func_ov093_021c39cc(BtlServerFlow *flow) {
    u32 style = BtlSetup_GetBattleStyle(flow->mainModule);
    u32 type = BtlSetup_GetBattleType(flow->mainModule);
    if (style == 0) {
        switch (type) {
        case 0:
            return 0;
        default:
            return 1;
        }
    }
    return 1;
}

s32 func_ov093_021c39f4(BtlServerFlow *flow, BtlServerClient *client) {
    u8 slots[6];
    u32 count = GetNumMonsInParty(client->party);
    u32 i = func_ov093_021b99c0(flow->mainModule, client->clientId);
    u32 numSlots = 0;

    for (; i < count; i++) {
        if (CanPokemonBattle(GetBattleMonFromParty(client->party, i))) {
            slots[numSlots++] = i;
        }
    }
    if (numSlots != 0) {
        return slots[BattleRandom(numSlots)];
    }
    return -1;
}

BOOL func_ov093_021c3a4c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target) {
    BOOL blocked;

    BattleEventVar_Push(0x22ad);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEvent_CallHandlers(flow, 0x8b);
    data_ov095_0689c7a5(flow, target);
    blocked = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x22b3);
    return blocked;
}



void ServerControl_FieldEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon) {
    u8 weather = GetMoveWeather(param->move);

    if (weather != 0) {
        if (ServerControl_ChangeWeather(flow, weather, ServerEvent_IncreaseMoveWeatherTurns(flow, weather, mon) + 5)) {
            BattleMoveEffectState *effect = flow->moveEffect;
            if (!effect->enabled) {
                effect->enabled = 1;
            }
        } else {
            func_ov093_021c5718(flow, mon, param->move);
        }
    } else {
        BOOL success;
        u32 state = PushState(&flow->actionState, 0x22c8);
        func_ov093_021c3ccc(flow, mon, param->move);
        success = BattleHandler_Result(flow) == 2;
        PopState(&flow->actionState, state, 0x22cc);
        if (success) {
            BattleMoveEffectState *effect = flow->moveEffect;
            if (!effect->enabled) {
                effect->enabled = 1;
            }
        } else {
            func_ov093_021c5718(flow, mon, param->move);
        }
    }
}

BOOL ServerControl_ChangeWeather(BtlServerFlow *flow, u8 weather, u8 turns) {
    if (ServerControl_ChangeWeatherCheck(flow, weather, turns)) {
        ServerControl_ChangeWeatherCore(flow, weather, turns);
        return TRUE;
    }
    return FALSE;
}

BOOL ServerControl_ChangeWeatherCheck(BtlServerFlow *flow, u8 weather, u8 duration) {
    if (weather >= 5) {
        return FALSE;
    }
    if (weather == func_ov093_021ef884() && (duration != 0xff || func_ov093_021ef894() == 0xff)) {
        return FALSE;
    }
    return TRUE;
}

void ServerControl_ChangeWeatherCore(BtlServerFlow *flow, u8 weather, u8 duration) {
    FieldStatusSetWeather(weather, duration);
    func_ov093_021cd4b0(flow->queue, 0x3f, weather, duration);
    ServerControl_ChangeWeatherAfter(flow, weather);
}

void ServerControl_ChangeWeatherAfter(BtlServerFlow *flow, u8 weather) {
    u32 handle;

    handle = PushState(&flow->actionState, 0x2303);
    func_ov093_021c7998(flow, weather);
    PopState(&flow->actionState, handle, 0x2305);
}


u8 ServerEvent_IncreaseMoveWeatherTurns(BtlServerFlow *flow, u8 weather, BattleMon *mon) {
    u8 turns;

    BattleEventVar_Push(0x2317);
    BattleEventVar_SetConstValue(0x39, weather);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetValue(0x24, 0);
    BattleEvent_CallHandlers(flow, 0x7c);
    turns = BattleEventVar_GetValue(0x24);
    BattleEventVar_Pop(0x231d);
    return turns;
}

BOOL ServerControl_FieldEffectCore(BtlServerFlow *flow, u32 effect, BattleCondition value, u8 dependPoke) {
    if (FieldStatusAddEffect(effect, value)) {
        func_ov093_021cd4b0(flow->queue, 0x21, (u8)effect, value.raw);
        return TRUE;
    }
    if (dependPoke) {
        u8 monId = Condition_GetMonID(value);
        if (monId != 0x1f) {
            FieldStatusAddDependPoke(effect, monId);
            func_ov093_021cd4b0(flow->queue, 0x22, (u8)effect, monId);
            return TRUE;
        }
    }
    return FALSE;
}


void func_ov093_021c3ccc(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    u8 monId;

    BattleEventVar_Push(0x2360);
    monId = GetMonID(mon);
    BattleEventVar_SetValue(3, monId);
    BattleEventVar_SetValue(0x12, move);
    BattleEvent_CallHandlers(flow, 0x9e);
    BattleEventVar_Pop(0x2364);
}

void func_ov093_021c3d08(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets) {
    if (!data_ov095_0689cd51(targets)) {
        switch (param->move) {
        case 0x11d:
            func_ov093_021c3e0c(flow, mon, targets);
            break;
        case 0xa4:
            if (func_ov093_021c4034(flow, mon)) {
                BattleMoveEffectState *effect = flow->moveEffect;
                if (!effect->enabled) {
                    effect->enabled = 1;
                    effect->unk05_1 = 1;
                }
            }
            break;
        default: {
            BOOL showFail;
            u32 result;
            u32 state = PushState(&flow->actionState, 0x237f);

            result = BattleHandler_Result(flow);
            if (func_ov093_021c4304(flow, param, mon, targets, &showFail)) {
                result = BattleHandler_Result(flow);
                if (result == 2) {
                    BattleMoveEffectState *effect = flow->moveEffect;
                    if (!effect->enabled) {
                        effect->enabled = 1;
                    }
                }
            }
            if (result <= 1 && showFail && !flow->unk78A_4) {
                func_ov093_021cd64c(flow->queue, 0x5a, 0x47, 0xffff0000);
            }
            PopState(&flow->actionState, state, 0x2399);
            break;
        }
        }
    }
}



void func_ov093_021c3e0c(BtlServerFlow *flow, BattleMon *mon, void *targets) {
    BattleMon *target;
    u32 ability;
    u32 targetAbility;
    u8 monId;
    u8 targetId;
    u32 state;

    target = data_ov095_0689cc71(targets, 0);
    ability = GetBattleMonStat(mon, 0x10);
    targetAbility = GetBattleMonStat(target, 0x10);
    if (ability != targetAbility && !data_ov095_0689c955(ability) && !data_ov095_0689c955(targetAbility)) {
        monId = GetMonID(mon);
        targetId = GetMonID(target);
        if (!flow->moveEffect->enabled) {
            flow->moveEffect->enabled = 1;
        }
        func_ov093_021cd4b0(flow->queue, 0x4a, monId, targetId, (u16)targetAbility, (u16)ability);
        func_ov093_021cd64c(flow->queue, 0x5b, 0x1fc, monId, 0xffff0000);
        state = PushState(&flow->actionState, 0x23b4);
        func_ov093_021c7b3c(flow, monId, ability, targetAbility);
        func_ov093_021c7b3c(flow, targetId, targetAbility, ability);
        PopState(&flow->actionState, state, 0x23b7);
        ChangeAbility(mon, targetAbility);
        ChangeAbility(target, ability);
        AbilityEvent_Swap(mon, target);
        func_ov093_021cd4b0(flow->queue, 0x58, monId);
        func_ov093_021cd4b0(flow->queue, 0x58, targetId);
        if (ability != targetAbility) {
            state = PushState(&flow->actionState, 0x23c4);
            func_ov093_021c7b7c(flow, monId);
            func_ov093_021c7b7c(flow, targetId);
            PopState(&flow->actionState, state, 0x23c7);
        }
        if (!CheckCondition(mon, 0x10)) {
            if (ability == 0x67) {
                ServerControl_CheckItemReaction(flow, mon, 0);
            }
            if (ability == 0x7f) {
                ServerControl_UnnerveAction(flow, mon);
            }
        }
        if (!CheckCondition(target, 0x10)) {
            if (targetAbility == 0x67) {
                ServerControl_CheckItemReaction(flow, target, 0);
            }
            if (targetAbility == 0x7f) {
                ServerControl_UnnerveAction(flow, target);
            }
        }
    } else {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x47, 0xffff0000);
    }
}










void ServerControl_UnnerveAction(BtlServerFlow *flow, BattleMon *mon) {
    BtlFlowMonIter iter;
    BattleMon *target;
    u8 monId = GetMonID(mon);

    func_ov093_021bd2b4(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &target)) {
        if (!IsAllyMonID(monId, GetMonID(target))) {
            ServerControl_CheckItemReaction(flow, target, 0);
        }
    }
}


BOOL func_ov093_021c4034(BtlServerFlow *flow, BattleMon *mon) {
    if (!IsSubstituteActive(mon)) {
        s32 cost = DivideMaxHPZeroCheck(mon, 4);
        if ((s32)GetBattleMonStat(mon, 0xd) > cost) {
            u8 pos = GetBattlePos(&flow->posPoke, GetMonID(mon));
            if (pos != 6) {
                ServerDisplay_SimpleHP(flow, mon, -cost, TRUE);
                ServerControl_CheckItemReaction(flow, mon, 1);
                func_ov093_021d6d5c(mon, cost);
                func_ov093_021cd4b0(flow->queue, 0x27, GetMonID(mon), (u16)cost);
                func_ov093_021cd4b0(flow->queue, 0x51, pos);
                func_ov093_021cd64c(flow->queue, 0x5b, 0x311, GetMonID(mon), 0xffff0000);
                return TRUE;
            }
        }
        ServerDisplay_StandardMessage(flow, 0x7b, 0, NULL);
    } else {
        ServerDisplay_SkyDropTargetAppear(flow, mon, 0x314);
    }
    return FALSE;
}




u16 func_ov093_021c4104(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 damage, u32 effectiveness,
                        u8 critical, BtlFlowMoveParam *param) {
    ServerCmd_PutMoveDamage(flow->queue, GetMonID(target), effectiveness, param->move);
    ServerDisplay_SkyDropTargetAppear(flow, target, 0x317);
    func_ov093_021c5840(flow, 1, &effectiveness, &target, FALSE);
    func_ov093_021c59c4(flow, 1, &target, &critical, FALSE);
    if (func_ov093_021d6d9c(target, &damage)) {
        func_ov093_021c41c0(flow, target);
    }
    ServerControl_DamageDrain(flow, param, attacker, target, damage);
    func_ov093_021c1880(flow, param, attacker, target, damage, TRUE);
    func_ov093_021c4218(flow, attacker, target, param, effectiveness, damage, critical, TRUE);
    return damage;
}

void func_ov093_021c41c0(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId = GetMonID(mon);
    u8 pos = GetBattlePos(&flow->posPoke, monId);
    ServerDisplay_SkyDropTargetAppear(flow, mon, 0x31a);
    ResetSpActPriority(mon);
    func_ov093_021cd4b0(flow->queue, 0x28, monId);
    func_ov093_021cd4b0(flow->queue, 0x52, pos);
}



void func_ov093_021c4218(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param,
                         u32 effectiveness, u32 damage, u32 critical, BOOL flag) {
    u32 state = PushState(&flow->actionState, 0x245e);
    func_ov093_021c4268(flow, attacker, target, param, effectiveness, damage, critical, flag);
    PopState(&flow->actionState, state, 0x2461);
}

void func_ov093_021c4268(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param,
                         u32 effectiveness, u32 damage, u32 critical, BOOL flag) {
    u8 targetId = GetMonID(target);

    BattleEventVar_Push(0x2477);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, targetId);
    BattleEventVar_SetConstValue(0x14, param->originalMove);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x38, effectiveness);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x1a, param->category);
    BattleEventVar_SetConstValue(0x32, damage);
    BattleEventVar_SetConstValue(0x45, critical);
    BattleEventVar_SetConstValue(0x46, flag);
    BattleEventVar_SetRewriteOnceValue(0x47, 0);
    BattleEvent_CallHandlers(flow, 0x4a);
    BattleEvent_CallHandlers(flow, 0x4b);
    BattleEvent_CallHandlers(flow, 0x4c);
    BattleEventVar_Pop(0x2488);
}


BOOL func_ov093_021c4304(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, BOOL *showFail) {
    u8 realCount;
    u8 count;
    u8 i;

    BattleEventVar_Push(0x249a);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    realCount = data_ov095_0689cd41(targets);
    count = data_ov095_0689cd39(targets);
    BattleEventVar_SetConstValue(5, count);
    BattleEventVar_SetRewriteOnceValue(0x51, 1);
    for (i = 0; i < count; i++) {
        BattleEventVar_SetConstValue(6 + i, GetMonID(data_ov095_0689cc71(targets, i)));
    }
    BattleEventVar_SetConstValue(0x12, param->move);
    if (realCount) {
        if (count) {
            BattleEvent_CallHandlers(flow, 0xa0);
        }
    } else {
        BattleEvent_CallHandlers(flow, 0xa1);
    }
    *showFail = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x24ba);
    if (BattleHandler_Result(flow)) {
        return TRUE;
    }
    return FALSE;
}









void StoreBattleMonsSpeedOrder(BtlServerFlow *flow, void *monSet) {
    BtlFlowMonIter iter;
    BattleMon *mon;

    data_ov095_0689cb3d(monSet);
    func_ov093_021bd2b4(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &mon)) {
        data_ov095_0689cb49(monSet, mon);
    }
    SortBySpeed(monSet, flow);
}











BOOL func_ov093_021c446c(BtlServerFlow *flow) {
    void *monSet = flow->unk854;
    BOOL result = FALSE;
    BattleMon *mon;

    if (flow->unk77E == 0) {
        StoreBattleMonsSpeedOrder(flow, monSet);
        flow->unk77E = 1;
        func_ov093_021c4744(flow);
    }
    switch (flow->unk77E) {
    case 1:
        flow->unk77E++;
        func_ov093_021cd4b0(flow->queue, 0x56, 0);
        result = FALSE;
        if (func_ov093_021c4d1c(flow, monSet)) {
            result = TRUE;
            break;
        }
        if (ServerControl_CheckMatchup(flow)) {
            return result;
        }
    case 2:
        flow->unk77E++;
        if (func_ov093_021c4838(flow, monSet, 0x76)) {
            result = TRUE;
            break;
        }
        if (ServerControl_CheckMatchup(flow)) {
            return FALSE;
        }
    case 3:
        flow->unk77E++;
        if (func_ov093_021c493c(flow, monSet)) {
            result = TRUE;
            break;
        }
        if (ServerControl_CheckMatchup(flow)) {
            return FALSE;
        }
    case 4:
        flow->unk77E++;
        func_ov093_021c4b8c(flow);
        func_ov093_021c4c24(flow);
    case 5:
        flow->unk77E++;
        if (func_ov093_021c4838(flow, monSet, 0x77)) {
            result = TRUE;
            break;
        }
        if (ServerControl_CheckMatchup(flow)) {
            return FALSE;
        }
    case 6:
        func_ov093_021c4838(flow, monSet, 0x78);
        flow->unk77E++;
        data_ov095_0689cc85(monSet);
        for (mon = data_ov095_0689cc8d(monSet); mon != NULL; mon = data_ov095_0689cc8d(monSet)) {
            func_ov093_021d6408(mon);
            ComboMove_ClearParam(mon);
            func_ov093_021cd4b0(flow->queue, 0x2e, GetMonID(mon));
        }
        func_ov093_021c4614(flow);
        BattleEvent_RemoveIsolatedItems();
        if (flow->unk10 < 0x270f) {
            flow->unk10++;
        }
        flow->unk77E = 0;
        flow->unk774 = 0;
        return FALSE;
    }
    return result;
}










void func_ov093_021c4614(BtlServerFlow *flow) {
    if (BtlSetup_GetBattleStyle(flow->mainModule) == 2) {
        u8 playerClient = GetPlayerClientID(flow->mainModule);
        u8 firstClient = playerClient;
        u8 secondClient = func_ov093_021b9064(flow->mainModule, playerClient, 0);
        BattleParty *firstParty;
        BattleParty *secondParty;

        if (playerClient > secondClient) {
            firstClient = secondClient;
            secondClient = playerClient;
        }
        firstParty = GetPartyData(flow->pokeCon, firstClient);
        secondParty = GetPartyData(flow->pokeCon, secondClient);
        if (GetAlivePartyCount(firstParty) == 1 && GetAlivePartyCount(secondParty) == 1) {
            BattleMon *firstMon = func_ov093_021b9ce8(firstParty);
            BattleMon *secondMon = func_ov093_021b9ce8(secondParty);
            u8 firstId = GetMonID(firstMon);
            u8 secondId = GetMonID(secondMon);
            u8 firstPos = GetBattlePos(&flow->posPoke, firstId);
            u8 secondPos = GetBattlePos(&flow->posPoke, secondId);

            if (firstPos != 6 && secondPos != 6) {
                u8 firstSlot = func_ov093_021b8dec(flow->mainModule, firstPos);
                u8 secondSlot = func_ov093_021b8dec(flow->mainModule, secondPos);
                if (firstSlot == secondSlot && !func_ov093_021b99f0(firstPos)) {
                    func_ov093_021cd4b0(flow->queue, 0x50, firstClient, firstSlot, secondClient, secondSlot);
                    ServerControl_MoveCore(flow, firstClient, firstSlot, 1, TRUE);
                    ServerControl_MoveCore(flow, secondClient, secondSlot, 1, TRUE);
                    ServerControl_AfterMove(flow, firstClient, firstSlot, 1);
                    ServerControl_AfterMove(flow, secondClient, secondSlot, 1);
                }
            }
        }
    }
}







void func_ov093_021c4744(BtlServerFlow *flow) {
    void *passPower = func_ov093_021b8630(flow->mainModule);

    if (passPower != NULL) {
        u32 type = func_0202e7e0(passPower);
        if (type != 0 && type != 3) {
            u8 monIds[3];
            u8 count;
            u8 i;
            u8 clientId = GetPlayerClientID(flow->mainModule);
            BtlServerClient *client = func_ov093_021bb85c(flow->server, clientId);

            for (i = 0, count = 0; i < client->numCoverPos; i++) {
                BattleMon *mon = func_ov093_021b990c(flow->pokeCon, clientId, i);
                if (BtlFlow_IsMonAlive(mon) && !IsMonFullHP(mon)) {
                    monIds[count++] = GetMonID(mon);
                }
            }
            if (count != 0) {
                u8 monId = monIds[GFL_RandomMTRange(count)];
                BattleMon *mon = GetPokeParam(flow->pokeCon, monId);
                u32 amount = GetBattleMonStat(mon, 0xe);
                if (type == 1) {
                    amount >>= 1;
                }
                if (amount != 0 && ServerControl_RecoverHP(flow, mon, amount, FALSE)) {
                    func_ov093_021cd64c(flow->queue, 0x5a, 0x53, 4, monId, 0xffff0000);
                    func_0202e7e4(passPower);
                }
            }
        }
    }
}

BOOL func_ov093_021c4838(BtlServerFlow *flow, void *monSet, u32 event) {
    u32 state;
    BattleMon *mon;

    data_ov095_0689cc85(monSet);
    state = PushState(&flow->actionState, 0x25ac);
    func_ov093_021c4910(flow, 0x1f, event);
    PopState(&flow->actionState, state, 0x25ae);
    if (!ServerControl_CheckMatchup(flow)) {
        for (mon = data_ov095_0689cc8d(monSet); mon != NULL; mon = data_ov095_0689cc8d(monSet)) {
            state = PushState(&flow->actionState, 0x25b4);
            func_ov093_021c4910(flow, GetMonID(mon), event);
            PopState(&flow->actionState, state, 0x25b6);
            ServerControl_CheckFainted(flow, mon);
            if (ServerControl_CheckMatchup(flow)) {
                break;
            }
        }
    }
    return func_ov093_021c5200(flow);
}










void func_ov093_021c4910(BtlServerFlow *flow, u8 monId, u32 event) {
    BattleEventVar_Push(0x25c3);
    BattleEventVar_SetConstValue(2, monId);
    BattleEvent_CallHandlers(flow, event);
    BattleEventVar_Pop(0x25c6);
}

BOOL func_ov093_021c493c(BtlServerFlow *flow, void *monSet) {
    u8 monIds[6];
    BattleCondition prev;
    BOOL cured;
    u32 state;
    u32 count;
    u32 index;
    u32 i;

    monIds[0] = 0;
    monIds[1] = 0;
    monIds[2] = 0;
    monIds[3] = 0;
    monIds[4] = 3;
    monIds[5] = 0;
    count = data_ov095_0689cd39(monSet);
    for (i = 0; i < count; i++) {
        monIds[i] = GetMonID(data_ov095_0689cc71(monSet, i));
    }
    index = 0;
    while (TRUE) {
        u32 condition = data_ov095_0689c9f9(index++);
        if (condition == 0) {
            break;
        }
        for (i = 0; i < count; i++) {
            BattleMon *mon = GetPokeParam(flow->pokeCon, monIds[i]);
            if (!IsFainted(mon) && func_ov093_021d6064(mon, condition, &prev, &cured)) {
                state = PushState(&flow->actionState, 0x25f7);
                data_ov095_0689b7a9(mon, condition, prev, cured, flow);
                PopState(&flow->actionState, state, 0x25f9);
            }
        }
        if (ServerControl_CheckMatchup(flow)) {
            break;
        }
    }
    func_ov093_021cd4b0(flow->queue, 0x15, (u8)count, (u8)index, func_ov093_021d806c(monIds));
    return func_ov093_021c5200(flow);
}













void func_ov093_021c4a64(BtlServerFlow *flow, BattleMon *mon, u32 condition, u32 damage) {
    u32 state = PushState(&flow->actionState, 0x261b);
    u32 amount = func_ov093_021c4b3c(flow, mon, condition, damage);

    if (amount != 0 && ServerControl_CheckSimpleDamageEnabled(flow, mon, amount)) {
        switch (condition) {
        case 5:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x257);
            break;
        case 4:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x258);
            break;
        case 10:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x273);
            break;
        case 9:
            ServerDisplay_AddEffectAtPosition(flow, mon, 0x274);
            break;
        }
        BattleHandler_StrClear(&flow->message);
        data_ov095_0689b8cd(&flow->message, mon, condition);
        ServerControl_SimpleDamageCore(flow, mon, amount, &flow->message);
    }
    PopState(&flow->actionState, state, 0x263b);
}




u32 func_ov093_021c4b3c(BtlServerFlow *flow, BattleMon *mon, u32 condition, u32 damage) {
    u32 result;

    BattleEventVar_Push(0x2649);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x1d, condition);
    BattleEventVar_SetValue(0x32, damage);
    BattleEvent_CallHandlers(flow, 0x6b);
    result = BattleEventVar_GetValue(0x32);
    BattleEventVar_Pop(0x264f);
    return result;
}

void func_ov093_021c4b8c(BtlServerFlow *flow) {
    data_ov095_06898d55(ServerControl_SideEffectEndMessage, flow);
}

void ServerControl_SideEffectEndMessage(u8 side, u32 effect, BtlServerFlow *flow) {
    func_ov093_021c4bac(flow, effect, side);
}

void func_ov093_021c4bac(BtlServerFlow *flow, u32 effect, u32 side) {
    s32 message = -1;

    switch (effect) {
    case 0:
        message = 0x7e;
        break;
    case 1:
        message = 0x82;
        break;
    case 2:
        message = 0x86;
        break;
    case 3:
        message = 0x8a;
        break;
    case 4:
        message = 0x8e;
        break;
    case 5:
        message = 0x92;
        break;
    case 6:
        message = 0x96;
        break;
    case 7:
        message = 0x9a;
        break;
    case 8:
        message = 0x9e;
        break;
    case 11:
        message = 0xa6;
        break;
    case 12:
        message = 0xaa;
        break;
    case 13:
        message = 0xae;
        break;
    }
    if (message >= 0) {
        func_ov093_021cd64c(flow->queue, 0x5a, message, side, 0xffff0000);
    }
}

void func_ov093_021c4c24(BtlServerFlow *flow) {
    func_ov093_021ef934(func_ov093_021c4c40, flow);
    func_ov093_021cd4b0(flow->queue, 0x2f, 0);
}

void func_ov093_021c4c40(u32 effect, BtlServerFlow *flow) {
    ServerControl_FieldEffectEnd(flow, effect);
}

void ServerControl_FieldEffectEnd(BtlServerFlow *flow, u32 effect) {
    s32 message = -1;

    switch (effect) {
    case 1:
        message = 0x74;
        break;
    case 2:
        message = 0x76;
        break;
    case 6:
        message = 0xb3;
        break;
    case 7:
        message = 0xb5;
        break;
    }
    if (message >= 0) {
        func_ov093_021cd64c(flow->queue, 0x5a, message, 0xffff0000);
    }
    func_ov093_021cd4b0(flow->queue, 0x24, (u8)effect);
    if (effect == 7) {
        BattleMon *mon;

        StoreBattleMonsSpeedOrder(flow, flow->unk868);
        data_ov095_0689cc85(flow->unk868);
        for (mon = data_ov095_0689cc8d(flow->unk868); mon != NULL; mon = data_ov095_0689cc8d(flow->unk868)) {
            if (CanPokemonBattle(mon)) {
                ServerControl_CheckItemReaction(flow, mon, 0);
            }
        }
    }
}










BOOL func_ov093_021c4d1c(BtlServerFlow *flow, void *monSet) {
    u8 ended = func_ov093_021ef8b8();
    u32 weather;
    BOOL damaged;
    BattleMon *mon;

    if (ended != 0) {
        func_ov093_021cd4b0(flow->queue, 0x40, ended);
        ServerControl_ChangeWeatherAfter(flow, 0);
        return FALSE;
    }
    damaged = FALSE;
    weather = ServerEvent_GetWeather(flow);
    data_ov095_0689cc85(monSet);
    for (mon = data_ov095_0689cc8d(monSet); mon != NULL; mon = data_ov095_0689cc8d(monSet)) {
        if (!IsFainted(mon) && !GetAdditionalConditionFlag(mon, 5) && !GetAdditionalConditionFlag(mon, 4)) {
            s32 damage;
            u32 state = PushState(&flow->actionState, 0x26c6);
            damage = func_ov093_021c4e38(flow, mon, weather, func_ov093_021d7bd4(mon, weather));
            if (damage != 0) {
                func_ov093_021c4ea4(flow, mon, weather, damage);
                damaged = TRUE;
            }
            PopState(&flow->actionState, state, 0x26d2);
            ServerControl_CheckFainted(flow, mon);
        }
    }
    if (damaged) {
        ServerControl_ViewEffect(flow, 0x255, 6, 6, 0, 0);
    }
    return func_ov093_021c5200(flow);
}










s32 func_ov093_021c4e38(BtlServerFlow *flow, BattleMon *mon, u32 weather, s32 damage) {
    u8 cancel;
    s32 value;
    s32 result;

    BattleEventVar_Push(0x26e8);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x39, weather);
    result = 0;
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEventVar_SetValue(0x32, damage);
    BattleEvent_CallHandlers(flow, 0x7f);
    value = BattleEventVar_GetValue(0x32);
    cancel = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x26f1);
    if (!cancel) {
        result = value;
    }
    return result;
}

void func_ov093_021c4ea4(BtlServerFlow *flow, BattleMon *mon, u32 weather, s32 damage) {
    u8 monId = GetMonID(mon);

    switch (weather) {
    case 4:
        BattleHandler_StrSetup(&flow->message, 2, 0x18c);
        BattleHandler_AddArg(&flow->message, monId);
        break;
    case 3:
        BattleHandler_StrSetup(&flow->message, 2, 0x18f);
        BattleHandler_AddArg(&flow->message, monId);
        break;
    default:
        BattleHandler_StrClear(&flow->message);
        break;
    }
    if (damage > 0 && ServerControl_CheckSimpleDamageEnabled(flow, mon, damage)) {
        BattleHandler_SetString(flow, &flow->message);
        BattleHandler_StrClear(&flow->message);
        ServerControl_ViewEffect(flow, 0x290, func_ov093_021c8010(flow, monId), 6, 0, 0);
        if ((s32)GetBattleMonStat(mon, 0xd) <= damage) {
            ServerControl_ViewEffect(flow, 0x255, 6, 6, 0, 0);
        }
        ServerControl_SimpleDamageCore(flow, mon, damage, NULL);
    }
}

BOOL ServerControl_CheckFainted(BtlServerFlow *flow, BattleMon *mon) {
    u8 monId = GetMonID(mon);

    if (!flow->unk7A9[monId] && IsFainted(mon)) {
        BOOL bigLevelGap;
        u8 clientId;
        u8 playerClient;

        flow->unk7A9[monId] = TRUE;
        data_ov095_0689d15d(&flow->faintRec, monId);
        if (!BtlSetup_IsBattleType(flow->mainModule, 0x200) || func_ov093_021b8ddc(monId) == 0) {
            if (flow->unk789 != monId) {
                func_ov093_021cd64c(flow->queue, 0x5b, 0, monId, 0xffff0000);
            }
        }
        func_ov093_021cd4b0(flow->queue, 0x39, monId);
        ServerControl_ClearMonDependentEffects(flow, mon, FALSE);
        Clear_ForFainted(mon);
        if (func_ov093_021b8ddc(monId) == GetPlayerClientID(flow->mainModule)) {
            bigLevelGap = FALSE;
            s32 level = GetBattleMonStat(mon, 0xf);
            if (level + 30 <= (s32)GetEnemyMaxLevel(flow)) {
                bigLevelGap = TRUE;
            }
            ChangeFriendshipWhenFainted(flow->mainModule, mon, bigLevelGap);
        }
        data_ov095_0689d2f9(&flow->posPoke, monId);
        clientId = func_ov093_021b8ddc(monId);
        playerClient = GetPlayerClientID(flow->mainModule);
        if (clientId == playerClient) {
            func_ov093_021ba1b4(flow->mainModule, 0x4c);
        } else if (!IsAllyClientID(clientId, playerClient)) {
            func_ov093_021ba1b4(flow->mainModule, 0x19);
            func_ov093_021ba1b4(flow->mainModule, 0x52);
        }
        return TRUE;
    }
    return FALSE;
}






u32 GetEnemyMaxLevel(BtlServerFlow *flow) {
    u32 maxLevel = 1;
    u32 posMax = GetValidPosMax(flow->mainModule);
    u8 playerClient = GetPlayerClientID(flow->mainModule);
    u32 pos;

    for (pos = 0; pos <= posMax; pos++) {
        if (!IsAllyClientID(playerClient, func_ov093_021b8de4(flow->mainModule, pos))) {
            u8 monId = GetExistPokeID(&flow->posPoke, pos);
            if (monId != 0x1f) {
                u32 level = GetBattleMonStat(GetPokeParam(flow->pokeCon, monId), 0xf);
                if (level > maxLevel) {
                    maxLevel = level;
                }
            }
        }
    }
    return maxLevel;
}




void ServerControl_ClearMonDependentEffects(BtlServerFlow *flow, BattleMon *mon, BOOL rotation) {
    BtlFlowMonIter iter;
    BattleMon *other;
    u8 monId = GetMonID(mon);

    ServerEvent_BeforeFaint(flow, mon);
    ServerControl_SkyDropCheckRelease(flow, mon, FALSE);
    if (!rotation) {
        AbilityEvent_RemoveItem(mon);
        ItemEvent_RemoveItem(mon);
    } else {
        AbilityEvent_ItemRotationSleep(mon);
        ItemEvent_ItemRotationSleep(mon);
    }
    RemoveForceAll(mon);
    func_ov093_021bd2b4(&iter, flow);
    func_ov093_021bd338(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &other)) {
        func_ov093_021d6264(other, monId);
        func_ov093_021cd4b0(flow->queue, 0x2c, GetMonID(other), monId);
    }
    func_ov093_021ef90c(monId);
    func_ov093_021cd4b0(flow->queue, 0x23, monId);
    if (!ServerControl_CheckMatchup(flow) && (u16)GetBattleMonStat(mon, 0x11) == 0x7f) {
        ServerControl_UnnerveAction(flow, mon);
    }
}

void ServerEvent_BeforeFaint(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x27df);
    BattleEventVar_SetValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0xa3);
    BattleEventVar_Pop(0x27e2);
}

BOOL func_ov093_021c5200(BtlServerFlow *flow) {
    BOOL result;
    int i;
    u32 count;
    BattleMon *mon;

    result = FALSE;
    if (func_ov093_021b85ec(flow->mainModule)) {
        count = data_ov095_0689d175(&flow->faintRec, 0);
        for (i = 0; i < count; i++) {
            if (!data_ov095_0689d1a1(&flow->faintRec, 0, i)) {
                mon = GetPokeParam(flow->pokeCon, data_ov095_0689d185(&flow->faintRec, 0, i));
                if (BtlSetup_GetBattleType(flow->mainModule) == 0 && !flow->unk78A_1 &&
                    ServerControl_CheckMatchup(flow) && func_ov093_021bc310(flow)) {
                    func_ov093_021cd4b0(flow->queue, 0x55, func_ov093_021b86dc(flow->mainModule));
                    flow->unk78A_1 = 1;
                }
                if (func_ov093_021c5308(flow, mon)) {
                    result = TRUE;
                }
                data_ov095_0689d1bd(&flow->faintRec, 0, i);
            }
        }
    }
    return result;
}















BOOL func_ov093_021c5308(BtlServerFlow *flow, BattleMon *mon) {
    BattleParty *party;

    if (GetSideFromMonID(GetMonID(mon)) == 1) {
        party = GetPartyData(flow->pokeCon, GetPlayerClientID(flow->mainModule));
        AddExpAndEVs(flow, party, mon, flow->expEntries);
        return func_ov093_021c5350(flow, party, flow->expEntries);
    }
    return FALSE;
}

BOOL func_ov093_021c5350(BtlServerFlow *flow, BattleParty *party, const BtlFlowExpEntry *entries) {
    u32 remaining;
    u32 i;
    int monId;
    u32 message;
    BOOL result;

    result = FALSE;
    for (i = 0; i < 6; i++) {
        BtlFlowExpEntry *entry = &entries[i];
        if (entries[i].exp != 0) {
            BattleMon *mon = func_ov093_021b9bf0(party, i);
            if ((s32)GetBattleMonStat(mon, 0xf) < 100) {
                u32 exp = entries[i].exp;
                if (entry->boosted) {
                    message = 0x2b;
                } else {
                    message = 0x2a;
                }
                monId = GetMonID(mon);
                func_ov093_021cd64c(flow->queue, 0x5a, (u16)message, monId, exp, 0xffff0000);
                func_ov093_021cd4b0(flow->queue, 0x3e, monId, entries->hp, entries->attack, entries->defense,
                                    entries->speed, entries->spAttack, entries->spDefense);
                remaining = exp;
                while (func_ov093_021d69b8(mon, &remaining, &flow->levelUp)) {
                }
                func_ov093_021cd4b0(flow->queue, 0x45, monId, exp);
                func_ov093_021ba1cc(flow->mainModule, 0x1b, exp);
                result = TRUE;
            }
        }
    }
    return result;
}

BOOL ServerControl_HideTurnCancel(BtlServerFlow *flow, BattleMon *mon, u32 flag) {
    if (GetAdditionalConditionFlag(mon, flag)) {
        u8 monId = GetMonID(mon);
        scPut_ResetContFlag(flow, mon, flag);
        if (CheckCondition(mon, 0x1a)) {
            ServerControl_CureCondition(flow, mon, 0x1a, 0);
        }
        func_ov093_021cd4b0(flow->queue, 0x31, monId, 0);
        ActionOrder_ForceDone(flow, monId);
        return TRUE;
    }
    return FALSE;
}


#pragma thumb on

void func_ov093_021c5474(BtlFlowUnk1B50 *work) {
    work->unk220 = 0;
}

void ServerDisplay_AddEffectAtPosition(BtlServerFlow *flow, BattleMon *mon, u32 effect) {
    u8 pos = GetBattlePos(&flow->posPoke, GetMonID(mon));
    if (pos != 6) {
        func_ov093_021cd4b0(flow->queue, 0x4d, pos, effect);
    }
}




void func_ov093_021c54bc(BtlServerFlow *flow, BattleMon *mon) {
    ServerDisplay_SkyDropTargetAppear(flow, mon, 0x350);
}

void func_ov093_021c54c8(BtlServerFlow *flow, BattleMon *mon) {
    ServerDisplay_AddEffectAtPosition(flow, mon, 0x25b);
    func_ov093_021cd64c(flow->queue, 0x5b, 0x15c, GetMonID(mon), 0xffff0000);
}

void func_ov093_021c54fc(BtlServerFlow *flow, BattleMon *mon) {
    u8 move = GetDisabledMove(mon, 7);
    ServerDisplay_AddEffectAtPosition(flow, mon, 0x25c);
    func_ov093_021cd64c(flow->queue, 0x5b, 0x14d, GetMonID(mon), move, 0xffff0000);
}

void func_ov093_021c5540(BtlServerFlow *flow, BattleMon *mon, u32 damage) {
    u8 monId = GetMonID(mon);
    func_ov093_021cd4b0(flow->queue, 0x36, monId);
    ServerDisplay_SimpleHP(flow, mon, -damage, TRUE);
    func_ov093_021cd64c(flow->queue, 0x5a, 0x50, monId, 0xffff0000);
}

void func_ov093_021c557c(BtlServerFlow *flow, BattleMon *mon, u32 status, BOOL flag) {
    u8 monId = GetMonID(mon);

    CureCondition(mon);
    func_ov093_021cd4b0(flow->queue, 0x10, monId);
    func_ov093_021cd4b0(flow->queue, 0x35, monId, 0);
    if (flag) {
        switch (status) {
        case 2:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x138, monId, 0xffff0000);
            break;
        case 3:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x126, monId, 0xffff0000);
            break;
        default:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x156, monId, 0xffff0000);
            break;
        }
    }
}

void func_ov093_021c5604(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    func_ov093_021cd4b0(flow->queue, 0x59, GetMonID(mon), move);
}

void ServerControl_CureCondition(BtlServerFlow *flow, BattleMon *mon, s32 condition, BattleCondition *prev) {
    u8 monId;

    if (condition != 0) {
        monId = GetMonID(mon);
        if (prev != NULL) {
            *prev = GetConditionContinuationParam(mon, condition);
        }
        if (condition < 6) {
            CureCondition(mon);
            func_ov093_021cd4b0(flow->queue, 0x10, monId);
            if (GetBattlePos(&flow->posPoke, monId) != 6) {
                func_ov093_021cd4b0(flow->queue, 0x35, monId, 0);
            }
        } else {
            CureMoveCondition(mon, condition);
            func_ov093_021cd4b0(flow->queue, 0x11, monId, (u16)condition);
        }
    }
}



s32 ConvertConditionCode(BattleMon *mon, s32 *condition) {
    s32 result = 0;
    u32 status = GetBattleMonStatus(mon);

    switch (*condition) {
    case 0:
        break;
    case 0x24:
        if (status != 0) {
            result = status;
        }
        *condition = 0;
        break;
    case 0x25:
        if (status != 0) {
            result = status;
            *condition = 6;
        } else {
            if (CheckCondition(mon, 6)) {
                result = 6;
            }
            *condition = 0;
        }
        break;
    case 0x26: {
        s32 found = func_ov093_021d7e10(mon);
        if (found != 0) {
            return found;
        }
        *condition = result;
        return found;
    }
    default:
        if (CheckCondition(mon, *condition)) {
            result = *condition;
        }
        *condition = 0;
        break;
    }
    return result;
}


void func_ov093_021c5718(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    func_ov093_021cd64c(flow->queue, 0x5a, 0x47, 0xffff0000);
}

void func_ov093_021c572c(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    func_ov093_021cd64c(flow->queue, 0x5b, 0xd5, GetMonID(mon), 0xffff0000);
}

void func_ov093_021c5750(BtlServerFlow *flow, BattleMon *mon) {
    func_ov093_021cd64c(flow->queue, 0x5b, 0xd2, GetMonID(mon), 0xffff0000);
}

void func_ov093_021c5774(BtlServerFlow *flow, BattleMon *mon) {
    func_ov093_021cd64c(flow->queue, 0x5b, 0xd8, GetMonID(mon), 0xffff0000);
}

void func_ov093_021c5798(BtlServerFlow *flow, BtlFlowMoveParam *param, u32 count, u32 *effectiveness, BattleMon **mons,
                         u16 *damages, u8 *critical, BOOL multipleTargets) {
    u8 superEffective;
    u8 notVeryEffective;
    u32 kind;
    u32 i;

    notVeryEffective = 0;
    superEffective = 0;
    for (i = 0; i < count; i++) {
        s32 value;
        func_ov093_021c604c(flow, mons[i], damages[i]);
        value = effectiveness[i];
        if (value > 3) {
            superEffective++;
        }
        if (value < 3) {
            notVeryEffective++;
        }
    }
    if (superEffective) {
        kind = 2;
    } else if (notVeryEffective) {
        kind = 3;
    } else {
        kind = 1;
    }
    func_ov093_021cd4b0(flow->queue, 0x33, (u8)count, (u8)kind, param->move);
    for (i = 0; i < count; i++) {
        func_ov093_021cd63c(flow->queue, GetMonID(mons[i]));
    }
}

void func_ov093_021c5840(BtlServerFlow *flow, u32 count, u32 *effectiveness, BattleMon **mons, BOOL multipleTargets) {
    u8 monIds[3];
    u8 notVeryEffective;
    u8 superEffective;
    u32 i;

    notVeryEffective = 0;
    superEffective = 0;
    for (i = 0; i < count; i++) {
        s32 value = effectiveness[i];
        if (value > 3) {
            superEffective++;
        }
        if (value < 3) {
            notVeryEffective++;
        }
    }
    if (multipleTargets) {
        if (superEffective) {
            u8 n = 0;
            for (i = 0; i < count; i++) {
                if ((s32)effectiveness[i] > 3) {
                    monIds[n++] = GetMonID(mons[i]);
                }
            }
            switch (superEffective) {
            case 1:
                func_ov093_021cd64c(flow->queue, 0x5b, 6, monIds[0], 0xffff0000);
                break;
            case 2:
                func_ov093_021cd64c(flow->queue, 0x5b, 9, monIds[0], monIds[1], 0xffff0000);
                break;
            case 3:
                func_ov093_021cd64c(flow->queue, 0x5b, 0xc, monIds[0], monIds[1], monIds[2], 0xffff0000);
                break;
            }
        }
        if (notVeryEffective) {
            u8 n;
            for (i = 0, n = 0; i < count; i++) {
                if ((s32)effectiveness[i] < 3) {
                    monIds[n++] = GetMonID(mons[i]);
                }
            }
            switch (notVeryEffective) {
            case 1:
                func_ov093_021cd64c(flow->queue, 0x5b, 0xf, monIds[0], 0xffff0000);
                break;
            case 2:
                func_ov093_021cd64c(flow->queue, 0x5b, 0x12, monIds[0], monIds[1], 0xffff0000);
                break;
            case 3:
                func_ov093_021cd64c(flow->queue, 0x5b, 0x15, monIds[0], monIds[1], monIds[2], 0xffff0000);
                break;
            }
        }
    } else if (superEffective) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x4e, 0xffff0000);
    } else if (notVeryEffective) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x4f, 0xffff0000);
    }
}

void func_ov093_021c59c4(BtlServerFlow *flow, u32 count, BattleMon **mons, u8 *critical, BOOL multipleTargets) {
    u32 i;

    for (i = 0; i < count; i++) {
        if (critical[i]) {
            if (multipleTargets) {
                func_ov093_021cd64c(flow->queue, 0x5b, 0x180, GetMonID(mons[i]), 0xffff0000);
            } else {
                func_ov093_021cd64c(flow->queue, 0x5a, 0x51, 0xffff0000);
            }
        }
    }
}

// Function names from swan.
void ServerDisplay_AbilityPopupAdd(BtlServerFlow *handler, BattleMon *mon) {
    func_ov093_021cd4b0(handler->queue, 0x57, GetMonID(mon));
}

void ServerDisplay_AbilityPopupRemove(BtlServerFlow *handler, BattleMon *mon) {
    func_ov093_021cd4b0(handler->queue, 0x58, GetMonID(mon));
}

void func_ov093_021c5a4c(BtlServerFlow *flow, BattleMon *mon, u32 stat, s32 change) {
    u8 monId = GetMonID(mon);
    if (change > 0) {
        func_ov093_021cd64c(flow->queue, 0x5b, 0x99, monId, stat, 0xffff0000);
    } else {
        func_ov093_021cd64c(flow->queue, 0x5b, 0xae, monId, stat, 0xffff0000);
    }
}

void func_ov093_021c5a8c(BtlServerFlow *flow, BattleMon *mon, u32 stat, s32 change, u16 context, BOOL flag) {
    u8 monId = GetMonID(mon);

    if (change > 0) {
        u32 actual = func_ov093_021d5dc0(mon, stat, change);
        func_ov093_021cd4b0(flow->queue, 9, monId, (u8)stat, (u8)actual);
        func_ov093_021cd4b0(flow->queue, 0x37, monId, (u8)stat, (u8)actual);
        if (flag) {
            if (context == 0) {
                func_ov093_021cd64c(flow->queue, 0x5b, 0x1b, monId, stat, actual, 0xffff0000);
            } else {
                func_ov093_021cd64c(flow->queue, 0x5b, 0x3aa, monId, context, stat, actual, 0xffff0000);
            }
        }
    } else {
        u32 actual = func_ov093_021d5e38(mon, stat, -change);
        func_ov093_021cd4b0(flow->queue, 0xa, monId, (u8)stat, (u8)actual);
        func_ov093_021cd4b0(flow->queue, 0x38, monId, (u8)stat, (u8)actual);
        func_ov093_021cd64c(flow->queue, 0x5b, 0x5a, monId, stat, actual, 0xffff0000);
    }
}

void ServerDisplay_SimpleHP(BtlServerFlow *flow, BattleMon *mon, s32 amount, BOOL show) {
    u8 monId;

    monId = GetMonID(mon);
    if (amount > 0) {
        HPAdd(mon, (u16)amount);
        func_ov093_021cd4b0(flow->queue, 2, monId, (u16)amount);
    } else if (amount < 0) {
        func_ov093_021c604c(flow, mon, -amount);
    }
    if (show) {
        if (GetBattlePos(&flow->posPoke, monId) != 6) {
            func_ov093_021cd4b0(flow->queue, 0x41, monId);
        }
    }
}



void ServerDisplay_FaintPokemon(BtlServerFlow *flow, BattleMon *mon, u32 flag) {
    GetBattleMonStat(mon, 0xd);
    u8 monId = GetMonID(mon);

    HPZero(mon);
    func_ov093_021cd4b0(flow->queue, 3, monId);
    func_ov093_021cd4b0(flow->queue, 0x43, monId, flag);
}

void ServerDisplay_AddCondition(BtlServerFlow *flow, BattleMon *mon, s32 condition, BattleCondition value) {
    u8 monId = GetMonID(mon);

    SetMoveCondition(mon, condition, value);
    func_ov093_021cd4b0(flow->queue, 0xf, monId, (u8)condition, value);
    // The major status conditions show an icon, and a bad poisoning has its own after the last of them
    if (condition < CONDITION_CONFUSION) {
        if (condition == CONDITION_POISON && Condition_IsBadlyPoisoned(value)) {
            condition = CONDITION_POISON + 1;
        }
        func_ov093_021cd4b0(flow->queue, 0x35, monId, condition);
    }
}

void AddConditionCheckFailStandard(BtlServerFlow *flow, BattleMon *mon, u32 cause, u32 condition) {
    u8 monId = GetMonID(mon);

    switch (cause) {
    case 1:
        switch (condition) {
        case CONDITION_POISON:
            func_ov093_021cd64c(flow->queue, 0x5b, 0xf9, monId, 0xffff0000);
            break;
        case CONDITION_BURN:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x10b, monId, 0xffff0000);
            break;
        case CONDITION_PARALYSIS:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x11a, monId, 0xffff0000);
            break;
        case CONDITION_SLEEP:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x13b, monId, 0xffff0000);
            break;
        case CONDITION_FREEZE:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x129, monId, 0xffff0000);
            break;
        case CONDITION_CONFUSION:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x162, monId, 0xffff0000);
            break;
        case CONDITION_HEAL_BLOCK:
            func_ov093_021cd64c(flow->queue, 0x5b, 0x18, monId, 0xffff0000);
            break;
        case CONDITION_PERISH_SONG:
            return;
        case CONDITION_NONE:
        default:
            func_ov093_021cd64c(flow->queue, 0x5a, 0x47, 0xffff0000);
            break;
        }
        flow->unk78A_4 = 1;
        break;
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5b, 0xd2, monId, 0xffff0000);
        flow->unk78A_4 = 1;
        break;
    default:
        func_ov093_021cd64c(flow->queue, 0x5a, 0x47, 0xffff0000);
        flow->unk78A_4 = 1;
        break;
    }
}

void ServerDisplay_SkyDropTargetAppear(BtlServerFlow *flow, BattleMon *mon, u16 message) {
    func_ov093_021cd64c(flow->queue, 0x5b, message, GetMonID(mon), 0xffff0000);
}

void ServerDisplay_StandardMessage(BtlServerFlow *flow, u16 message, u32 count, const u32 *args) {
    switch (count) {
    default:
        func_ov093_021cd64c(flow->queue, 0x5a, message, 0xffff0000);
        break;
    case 1:
        func_ov093_021cd64c(flow->queue, 0x5a, message, args[0], 0xffff0000);
        break;
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5a, message, args[0], args[1], 0xffff0000);
        break;
    case 3:
        func_ov093_021cd64c(flow->queue, 0x5a, message, args[0], args[1], args[2], 0xffff0000);
        break;
    case 4:
        func_ov093_021cd64c(flow->queue, 0x5a, message, args[0], args[1], args[2], args[3], 0xffff0000);
        break;
    }
}

void ServerDisplay_StandardMessageEx(BtlServerFlow *flow, u16 message, u16 soundEffect, u32 count, const u32 *args) {
    switch (count) {
    default:
        func_ov093_021cd64c(flow->queue, 0x5c, message, soundEffect, 0xffff0000);
        break;
    case 1:
        func_ov093_021cd64c(flow->queue, 0x5c, message, soundEffect, args[0], 0xffff0000);
        break;
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5c, message, soundEffect, args[0], args[1], 0xffff0000);
        break;
    case 3:
        func_ov093_021cd64c(flow->queue, 0x5c, message, soundEffect, args[0], args[1], args[2], 0xffff0000);
        break;
    case 4:
        func_ov093_021cd64c(flow->queue, 0x5c, message, soundEffect, args[0], args[1], args[2], args[3], 0xffff0000);
        break;
    }
}

void ServerDisplay_SetMessage(BtlServerFlow *flow, u16 message, u32 count, const u32 *args) {
    switch (count) {
    default:
        func_ov093_021cd64c(flow->queue, 0x5b, message, 0xffff0000);
        break;
    case 1:
        func_ov093_021cd64c(flow->queue, 0x5b, message, args[0], 0xffff0000);
        break;
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5b, message, args[0], args[1], 0xffff0000);
        break;
    case 3:
        func_ov093_021cd64c(flow->queue, 0x5b, message, args[0], args[1], args[2], 0xffff0000);
        break;
    case 4:
        func_ov093_021cd64c(flow->queue, 0x5b, message, args[0], args[1], args[2], args[3], 0xffff0000);
        break;
    }
}

void ServerDisplay_SetMessageEx(BtlServerFlow *flow, u16 message, u16 soundEffect, u32 count, const u32 *args) {
    switch (count) {
    default:
        func_ov093_021cd64c(flow->queue, 0x5d, message, soundEffect, 0xffff0000);
        break;
    case 1:
        func_ov093_021cd64c(flow->queue, 0x5d, message, soundEffect, args[0], 0xffff0000);
        break;
    case 2:
        func_ov093_021cd64c(flow->queue, 0x5d, message, soundEffect, args[0], args[1], 0xffff0000);
        break;
    case 3:
        func_ov093_021cd64c(flow->queue, 0x5d, message, soundEffect, args[0], args[1], args[2], 0xffff0000);
        break;
    case 4:
        func_ov093_021cd64c(flow->queue, 0x5d, message, soundEffect, args[0], args[1], args[2], args[3], 0xffff0000);
        break;
    }
}


void func_ov093_021c604c(BtlServerFlow *flow, BattleMon *mon, u32 damage) {
    if (!ReturnZero(flow->mainModule, 4)) {
        func_ov093_021d5f90(mon, damage);
        func_ov093_021cd4b0(flow->queue, 1, GetMonID(mon), (u16)damage);
    }
}

void ServerDisplay_RecoverPP(BtlServerFlow *flow, BattleMon *mon, u8 slot, u8 amount, BOOL original) {
    u8 monId = GetMonID(mon);

    if (original) {
        Move_IncrementPP_Org(mon, slot, amount);
        func_ov093_021cd4b0(flow->queue, 8, monId, slot, amount);
    } else {
        Move_IncrementPP(mon, slot, amount);
        func_ov093_021cd4b0(flow->queue, 7, monId, slot, amount);
    }
}

void ServerDisplay_UseHeldItem(BtlServerFlow *flow, BattleMon *mon) {
    func_ov093_021cd4b0(flow->queue, 0x42, GetMonID(mon));
}

// Function names from swan.
void scPut_SetContFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag) {
    func_ov093_021d5fe4(mon, flag);
    func_ov093_021cd4b0(handler->queue, 0x19, GetMonID(mon), flag);
}

void scPut_ResetContFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag) {
    func_ov093_021d6008(mon, flag);
    func_ov093_021cd4b0(handler->queue, 0x1a, GetMonID(mon), flag);
}

void ServerDisplay_SetTurnFlag(BtlServerFlow *handler, BattleMon *mon, u32 flag) {
    func_ov093_021d5fc0(mon, flag);
    func_ov093_021cd4b0(handler->queue, 0x1b, GetMonID(mon), flag);
}

void func_ov093_021c6158(BtlServerFlow *flow, BtlFlowClientList *list) {
    u32 i;
    BattleParty *party;

    for (i = 0; i < list->count; i++) {
        party = GetPartyData(flow->pokeCon, list->clientIds[i]);
        func_ov093_021cd4b0(flow->queue, 0x2a, list->clientIds[i]);
        func_ov093_021b9d10(flow->mainModule, party, list->clientIds[i]);
    }
}

u32 ServerEvent_InterruptSwitch(BtlServerFlow *flow, BattleMon *mon) {
    u32 i;
    u16 move;
    ActionOrderEntry *entry;

    // Pursuit strikes a mon that is switching out before it leaves
    for (i = 0; i < flow->actionOrderCount; i++) {
        if (flow->actionOrder[i].mon != mon && !flow->actionOrder[i].done) {
            entry = &flow->actionOrder[i];
            move = BattleAction_GetMove(&entry->action);
            if (move == MOVE_PURSUIT
                && MoveEvent_AddItem(flow->actionOrder[i].mon, move, GetBattleMonStat(flow->actionOrder[i].mon, 0xc))) {
                flow->actionOrder[i].interrupting = TRUE;
            }
        }
    }
    flow->interruptCount = 0;
    BattleEventVar_Push(0x2c18);
    BattleEventVar_SetConstValue(6, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x53);
    BattleEventVar_Pop(0x2c1b);
    for (i = 0; i < flow->actionOrderCount; i++) {
        if (flow->actionOrder[i].interrupting) {
            entry = &flow->actionOrder[i];
            move = BattleAction_GetMove(&entry->action);
            if (move == MOVE_PURSUIT) {
                RemoveForce(flow->actionOrder[i].mon, move);
            }
            flow->actionOrder[i].interrupting = FALSE;
        }
    }
    return flow->interruptCount;
}

BOOL func_ov093_021c62d8(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target, BtlFlowCalledMove *called) {
    BOOL failed;

    BattleEventVar_Push(0x2c3f);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0xe, target);
    BattleEventVar_SetValue(0x12, 0);
    BattleEventVar_SetValue(0xd, 6);
    BattleEventVar_SetRewriteOnceValue(0x41, 0);
    BattleEvent_CallHandlers(flow, 0x18);
    called->move = BattleEventVar_GetValue(0x12);
    called->target = BattleEventVar_GetValue(0xd);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2c4c);
    return !failed;
}

u8 func_ov093_021c6350(BtlServerFlow *flow, BattleMon *mon) {
    u8 result;

    BattleEventVar_Push(0x2c5d);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x11, 1);
    BattleEvent_CallHandlers(flow, 0xf);
    result = BattleEventVar_GetValue(0x11);
    BattleEventVar_Pop(0x2c62);
    return result;
}

void func_ov093_021c6394(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    BattleEventVar_Push(0x2c72);
    BattleEventVar_SetValue(2, GetMonID(mon));
    BattleEventVar_SetValue(0x12, move);
    BattleEvent_CallHandlers(flow, 0x15);
    BattleEventVar_Pop(0x2c76);
}

// A confused mon hurts itself, with a typeless 40 power physical attack on itself
u16 func_ov093_021c63d0(BtlServerFlow *flow, BattleMon *mon) {
    u32 attack;
    u32 defense;
    u32 cause;
    u16 damage;

    attack = GetBattleMonStat(mon, 8);
    defense = GetBattleMonStat(mon, 9);
    damage = CalcBaseDamage(40, attack, GetBattleMonStat(mon, 0xf), defense);
    damage = damage * (100 - BattleRandom(16)) / 100;
    if (damage == 0) {
        damage = 1;
    }
    cause = func_ov093_021c1678(flow, mon, mon, 0, &damage);
    func_ov093_021c5540(flow, mon, damage);
    if (cause) {
        func_ov093_021c16f8(flow, mon, cause);
    }
    return damage;
}

BOOL func_ov093_021c6458(BtlServerFlow *flow, BattleMon *mon, u16 move, u16 actualMove, BattleHandlerString *string) {
    BOOL result;

    BattleHandler_StrClear(string);
    BattleEventVar_Push(0x2cb0);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, actualMove);
    BattleEventVar_SetConstValue(0x3f, (intptr_t)string);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x19);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x2cb7);
    return result;
}

void ServerEvent_GetMoveParam(BtlServerFlow *flow, u16 move, BattleMon *mon, BtlFlowMoveParam *param) {
    BattleEventVar_Push(0x2cc8);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x16, PML_MoveGetType(move));
    BattleEventVar_SetValue(0x1c, GetPokeType(mon));
    BattleEventVar_SetValue(0x1a, PML_MoveGetCategory(move));
    BattleEventVar_SetValue(0x1b, PML_MoveGetParam(move, 0x1b));
    BattleEventVar_SetRewriteOnceValue(0x4b, FALSE);
    BattleEvent_CallHandlers(flow, 0x28);
    param->move = move;
    param->originalMove = move;
    param->type = BattleEventVar_GetValue(0x16);
    param->userType = BattleEventVar_GetValue(0x1c);
    param->category = BattleEventVar_GetValue(0x1a);
    param->targetType = BattleEventVar_GetValue(0x1b);
    param->flags.raw = 0;
    if (BattleEventVar_GetValue(0x4b)) {
        param->type = TYPE_NULL;
    }
    BattleEventVar_Pop(0x2ce5);
}

void func_ov093_021c6564(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 cause) {
    BattleEventVar_Push(0x2cf4);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetConstValue(0x22, cause);
    BattleEvent_CallHandlers(flow, 0x21);
    BattleEventVar_Pop(0x2cf9);
}

u32 func_ov093_021c65a8(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets) {
    u32 count;
    u32 i;
    u32 result;
    u16 newMove;

    count = data_ov095_0689cd39(targets);
    BattleEventVar_Push(0x2d0d);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetConstValue(5, count);
    for (i = 0; i < count; i++) {
        BattleEventVar_SetConstValue(6 + i, GetMonID(data_ov095_0689cc71(targets, i)));
    }
    BattleEventVar_SetRewriteOnceValue(0x3e, 0);
    BattleEventVar_SetRewriteOnceValue(0x13, 0);
    BattleEvent_CallHandlers(flow, 0x24);
    result = BattleEventVar_GetValue(0x3e);
    newMove = BattleEventVar_GetValue(0x13);
    BattleEventVar_Pop(0x2d1e);
    if (newMove) {
        flow->moveEffect->unk00 = newMove;
    }
    return result;
}







BOOL func_ov093_021c6668(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move) {
    u32 result;

    BattleEventVar_Push(0x2d36);
    BattleEventVar_SetValue(3, GetMonID(mon));
    BattleEventVar_SetValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x2e);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x2d3b);
    return result;
}

BOOL func_ov093_021c66ac(BtlServerFlow *flow, BattleMon *mon, void *targets) {
    BattleMon *target;
    u8 targetId;
    BOOL result;

    target = data_ov095_0689cc71(targets, 0);
    BattleEventVar_Push(0x2d4f);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    if (target != NULL) {
        targetId = GetMonID(target);
    } else {
        targetId = 0x1f;
    }
    BattleEventVar_SetConstValue(4, targetId);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x93);
    result = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2d55);
    return result;
}




BOOL func_ov093_021c6720(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    BOOL result;

    BattleEventVar_Push(0x2d66);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, move);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x94);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x2d6c);
    return result;
}

BOOL func_ov093_021c676c(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move, u8 *monId, BOOL *failed) {
    u32 count;
    u32 i;
    BOOL stopped;

    count = data_ov095_0689cd39(targets);
    BattleEventVar_Push(0x2d80);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(5, count);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            BattleEventVar_SetConstValue(6 + i, GetMonID(data_ov095_0689cc71(targets, i)));
        }
    } else {
        BattleEventVar_SetConstValue(6, 0x1f);
    }
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEventVar_SetRewriteOnceValue(0x4f, FALSE);
    BattleEvent_CallHandlers(flow, 0x95);
    if (BattleEventVar_GetValue(0x51)) {
        *monId = BattleEventVar_GetValue(6);
    } else {
        *monId = 0x1f;
    }
    *failed = BattleEventVar_GetValue(0x4f);
    stopped = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2d9d);
    return stopped ? FALSE : TRUE;
}







void func_ov093_021c6848(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x2dac);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x96);
    BattleEventVar_Pop(0x2daf);
}

void func_ov093_021c6878(BtlServerFlow *flow, BattleMon *mon, u16 move) {
    BattleEventVar_Push(0x2dbd);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x97);
    BattleEventVar_Pop(0x2dc0);
}

u32 func_ov093_021c68a8(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move) {
    u32 count;
    u32 i;
    u8 failed;

    count = data_ov095_0689cd39(targets);
    BattleEventVar_Push(0x2dd3);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEventVar_SetConstValue(5, count);
    for (i = 0; i < count; i++) {
        BattleEventVar_SetConstValue(6 + i, GetMonID(data_ov095_0689cc71(targets, i)));
    }
    BattleEvent_CallHandlers(flow, 0x98);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2dde);
    return !failed;
}







BOOL func_ov093_021c6948(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move) {
    BOOL result;
    u32 status;

    result = FALSE;
    status = func_ov093_021d5c08(target);
    if (status != 0x10) {
        BattleEventVar_Push(0x2df5);
        BattleEventVar_SetConstValue(3, GetMonID(mon));
        BattleEventVar_SetConstValue(4, GetMonID(target));
        BattleEventVar_SetConstValue(0x21, status);
        BattleEventVar_SetRewriteOnceValue(0x42, TRUE);
        BattleEvent_CallHandlers(flow, 0x99);
        if (BattleEventVar_GetValue(0x42)) {
            result = TRUE;
        }
        BattleEventVar_Pop(0x2dff);
    }
    return result;
}

BOOL func_ov093_021c69b8(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move) {
    BOOL result;

    result = PML_MoveIsAlwaysHit(move);
    if (!result) {
        BattleEventVar_Push(0x2e15);
        BattleEventVar_SetConstValue(3, GetMonID(mon));
        BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
        BattleEvent_CallHandlers(flow, 0x32);
        result = BattleEventVar_GetValue(0x51);
        BattleEventVar_Pop(0x2e1a);
    }
    return result;
}

u32 func_ov093_021c6a04(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param) {
    u32 accuracy;
    u32 ratio;

    accuracy = PML_MoveGetParam(param->move, 4);
    BattleEventVar_Push(0x2e2f);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x1a, param->category);
    BattleEventVar_SetRewriteOnceValue(0x2b, accuracy);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEvent_CallHandlers(flow, 0x34);
    accuracy = BattleEventVar_GetValue(0x2b);
    ratio = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x2e39);
    return fixed_round(accuracy, ratio);
}

// Whether a one-hit KO move hits: never a mon of a higher level, and more likely for each level the attacker has
// over the target
BOOL func_ov093_021c6a9c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move) {
    u8 attackerLevel;
    u8 targetLevel;
    u8 accuracy;
    BOOL failed;
    BOOL result;

    attackerLevel = GetBattleMonStat(attacker, 0xf);
    targetLevel = GetBattleMonStat(target, 0xf);
    if (attackerLevel < targetLevel) {
        return FALSE;
    }
    BattleEventVar_Push(0x2e5a);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    result = FALSE;
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x70);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2e60);
    if (failed) {
        return FALSE;
    }
    if (IsGuaranteedHit(flow, attacker, target)) {
        result = TRUE;
    } else {
        accuracy = PML_MoveGetParam(move, 4);
        accuracy += attackerLevel - targetLevel;
        if (BattleRandom(100) < accuracy) {
            result = TRUE;
        }
    }
    return result;
}

BOOL func_ov093_021c6b5c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param) {
    BOOL result;

    BattleEventVar_Push(0x2e83);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x30);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x2e8a);
    return result;
}

void func_ov093_021c6bb8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target) {
    BattleEventVar_Push(0x2e98);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEvent_CallHandlers(flow, 0x31);
    BattleEventVar_Pop(0x2e9c);
}

// Whether a move lands a critical hit
BOOL func_ov093_021c6bf8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move) {
    u16 stage;
    BOOL critical;
    s32 value;

    stage = PML_MoveGetParam(move, 7);
    stage += func_ov093_021d5f14(attacker);
    BattleEventVar_Push(0x2eb0);
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetValue(0x2c, stage);
    critical = FALSE;
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x36);
    if (!BattleEventVar_GetValue(0x41)) {
        if (PML_MoveIsAlwaysCrit(move)) {
            critical = TRUE;
        } else {
            value = BattleEventVar_GetValue(0x2c);
            if (value > 4) {
                value = 4;
            }
            stage = value;
            critical = func_ov093_021d7930(stage);
        }
        if (ReturnZero(flow->mainModule, 3)) {
            critical = TRUE;
        }
    }
    BattleEventVar_Pop(0x2ec7);
    return critical;
}

s32 ServerEvent_CalcRecoil(BtlServerFlow *flow, BattleMon *mon, u16 move, s32 damage, BOOL *forced) {
    u8 monId;
    u8 ratio;
    u8 bonus;
    u8 failed;
    u8 force;
    u32 kind;

    monId = GetMonID(mon);
    // A share of the damage dealt, or of the user's max HP
    ratio = PML_MoveGetParam(move, 0x1e);
    kind = 0;
    *forced = FALSE;
    if (ratio != 0) {
        kind = 1;
    } else {
        ratio = PML_MoveGetParam(move, 0x1f);
        if (ratio != 0) {
            kind = 2;
        }
    }
    if (PML_MoveGetQuality(move) == 8) {
        return 0;
    }
    BattleEventVar_Push(0x2ef3);
    BattleEventVar_SetConstValue(3, monId);
    BattleEventVar_SetValue(0x35, ratio);
    BattleEventVar_SetValue(0x36, 0);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x50);
    ratio = BattleEventVar_GetValue(0x35);
    failed = BattleEventVar_GetValue(0x41);
    force = BattleEventVar_GetValue(0x51);
    bonus = BattleEventVar_GetValue(0x36);
    BattleEventVar_Pop(0x2f00);
    if (!force && failed == TRUE) {
        return 0;
    }
    ratio += bonus;
    if (ratio != 0) {
        if (kind == 2) {
            damage = MultiplyValueByRatio(GetBattleMonStat(mon, 0xe), ratio);
            if (damage < 1) {
                damage = 1;
            }
        } else if (damage != 0) {
            damage = MultiplyValueByRatio(damage, ratio);
            if (damage < 1) {
                damage = 1;
            }
        }
        *forced = force;
        return damage;
    }
    return 0;
}

void ServerEvent_EquipItem(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x2f2d);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x72);
    BattleEventVar_Pop(0x2f30);
}

void ServerEvent_EquipTempItem(BtlServerFlow *flow, BattleMon *mon, u8 monIndex) {
    BattleEventVar_Push(0x2f3d);
    BattleEventVar_SetValue(2, GetMonID(mon));
    BattleEventVar_SetValue(3, monIndex);
    BattleEvent_ForceCallHandlers(flow, 0x73);
    BattleEventVar_Pop(0x2f41);
}

// The effectiveness of a move against a target's types, counting Ground moves against a floating target
u32 func_ov093_021c6e3c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param,
                        BOOL flag) {
    BOOL floating = FALSE;
    PokeTypePair types;
    u32 effectiveness;

    if (param->type == TYPE_NULL) {
        return TYPE_EFFECTIVENESS_NORMAL;
    }
    types = GetPokeType(target);
    effectiveness = func_ov093_021c7038(flow, attacker, target, param->type, PokeTypePair_GetType1(types));
    if (!PokeTypePair_IsMonotype(types)) {
        effectiveness = GetTypeEffectivenessMultiplier(
            effectiveness, func_ov093_021c7038(flow, attacker, target, param->type, PokeTypePair_GetType2(types)));
    }
    if (effectiveness != TYPE_EFFECTIVENESS_IMMUNE) {
        if (param->type != TYPE_GROUND) {
            return effectiveness;
        }
        floating = func_ov093_021c6f0c(flow, target, FALSE);
        if (!floating) {
            return effectiveness;
        }
        effectiveness = TYPE_EFFECTIVENESS_IMMUNE;
    } else if (param->type == TYPE_GROUND && !func_ov093_021c6f0c(flow, target, TRUE)) {
        return TYPE_EFFECTIVENESS_NORMAL;
    }
    if (effectiveness == TYPE_EFFECTIVENESS_IMMUNE && flag) {
        if (!floating) {
            func_ov093_021c5750(flow, target);
        } else {
            func_ov093_021c6fc0(flow, target);
        }
    }
    return effectiveness;
}

BOOL func_ov093_021c6f0c(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    if (!func_ov093_021ef948(FIELD_CONDITION_GRAVITY) && ServerEvent_CheckFloating(flow, mon, flag)) {
        return TRUE;
    }
    return FALSE;
}

BOOL ServerEvent_CheckFloating(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    u8 floating;
    u8 failed;

    floating = flag ? DoesMonHaveType(mon, TYPE_FLYING) : FALSE;
    BattleEventVar_Push(0x2fbc);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x51, floating);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x12);
    Condition_CheckFloating(flow, mon);
    floating = BattleEventVar_GetValue(0x51);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x2fc4);
    if (failed) {
        return FALSE;
    }
    return floating;
}




void func_ov093_021c6fc0(BtlServerFlow *flow, BattleMon *mon) {
    u32 state;

    state = PushState(&flow->actionState, 0x2fd6);
    func_ov093_021c7008(flow, mon);
    if (BattleHandler_Result(flow) != 2) {
        func_ov093_021c5750(flow, mon);
    }
    PopState(&flow->actionState, state, 0x2fdd);
}

void func_ov093_021c7008(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x2fe9);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x1b);
    BattleEventVar_Pop(0x2fec);
}

u32 func_ov093_021c7038(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u8 moveType, u8 defenseType) {
    BOOL ignoreImmunity;
    BOOL forceNormal;
    u32 effectiveness;

    BattleEventVar_Push(0x3002);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(target));
    BattleEventVar_SetConstValue(0x15, defenseType);
    BattleEventVar_SetConstValue(0x16, moveType);
    BattleEventVar_SetRewriteOnceValue(0x4b, FALSE);
    BattleEventVar_SetRewriteOnceValue(0x4c, FALSE);
    data_ov095_0689c541(flow, target);
    BattleEvent_CallHandlers(flow, 0x3e);
    ignoreImmunity = BattleEventVar_GetValue(0x4b);
    forceNormal = BattleEventVar_GetValue(0x4c);
    BattleEventVar_Pop(0x300f);
    if (forceNormal) {
        return TYPE_EFFECTIVENESS_NORMAL;
    }
    effectiveness = GetTypeEffectiveness(moveType, defenseType);
    if (effectiveness == TYPE_EFFECTIVENESS_IMMUNE && ignoreImmunity) {
        return TYPE_EFFECTIVENESS_NORMAL;
    }
    return effectiveness;
}



void ServerControl_ViewEffect(BtlServerFlow *flow, u16 effect, u8 pos1, u8 pos2, BOOL reserved, u32 reserve) {
    u32 posCount;

    if (pos2 != 6) {
        posCount = 2;
    } else {
        posCount = 1;
        if (pos1 == 6) {
            posCount = 0;
        }
    }
    if (reserved) {
        switch (posCount) {
        case 0:
            func_ov093_021cd568(flow->queue, (u16)reserve, 0x4c, effect);
            break;
        case 1:
            func_ov093_021cd568(flow->queue, (u16)reserve, 0x4d, pos1, effect);
            break;
        case 2:
            func_ov093_021cd568(flow->queue, (u16)reserve, 0x4e, pos1, pos2, effect);
            break;
        }
    } else {
        switch (posCount) {
        case 0:
            func_ov093_021cd4b0(flow->queue, 0x4c, effect);
            break;
        case 1:
            func_ov093_021cd4b0(flow->queue, 0x4d, pos1, effect);
            break;
        case 2:
            func_ov093_021cd4b0(flow->queue, 0x4e, pos1, pos2, effect);
            break;
        }
    }
}

BOOL ServerEvent_DecrementPP(BtlServerFlow *flow, BattleMon *mon, u8 moveIndex) {
    BOOL result;

    BattleEventVar_Push(0x306f);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x17, moveIndex);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x4f);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x3075);
    return result;
}

BOOL func_ov093_021c71e0(BtlServerFlow *flow, BattleMon *mon, u16 move, BtlFlowHitWork *hitWork) {
    u8 maxHits;
    u32 hits;

    maxHits = PML_MoveGetParam(move, 8);
    hitWork->unk01 = 0;
    hitWork->unk05 = 3;
    if (maxHits > 1) {
        hits = func_ov093_021d7ba4(maxHits);
        BattleEventVar_Push(0x3090);
        BattleEventVar_SetConstValue(3, GetMonID(mon));
        BattleEventVar_SetConstValue(0x29, maxHits);
        BattleEventVar_SetValue(0x2a, hits);
        BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
        BattleEventVar_SetRewriteOnceValue(0x42, FALSE);
        BattleEvent_CallHandlers(flow, 0x35);
        if (maxHits <= 5 && BattleEventVar_GetValue(0x51)) {
            hitWork->count = maxHits;
            hitWork->unk02 = FALSE;
        } else {
            hitWork->count = BattleEventVar_GetValue(0x2a);
            hitWork->unk02 = BattleEventVar_GetValue(0x42);
        }
        hitWork->unk03 = TRUE;
        BattleEventVar_Pop(0x30a7);
        return TRUE;
    }
    hitWork->count = 1;
    hitWork->unk02 = FALSE;
    hitWork->unk03 = FALSE;
    return FALSE;
}


u16 func_ov093_021c728c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender, BtlFlowMoveParam *param) {
    u16 power;
    u32 ratio;

    power = PML_MoveGetBasePower(param->move);
    BattleEventVar_Push(0x30c7);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(defender));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x1a, param->category);
    BattleEventVar_SetRewriteOnceValue(0x30, power);
    BattleEvent_CallHandlers(flow, 0x37);
    BattleEventVar_SetMulValue(0x31, 0x1000, 0x29, 0x200000);
    BattleEvent_CallHandlers(flow, 0x38);
    power = BattleEventVar_GetValue(0x30);
    ratio = BattleEventVar_GetValue(0x31);
    BattleEventVar_Pop(0x30d4);
    return fixed_round(power, ratio);
}


u32 func_ov093_021c7330(BtlServerFlow *flow, BattleMon *mon) {
    u32 result;

    BattleEventVar_Push(0x30e9);
    BattleEventVar_SetConstValue(4, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x51, TRUE);
    BattleEvent_CallHandlers(flow, 0x80);
    result = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x30ee);
    return result;
}

u16 func_ov093_021c7374(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender,
                        const BtlFlowMoveParam *param, BOOL critical) {
    u32 stat;
    u8 monId;
    u16 power;
    u32 mod;

    stat = PML_MoveGetCategory(param->move) == MOVE_CATEGORY_SPECIAL ? 10 : 8;
    BattleEventVar_Push(0x30fe);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(defender));
    BattleEventVar_SetValue(0x3b, 0x1f);
    BattleEventVar_SetValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x39);
    monId = BattleEventVar_GetValue(0x3b);
    if (monId != 0x1f) {
        attacker = GetPokeParam(flow->pokeCon, monId);
    }
    if (BattleEventVar_GetValue(0x51)) {
        power = RawBattleMonStat(attacker, stat);
    } else if (critical) {
        power = CritAtkDefLevel(attacker, stat);
    } else {
        power = GetBattleMonStat(attacker, stat);
    }
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x1a, param->category);
    BattleEventVar_SetValue(0x33, power);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEvent_CallHandlers(flow, 0x3b);
    power = BattleEventVar_GetValue(0x33);
    mod = BattleEventVar_GetValue(0x35);
    power = fixed_round(power, mod);
    BattleEventVar_Pop(0x3123);
    return power;
}

u16 func_ov093_021c7474(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender,
                        const BtlFlowMoveParam *param, BOOL critical) {
    u32 stat;
    u32 category;
    u8 raw;
    u16 defense;
    u32 ratio;

    stat = PML_MoveGetCategory(param->move) == MOVE_CATEGORY_SPECIAL ? 11 : 9;
    category = param->category;
    BattleEventVar_Push(0x3133);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(defender));
    BattleEventVar_SetConstValue(0x3c, stat);
    BattleEventVar_SetValue(0x3d, 0);
    BattleEventVar_SetRewriteOnceValue(0x51, FALSE);
    BattleEvent_CallHandlers(flow, 0x3a);
    if (BattleEventVar_GetValue(0x3d) & 1) {
        if (stat == 9) {
            stat = 11;
        } else {
            stat = 9;
        }
        if (stat == 9) {
            category = MOVE_CATEGORY_PHYSICAL;
        } else {
            category = MOVE_CATEGORY_SPECIAL;
        }
    }
    raw = BattleEventVar_GetValue(0x51);
    BattleEventVar_Pop(0x3142);
    if (raw) {
        defense = RawBattleMonStat(defender, stat);
    } else if (critical) {
        defense = CritAtkDefLevel(defender, stat);
    } else {
        defense = GetBattleMonStat(defender, stat);
    }
    if (ServerEvent_GetWeather(flow) == BTL_WEATHER_SANDSTORM && DoesMonHaveType(defender, TYPE_ROCK) && stat == 11) {
        defense = fixed_round(defense, 0x1800);
    }
    BattleEventVar_Push(0x3157);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(4, GetMonID(defender));
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x1a, category);
    BattleEventVar_SetValue(0x34, defense);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEvent_CallHandlers(flow, 0x3c);
    defense = BattleEventVar_GetValue(0x34);
    ratio = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x3162);
    return fixed_round(defense, ratio);
}



u32 func_ov093_021c75ec(BtlServerFlow *flow, BattleMon *mon, u32 type) {
    BOOL hasType;
    u32 result;
    s32 ratio;

    hasType = DoesMonHaveType(mon, type);
    ratio = 0x1000;
    BattleEventVar_Push(0x3178);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetRewriteOnceValue(0x51, hasType);
    BattleEvent_CallHandlers(flow, 0x40);
    result = BattleEventVar_GetValue(0x51);
    if (result != 0) {
        ratio = 0x1800;
    }
    BattleEventVar_SetConstValue(0x44, result);
    BattleEventVar_SetValue(0x35, ratio);
    BattleEvent_CallHandlers(flow, 0x41);
    result = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x3189);
    return result;
}

u32 func_ov093_021c7664(BtlServerFlow *flow, u16 move, BattleMon *attacker) {
    BOOL failed;
    BOOL doubled;
    u32 bonus;
    u32 chance;

    chance = PML_MoveGetParam(move, 0xa);
    BattleEventVar_Push(0x319d);
    BattleEventVar_SetConstValue(3, GetMonID(attacker));
    BattleEventVar_SetConstValue(0x25, chance);
    BattleEventVar_SetRewriteOnceValue(0x45, FALSE);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEventVar_SetValue(0x26, 0);
    BattleEvent_CallHandlers(flow, 0x6c);
    failed = BattleEventVar_GetValue(0x41);
    doubled = BattleEventVar_GetValue(0x45);
    bonus = BattleEventVar_GetValue(0x26);
    BattleEventVar_Pop(0x31a8);
    if (failed && chance != 0) {
        return 0;
    }
    if (chance == 0) {
        chance += bonus;
    }
    if (doubled) {
        chance *= 2;
    }
    return chance;
}


BOOL func_ov093_021c76f4(BtlServerFlow *flow, BattleMon *mon, s32 param3) {
    u32 value;
    u32 failed;
    BOOL result;

    BattleEventVar_Push(0x31c9);
    BattleEventVar_SetConstValue(4, GetMonID(mon));
    BattleEventVar_SetValue(0x26, param3);
    result = FALSE;
    BattleEventVar_SetRewriteOnceValue(0x41, result);
    BattleEvent_CallHandlers(flow, 0x6d);
    value = BattleEventVar_GetValue(0x26);
    failed = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x31d1);
    if (failed) {
        return result;
    }
    if (value != 0) {
        if (func_ov093_021c82b8(flow, result)) {
            return TRUE;
        }
        if (BattleRandom(100) < (u8)value) {
            result = TRUE;
        }
        return result;
    }
    return result;
}



void func_ov093_021c7780(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x31ea);
    BattleEventVar_SetValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x6e);
    BattleEventVar_Pop(0x31ed);
}

BOOL func_ov093_021c77b0(BtlServerFlow *flow, BattleMon *mon, u32 param3, u8 param4, s32 param5, u32 param6) {
    u32 result;

    BattleEventVar_Push(0x3201);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(3, param4);
    BattleEventVar_SetConstValue(0x1f, param3);
    BattleEventVar_SetConstValue(0x20, param5);
    BattleEventVar_SetConstValue(0x19, param6);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x5b);
    result = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x320a);
    return result == 0;
}

void func_ov093_021c7820(BtlServerFlow *flow, BattleMon *mon, u32 param3) {
    BattleEventVar_Push(0x321a);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x19, param3);
    BattleEvent_CallHandlers(flow, 0x5c);
    BattleEventVar_Pop(0x321e);
}

void func_ov093_021c785c(BtlServerFlow *flow, u8 param2, BattleMon *mon, u32 param4, s32 param5) {
    BattleEventVar_Push(0x322e);
    BattleEventVar_SetConstValue(3, param2);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x1f, param4);
    BattleEventVar_SetConstValue(0x20, param5);
    BattleEvent_CallHandlers(flow, 0x5d);
    BattleEventVar_Pop(0x3234);
}

void func_ov093_021c78a8(BtlServerFlow *flow, BattleMon *mon, s32 param3, s32 param4, s32 param5) {
    BattleEventVar_Push(0x3244);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x12, param3);
    BattleEventVar_SetConstValue(0x1f, param4);
    BattleEventVar_SetConstValue(0x20, param5);
    BattleEvent_CallHandlers(flow, 0x5e);
    BattleEventVar_Pop(0x324a);
}

u16 func_ov093_021c78f4(BtlServerFlow *flow, BattleMon *mon, BattleMon *source, u16 amount) {
    u8 sourceId;
    u32 ratio;

    sourceId = source != NULL ? GetMonID(source) : 0x1f;
    BattleEventVar_Push(0x325e);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(4, sourceId);
    BattleEventVar_SetMulValue(0x35, 0x1000, 0x19a, 0x20000);
    BattleEventVar_SetValue(0x20, amount);
    BattleEvent_CallHandlers(flow, 0x8c);
    amount = BattleEventVar_GetValue(0x20);
    ratio = BattleEventVar_GetValue(0x35);
    if (amount != 0) {
        amount = GetRatioOverZero(amount, ratio);
    }
    BattleEventVar_RewriteValue(0x20, amount);
    BattleEvent_CallHandlers(flow, 0x8d);
    amount = BattleEventVar_GetValue(0x20);
    BattleEventVar_Pop(0x326f);
    return amount;
}


void func_ov093_021c7998(BtlServerFlow *flow, s32 param2) {
    u32 result;

    BattleEventVar_Push(0x3282);
    BattleEventVar_SetConstValue(0x39, param2);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x7d);
    result = BattleEventVar_GetValue(0x41);
    if (result == 0) {
        BattleEvent_CallHandlers(flow, 0x7e);
    }
    BattleEventVar_Pop(0x328a);
}

u32 func_ov093_021c79e0(BtlServerFlow *flow, u16 move, BattleMon *mon) {
    s32 ratio;
    u32 multiplier;
    u32 maxHP;
    u32 amount;

    ratio = PML_MoveGetParam(move, 0x1a);
    BattleEventVar_Push(0x329c);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetValue(0x35, 0);
    BattleEvent_CallHandlers(flow, 0x8f);
    multiplier = BattleEventVar_GetValue(0x35);
    BattleEventVar_Pop(0x32a1);
    maxHP = GetBattleMonStat(mon, 0xe);
    if (multiplier != 0) {
        amount = fixed_round(maxHP, multiplier);
    } else {
        amount = MultiplyValueByRatio(maxHP, ratio);
    }
    if (amount == 0) {
        return 1;
    }
    if (amount > maxHP) {
        amount = maxHP;
    }
    return amount;
}


u32 func_ov093_021c7a54(BtlServerFlow *flow, BattleMon *mon, s32 param3) {
    u32 result;

    BattleEventVar_Push(0x32c4);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x2d, param3);
    BattleEventVar_SetRewriteOnceValue(0x41, FALSE);
    BattleEvent_CallHandlers(flow, 0x9a);
    result = BattleEventVar_GetValue(0x41);
    BattleEventVar_Pop(0x32ca);
    return result;
}







void func_ov093_021c7aa0(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x32d9);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x9b);
    BattleEventVar_Pop(0x32dc);
}

void func_ov093_021c7ad0(BtlServerFlow *flow, BattleMon *mon, s32 param3) {
    BattleEventVar_Push(0x32e9);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEventVar_SetConstValue(0x2d, param3);
    BattleEvent_CallHandlers(flow, 0x9c);
    BattleEventVar_Pop(0x32ed);
}


void func_ov093_021c7b0c(BtlServerFlow *flow, BattleMon *mon) {
    BattleEventVar_Push(0x32fa);
    BattleEventVar_SetConstValue(2, GetMonID(mon));
    BattleEvent_CallHandlers(flow, 0x9d);
    BattleEventVar_Pop(0x32fd);
}

void func_ov093_021c7b3c(BtlServerFlow *flow, u8 monId, u16 oldAbility, u16 newAbility) {
    BattleEventVar_Push(0x330c);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0xf, oldAbility);
    BattleEventVar_SetConstValue(0x10, newAbility);
    BattleEvent_CallHandlers(flow, 0x89);
    BattleEventVar_Pop(0x3311);
}


void func_ov093_021c7b7c(BtlServerFlow *flow, u8 monId) {
    BattleEventVar_Push(0x331d);
    BattleEventVar_SetConstValue(2, monId);
    BattleEvent_CallHandlers(flow, 0x8a);
    BattleEventVar_Pop(0x3320);
}

void ServerEvent_CheckSideEffectParam(BtlServerFlow *flow, u8 monId, u32 effect, u8 side, BattleCondition *cont) {
    BattleEventVar_Push(0x332e);
    BattleEventVar_SetConstValue(2, monId);
    BattleEventVar_SetConstValue(0x52, side);
    BattleEventVar_SetConstValue(0x53, effect);
    BattleEventVar_SetValue(0x1e, cont->raw);
    BattleEvent_CallHandlers(flow, 0x9f);
    cont->raw = BattleEventVar_GetValue(0x1e);
    BattleEventVar_Pop(0x3335);
}






void func_ov093_021c7bf8(BtlServerFlow *flow) {
    BattleEventVar_Push(0x3343);
    BattleEvent_CallHandlers(flow, 0x79);
    BattleEventVar_Pop(0x3345);
}




void func_ov093_021c7c18(void *param_1) {
    MI_CpuFill8(param_1, 0, 0xc);
}

void func_ov093_021c7c24(u16 *counts, BtlServerFlow *flow, BattleMon *mon, BattleMon *target, s32 effect) {
    u8 monId;
    u8 targetId;
    u8 clientId;
    u8 targetClientId;
    u8 opposing;
    u16 *count;

    monId = GetMonID(mon);
    targetId = GetMonID(target);
    clientId = func_ov093_021b8ddc(monId);
    targetClientId = func_ov093_021b8ddc(targetId);
    opposing = !IsAllyMonID(monId, targetId) ? TRUE : FALSE;
    count = NULL;
    if (clientId == 0) {
        if (opposing) {
            if (effect == 0) {
                count = counts;
            } else if (effect > 3) {
                count = counts + 1;
            } else if (effect < 3) {
                count = counts + 2;
            }
        }
    } else if (targetClientId == 0 && opposing) {
        if (effect == 0) {
            count = counts + 3;
        } else if (effect > 3) {
            count = counts + 4;
        } else if (effect < 3) {
            count = counts + 5;
        }
    }
    if (count != NULL && *count < 9999) {
        (*count)++;
    }
}


u16 func_ov093_021c7cbc(BtlServerFlow *flow) {
    return flow->unk1F7C;
}

u16 func_ov093_021c7cc8(BtlServerFlow *flow) {
    return flow->unk1F7E;
}




u16 func_ov093_021c7cd4(BtlServerFlow *flow) {
    return flow->unk1F80[0];
}

u16 func_ov093_021c7cdc(BtlServerFlow *flow) {
    return flow->unk1F80[1];
}


u16 func_ov093_021c7ce8(BtlServerFlow *flow) {
    return flow->unk1F80[3];
}


BattleMon *GetBattleMon(BtlServerFlow *flow, u8 monId) {
    return GetPokeParam(flow->pokeCon, monId);
}

u8 func_ov093_021c7d00(BtlServerFlow *flow, u8 monId) {
    u8 pos;

    pos = GetBattlePos(&flow->posPoke, monId);
    if (!func_ov093_021b86a0(flow->mainModule, pos)) {
        pos = 6;
    }
    return pos;
}




u8 func_ov093_021c7d34(BtlServerFlow *flow, u8 monId) {
    return data_ov095_0689d5f5(&flow->posPoke, monId);
}

u8 func_ov093_021c7d44(BtlServerFlow *flow, u8 pos) {
    return GetExistPokeID(&flow->posPoke, pos);
}

// The IDs of the mons in battle that are on the other side from a mon
u8 func_ov093_021c7d54(BtlServerFlow *flow, u8 monId, u8 *monIds) {
    BtlFlowMonIter iter;
    BattleMon *mon;
    u8 count = 0;

    func_ov093_021bd2b4(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &mon)) {
        if (!IsAllyMonID(monId, GetMonID(mon))) {
            monIds[count++] = GetMonID(mon);
        }
    }
    return count;
}

// pokebw2's CalcMoveEffectiveness
u32 func_ov093_021c7dac(BtlServerFlow *flow, u8 attackerId, u8 defenderId, u16 move) {
    BattleMon *attacker;
    BattleMon *defender;
    BtlFlowMoveParam param;
    u32 effectiveness;

    attacker = GetPokeParam(flow->pokeCon, attackerId);
    defender = GetPokeParam(flow->pokeCon, defenderId);
    flow->unk774++;
    if (IsIllusionEnabled(defender)) {
        defender = GetIllusionDisguise(flow->mainModule, flow->pokeCon, defender);
    }
    ServerEvent_GetMoveParam(flow, move, attacker, &param);
    effectiveness = func_ov093_021c6e3c(flow, attacker, defender, &param, FALSE);
    flow->unk774--;
    return effectiveness;
}

u32 AICalcDamage(BtlServerFlow *flow, u8 attackerId, u8 defenderId, u16 move, BOOL withEffectiveness, u32 damageRoll) {
    BattleMon *attacker;
    BattleMon *defender;
    u32 effectiveness;
    BtlFlowMoveParam param;
    u16 damage;

    if (move != 0 && PML_MoveIsDamaging(move)) {
        attacker = GetPokeParam(flow->pokeCon, attackerId);
        defender = GetPokeParam(flow->pokeCon, defenderId);
        flow->unk774++;
        if (IsIllusionEnabled(defender)) {
            defender = GetIllusionDisguise(flow->mainModule, flow->pokeCon, defender);
        }
        if (withEffectiveness) {
            effectiveness = func_ov093_021c7dac(flow, attackerId, defenderId, move);
        } else {
            effectiveness = TYPE_EFFECTIVENESS_NORMAL;
        }
        ServerEvent_GetMoveParam(flow, move, attacker, &param);
        ServerEvent_CalcDamage(flow, attacker, defender, &param, effectiveness, 0x1000, FALSE,
                               damageRoll == USE_MIN_DAMAGE ? TRUE : FALSE, &damage);
        flow->unk774--;
        return damage;
    }
    return 0;
}

// Whether the flow is only simulating a move, for the AI
BOOL func_ov093_021c7ec4(BtlServerFlow *flow) {
    return flow->unk774 != 0 ? TRUE : FALSE;
}

u8 func_ov093_021c7ed8(BtlServerFlow *flow, u8 monId) {
    return func_ov093_021b997c(flow->mainModule, func_ov093_021b8ddc(monId));
}

u8 func_ov093_021c7eec(BtlServerFlow *flow, u8 monId) {
    return func_ov093_021bb85c(flow->server, func_ov093_021b8ddc(monId))->numCoverPos;
}

u8 func_ov093_021c7f04(BtlServerFlow *flow, u8 monId) {
    if (BtlSetup_GetBattleStyle(flow->mainModule) != BTL_STYLE_ROTATION) {
        return func_ov093_021c7eec(flow, monId);
    }
    return 3;
}

BOOL func_ov093_021c7f24(BtlServerFlow *flow, u8 monId) {
    BattleParty *party;

    party = func_ov093_021c7fcc(flow, monId);
    if (func_ov093_021b9bc4(party, func_ov093_021c7f04(flow, monId))) {
        return TRUE;
    }
    return FALSE;
}

// Sets a flag that can only be set once, and returns whether this call set it
BOOL func_ov093_021c7f4c(BtlServerFlow *flow) {
    if (flow->unk78A_3) {
        return FALSE;
    }
    flow->unk78A_3 = 1;
    return TRUE;
}

u8 HandlerGetAlivePartyCount(BtlServerFlow *flow, u16 code, u8 *monIds) {
    u8 positions[6];
    u8 numPositions;
    u8 count;
    u8 i;
    BattleMon *mon;

    numPositions = func_ov093_021b8778(flow->mainModule, code, positions);
    for (i = 0, count = 0; i < numPositions; i++) {
        mon = func_ov093_021b98a4(flow->pokeCon, positions[i]);
        if (BtlFlow_IsMonAlive(mon)) {
            monIds[count++] = GetMonID(mon);
        }
    }
    return count;
}

BattleParty *func_ov093_021c7fcc(BtlServerFlow *flow, u8 monId) {
    return func_ov093_021b9af8(flow->pokeCon, func_ov093_021b8ddc(monId));
}

BattleParty *func_ov093_021c7fe0(BtlServerFlow *flow, u8 monId) {
    u8 clientId;

    clientId = func_ov093_021b9010(flow->mainModule, func_ov093_021b8ddc(monId));
    if (DoesClientExist(flow->mainModule, clientId)) {
        return func_ov093_021b9af8(flow->pokeCon, clientId);
    }
    return NULL;
}

u8 func_ov093_021c8010(BtlServerFlow *flow, u8 monId) {
    return MonIDToBattlePos(flow->mainModule, flow->pokeCon, monId);
}

u8 func_ov093_021c8020(BtlServerFlow *flow, u8 pos) {
    return GetMonID(func_ov093_021b98ac(flow->pokeCon, pos));
}

u8 func_ov093_021c8030(BtlServerFlow *flow, u8 monId, u16 move) {
    return func_ov093_021d80bc(flow->mainModule, flow->pokeCon, GetPokeParam(flow->pokeCon, monId), move);
}

// The action a mon still has to take this turn
BOOL func_ov093_021c804c(BtlServerFlow *flow, u8 monId, BattleAction *action) {
    u32 i;

    for (i = 0; i < flow->actionOrderCount; i++) {
        if (monId == GetMonID(flow->actionOrder[i].mon) && flow->actionOrder[i].action.bits.action != 6) {
            *action = flow->actionOrder[i].action;
            return TRUE;
        }
    }
    return FALSE;
}

// Whether every other mon has taken its action this turn
BOOL func_ov093_021c80ac(BtlServerFlow *flow, u8 monId) {
    u32 i;
    u32 others;
    u32 done;

    done = 0;
    others = 0;
    for (i = 0; i < flow->actionOrderCount; i++) {
        if (monId != GetMonID(flow->actionOrder[i].mon)) {
            others++;
            if (flow->actionOrder[i].done) {
                done++;
            }
        }
    }
    return others == done ? TRUE : FALSE;
}

u16 func_ov093_021c8114(BtlServerFlow *flow) {
    return flow->unk1F74;
}

// A work buffer that the event handlers share, of at least size bytes
u8 *func_ov093_021c8120(BtlServerFlow *flow, u32 size) {
    return flow->unk1FEC;
}

// pokebw2's BtlServerFlow_GetWazaRec
BtlWazaRec *func_ov093_021c812c(BtlServerFlow *flow) {
    return &flow->wazaRec;
}

// pokebw2's BtlServerFlow_GetFaintRec
BtlFaintRec *func_ov093_021c8130(BtlServerFlow *flow) {
    return &flow->faintRec;
}

u16 GetTurnCounter(BtlServerFlow *flow) {
    return flow->unk10;
}

u8 func_ov093_021c8140(BtlServerFlow *flow, u32 index) {
    return flow->unk7D9[index];
}

BOOL func_ov093_021c814c(BtlServerFlow *flow, u8 monId) {
    return DoesBattleMonExist(&flow->posPoke, monId);
}

u32 func_ov093_021c815c(BtlServerFlow *flow) {
    return BtlSetup_GetBattleStyle(flow->mainModule);
}

u32 func_ov093_021c8168(BtlServerFlow *flow) {
    return BtlSetup_GetBattleType(flow->mainModule);
}

u32 GetBattleTerrain(BtlServerFlow *flow) {
    return GetFieldEffectData(flow->mainModule)->env.terrain;
}

u32 func_ov093_021c8180(BtlServerFlow *flow) {
    return func_ov093_021b8670(flow->mainModule);
}

// Whether a mon's species can evolve
BOOL CheckEvolution(BtlServerFlow *flow, u8 monId) {
    u16 species;
    u16 i;

    species = GetBattleMonSpecies(GetPokeParam(flow->pokeCon, monId));
    for (i = 0; i < 7; i++) {
        if (func_0201b718(flow->unk4A4, species, 0, i)) {
            return TRUE;
        }
    }
    return FALSE;
}

u16 func_ov093_021c81c8(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    return ServerEvent_CalculateSpeed(flow, mon, flag);
}

// How many mons in battle are faster than a mon
u32 func_ov093_021c81d0(BtlServerFlow *flow, BattleMon *mon, BOOL flag) {
    u16 speed;
    u8 monId;
    BtlFlowMonIter iter;
    BattleMon *other;
    u16 count;

    speed = func_ov093_021c81c8(flow, mon, flag);
    monId = GetMonID(mon);
    count = 0;
    func_ov093_021bd2b4(&iter, flow);
    while (func_ov093_021bd34c(&iter, flow, &other)) {
        if (monId != GetMonID(other) && func_ov093_021c81c8(flow, other, flag) > speed) {
            count++;
        }
    }
    return count;
}

BOOL func_ov093_021c8234(BtlServerFlow *flow, u8 monId) {
    return func_ov093_021c6f0c(flow, GetPokeParam(flow->pokeCon, monId), TRUE);
}

// Whether a mon can use its held item: not with Klutz, under Embargo or in Magic Room
BOOL func_ov093_021c824c(BtlServerFlow *flow, u8 monId) {
    BattleMon *mon;

    mon = GetPokeParam(flow->pokeCon, monId);
    if (GetBattleMonStat(mon, 0x11) == ABILITY_KLUTZ) {
        return FALSE;
    }
    if (CheckCondition(mon, CONDITION_EMBARGO)) {
        return FALSE;
    }
    if (func_ov093_021ef948(7)) {
        return FALSE;
    }
    return TRUE;
}

u32 GetWeather(BtlServerFlow *serverFlow) {
    return ServerEvent_GetWeather(serverFlow);
}


BOOL func_ov093_021c8290(BtlServerFlow *flow, u8 attackerId, u8 defenderId, u16 move) {
    BattleMon *attacker;

    attacker = GetPokeParam(flow->pokeCon, attackerId);
    return func_ov093_021bfa00(flow, attacker, GetPokeParam(flow->pokeCon, defenderId), move);
}

BOOL func_ov093_021c82b8(BtlServerFlow *flow, u32 value) {
    return ReturnZero(flow->mainModule, value);
}

BOOL func_ov093_021c82c4(BtlServerFlow *flow, u8 side, u32 sideEffect) {
    return data_ov095_06898cf5(side, sideEffect);
}

u32 func_ov093_021c82d0(BtlServerFlow *flow, u8 pos, u32 sideEffect) {
    return data_ov095_06898ce1(func_ov093_021b9ae0(pos), sideEffect);
}




BOOL func_ov093_021c82f4(BtlServerFlow *flow, u8 pos, u32 a2) {
    return data_ov095_068982ad(a2, pos);
}

u8 func_ov093_021c8300(BtlServerFlow *flow, u8 clientId) {
    return func_ov093_021b9010(flow->mainModule, clientId);
}

BOOL IsMonSwitchingOut(BtlServerFlow *flow) {
    return flow->unk78A_0;
}

void AddSwitchOutInterrupt(BtlServerFlow *flow, u8 monId) {
    if (flow->interruptCount < 6) {
        flow->interruptMonIds[flow->interruptCount++] = monId;
    }
}

BOOL func_ov093_021c8338(BtlServerFlow *flow, u8 monId) {
    return IsSemiInvulnMove(GetPokeParam(flow->pokeCon, monId));
}

// Whether a mon is carrying another off with Sky Drop
BOOL func_ov093_021c8348(BtlServerFlow *flow, u8 monId) {
    if (BtlFlow_GetSkyDropTarget(GetPokeParam(flow->pokeCon, monId)) != 0x1f) {
        return TRUE;
    }
    return FALSE;
}

// Whether a mon is either side of a Sky Drop
BOOL func_ov093_021c8374(BtlServerFlow *flow, u8 monId) {
    if (func_ov093_021c8348(flow, monId)) {
        return TRUE;
    }
    if (CheckCondition(GetPokeParam(flow->pokeCon, monId), 0x21)) {
        return TRUE;
    }
    return FALSE;
}

s32 func_ov093_021c83a0(BtlServerFlow *flow, u8 monId) {
    BattleMon *mon;
    fx32 ratio;
    u32 weight;

    mon = GetPokeParam(flow->pokeCon, monId);
    ratio = ServerEvent_GetWeightRatio(flow, mon);
    weight = fixed_round(GetBattleMonWeight(mon), ratio);
    if (weight < 1) {
        weight = 1;
    }
    return weight;
}

u32 func_ov093_021c83cc(BtlServerFlow *flow) {
    return func_ov093_021c2e4c(flow);
}

BOOL func_ov093_021c83d4(BtlServerFlow *flow) {
    return ServerControl_CheckMatchup(flow);
}

void SetMoveEffectIndex(BtlServerFlow *flow, u8 index) {
    flow->moveEffect->index = index;
}

// Pay Day's money, in the battles that give it
BOOL func_ov093_021c83e8(BtlServerFlow *flow, u32 money, u8 monId) {
    if (BtlSetup_GetBattleType(flow->mainModule) <= 1) {
        func_ov093_021bb908(flow->server, money);
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021c8408(BtlServerFlow *flow, u8 monId) {
    u32 type;
    u8 clientId;

    type = BtlSetup_GetBattleType(flow->mainModule);
    clientId = func_ov093_021b8ddc(monId);
    if (type <= 1 && clientId == GetPlayerClientID(flow->mainModule)) {
        func_ov093_021bb914(flow->server);
    }
}

void func_ov093_021c8434(BtlServerFlow *flow, u8 monId, u8 targetId) {
    func_ov093_021be52c(flow->unk4B0, monId, targetId, GetBattlePos(&flow->posPoke, targetId));
}




BOOL func_ov093_021c846c(BtlServerFlow *flow, u8 attackerId, u8 targetId, BOOL *failed) {
    BattleMon *attacker;

    attacker = GetPokeParam(flow->pokeCon, attackerId);
    return func_ov093_021bfeac(flow, attacker, GetPokeParam(flow->pokeCon, targetId), failed);
}

void func_ov093_021c8494(BtlServerFlow *flow, u8 monId) {
    ServerControl_SkyDropCheckRelease(flow, GetPokeParam(flow->pokeCon, monId), TRUE);
}

BOOL func_ov093_021c84ac(BtlServerFlow *flow, BattleMon *mon, u8 *flag) {
    BOOL result;

    flow->unk78A_5 = 0;
    result = ServerControl_UseHeldItem(flow, mon);
    *flag = flow->unk78A_5;
    return result;
}

void func_ov093_021c84d0(BtlServerFlow *flow, BattleMon *mon) {
    func_ov093_021bdbb8(flow, mon);
}

u32 func_ov093_021c84d8(BtlServerFlow *flow) {
    return func_ov093_021cc668(flow);
}

void func_ov093_021c84e0(BtlServerFlow *flow, u8 monId) {
    func_ov093_021c84f4(flow, monId);
}

void func_ov093_021c84e8(BtlServerFlow *flow) {
    flow->unk785 = 0;
}

void func_ov093_021c84f4(BtlServerFlow *flow, u8 monId) {
    u32 i;

    for (i = 0; i < flow->unk785; i++) {
        if (monId == flow->unk791[i]) {
            return;
        }
    }
    if (i < 24) {
        flow->unk791[i] = monId;
        flow->unk785++;
    }
}

BOOL func_ov093_021c8534(BtlServerFlow *flow) {
    u32 i;
    u8 positions[3];

    for (i = 0; i < flow->unk785; i++) {
        if (data_ov095_0689d559(&flow->posPoke, func_ov093_021b8ddc(flow->unk791[i]), positions)) {
            return TRUE;
        }
    }
    return FALSE;
}




void func_ov093_021c8588(BtlServerFlow *flow) {
    flow->frameDepth = 0;
    func_ov093_021c85d4(flow, 0, TRUE);
}

void func_ov093_021c859c(BtlServerFlow *flow) {
    if (flow->frameDepth < 6) {
        flow->frameDepth++;
        func_ov093_021c85d4(flow, flow->frameDepth, TRUE);
    }
}

void func_ov093_021c85b8(BtlServerFlow *flow) {
    if (flow->frameDepth != 0) {
        flow->frameDepth--;
        func_ov093_021c85d4(flow, flow->frameDepth, FALSE);
    }
}

// Points the flow at one level of its work, clearing it when the level is entered
void func_ov093_021c85d4(BtlServerFlow *flow, u32 depth, BOOL clear) {
    BtlFlowWorkFrame *frame = &flow->frames[depth];

    flow->unk850 = &frame->monSets[0];
    flow->unk854 = &frame->monSets[1];
    flow->unk858 = &frame->monSets[2];
    flow->unk85C = &frame->monSets[3];
    flow->unk860 = &frame->monSets[4];
    flow->unk864 = &frame->monSets[5];
    flow->unk868 = &frame->monSets[6];
    flow->moveEffect = &frame->moveEffect;
    flow->unk1AB0 = &frame->moveParams[0];
    flow->unk1AB4 = &frame->moveParams[1];
    flow->unk4B4 = &frame->hitWork;
    flow->unk4AC = &frame->reactionLists[0];
    flow->unk4B0 = &frame->reactionLists[1];
    flow->unk86C = &frame->damageLists[0];
    flow->unk870 = &frame->damageLists[1];
    flow->unk77F = frame->unk28C;
    flow->unk78A_3 = frame->unk28D;
    flow->unk78A_4 = frame->unk28E;
    if (clear) {
        data_ov095_0689cb3d(flow->unk850);
        data_ov095_0689cb3d(flow->unk854);
        data_ov095_0689cb3d(flow->unk858);
        data_ov095_0689cb3d(flow->unk85C);
        data_ov095_0689cb3d(flow->unk860);
        data_ov095_0689cb3d(flow->unk864);
        data_ov095_0689cb3d(flow->unk868);
        func_ov093_021bd1e0(flow->moveEffect);
        MI_CpuFill8(flow->unk1AB0, 0, sizeof(BtlFlowMoveParam));
        MI_CpuFill8(flow->unk1AB4, 0, sizeof(BtlFlowMoveParam));
        MI_CpuFill8(flow->unk4B4, 0, sizeof(BtlFlowHitWork));
        MI_CpuFill8(flow->unk4AC, 0, sizeof(BtlFlowReactionList));
        MI_CpuFill8(flow->unk4B0, 0, sizeof(BtlFlowReactionList));
        MI_CpuFill8(flow->unk86C, 0, sizeof(BtlFlowDamageList));
        MI_CpuFill8(flow->unk870, 0, sizeof(BtlFlowDamageList));
        flow->unk77F = 6;
        flow->unk78A_3 = 0;
        flow->unk78A_4 = 0;
    }
}





















void BattleHandler_StrClear(BattleHandlerString *string) {
    MI_CpuFill8(string, 0, 0x24);
    string->enabled = 0;
}

BOOL BattleHandler_StrIsEnabled(const BattleHandlerString *string) {
    return string->enabled != 0;
}

void BattleHandler_StrSetup(BattleHandlerString *string, u32 enabled, u16 message) {
    string->enabled = enabled;
    string->message = message;
    string->count = 0;
}

void BattleHandler_AddArg(BattleHandlerString *string, u32 arg) {
    u16 count;

    count = string->count;
    if (count < 8) {
        string->count = count + 1;
        string->args[count] = arg;
    }
}

void BattleHandler_AddSoundEffect(BattleHandlerString *string, u32 soundEffect) {
    if (string->count < 8) {
        string->soundEffect = soundEffect;
        string->hasSound = 1;
    }
}

void *BattleHandler_PushWork(BtlServerFlow *flow, u32 command, u32 monId) {
    return func_ov093_021cc9ac(&flow->actionState, command, monId);
}

void BattleHandler_PushRun(BtlServerFlow *flow, u32 command, u32 monId) {
    void *work;

    work = BattleHandler_PushWork(flow, command, monId);
    BattleHandler_PopWork(flow, work);
}

void BattleHandler_PopWork(BtlServerFlow *flow, void *work) {
    BattleHandler_Execute(flow, work);
    PopWork(&flow->actionState, work);
}

u32 BattleHandler_Result(BtlServerFlow *handler) {
    BtlActionState *state;

    state = &handler->actionState;
    if (IsUsed(state)) {
        if (func_ov093_021cc9a4(state)) {
            return 2;
        }
        return 1;
    }
    return 0;
}


u32 func_ov093_021c8910(BtlServerFlow *flow) {
    return BattleHandler_Result(flow);
}

// Function name from swan.
void BattleHandler_Execute(BtlServerFlow *flow, void *work) {
    BattleHandlerHeader *header = work;
    u16 itemId;
    u8 result;

    itemId = GetUseItemNo(&flow->actionState);
    if (IsUsed(&flow->actionState)) {
        result = GetPrevResult(&flow->actionState);
    } else {
        result = TRUE;
    }
    if ((header->checkPrevResult && !result)
        || (header->checkFainted && IsFainted(GetPokeParamConst(flow->pokeCon, header->monId)))) {
        return;
    }
    switch (header->command) {
    case 1:
        result = func_ov093_021c8c6c(flow, work);
        break;
    case 2:
        result = BattleHandler_AbilityPopupAdd(flow, work);
        break;
    case 3:
        result = BattleHandler_AbilityPopupRemove(flow, work);
        break;
    case 0:
        result = BattleHandler_UseHeldItem(flow, work);
        break;
    case 5:
        result = BattleHandler_RecoverHP(flow, work, itemId);
        break;
    case 6:
        result = BattleHandler_Drain(flow, work, itemId);
        break;
    case 7:
        result = BattleHandler_Damage(flow, work);
        break;
    case 8:
        result = BattleHandler_ChangeHP(flow, work);
        break;
    case 9:
        result = BattleHandler_RecoverPP(flow, work, itemId);
        break;
    case 10:
        result = BattleHandler_DecrementPP(flow, work, itemId);
        break;
    case 11:
        result = BattleHandler_CureCondition(flow, work, itemId);
        break;
    case 12:
        result = BattleHandler_AddCondition(flow, work);
        break;
    case 14:
        result = BattleHandler_StatChange(flow, work, itemId);
        break;
    case 15:
        result = BattleHandler_SetStatStage(flow, work);
        break;
    case 18:
        result = BattleHandler_RecoverStatStage(flow, work);
        break;
    case 16:
        result = BattleHandler_ResetStatStage(flow, work);
        break;
    case 17:
        result = BattleHandler_SetStatus(flow, work);
        break;
    case 19:
        result = BattleHandler_Faint(flow, work);
        break;
    case 20:
        result = BattleHandler_ChangeType(flow, work);
        break;
    case 4:
        result = BattleHandler_Message(flow, work);
        break;
    case 21:
        result = BattleHandler_SetTurnFlag(flow, work);
        break;
    case 22:
        result = BattleHandler_ResetTurnFlag(flow, work);
        break;
    case 23:
        result = BattleHandler_SetContinueFlag(flow, work);
        break;
    case 24:
        result = BattleHandler_ResetContinueFlag(flow, work);
        break;
    case 25:
        result = BattleHandler_AddSideEffect(flow, work);
        break;
    case 26:
        result = BattleHandler_RemoveSideEffectCore(flow, work);
        break;
    case 27:
        result = BattleHandler_AddFieldEffect(flow, work);
        break;
    case 29:
        result = BattleHandler_ChangeWeather(flow, work);
        break;
    case 28:
        result = BattleHandler_RemoveFieldEffect(flow, work);
        break;
    case 30:
        result = func_ov093_021c9a24(flow, work);
        break;
    case 31:
        result = BattleHandler_AbilityChange(flow, work);
        break;
    case 32:
        result = BattleHandler_SetItem(flow, work);
        break;
    case 33:
        result = BattleHandler_CheckHeldItem(flow, work);
        break;
    case 34:
        result = BattleHandler_ForceUseItem(flow, work);
        break;
    case 35:
        result = BattleHandler_ConsumeItem(flow, work);
        break;
    case 36:
        result = BattleHandler_SwapItem(flow, work);
        break;
    case 37:
        result = BattleHandler_UpdateMove(flow, work);
        break;
    case 38:
        result = BattleHandler_SetCounter(flow, work);
        break;
    case 39:
        result = BattleHandler_DelayMoveDamage(flow, work);
        break;
    case 40:
        result = BattleHandler_QuitBattle(flow, work);
        break;
    case 41:
        result = BattleHandler_Switch(flow, work);
        break;
    case 42:
        result = BattleHandler_BatonPass(flow, work);
        break;
    case 43:
        result = BattleHandler_Flinch(flow, work);
        break;
    case 44:
        result = BattleHandler_Revive(flow, work);
        break;
    case 45:
        result = BattleHandler_SetWeight(flow, work);
        break;
    case 46:
        result = BattleHandler_ForceSwitch(flow, work);
        break;
    case 47:
        result = BattleHandler_InterruptAction(flow, work);
        break;
    case 48:
        result = BattleHandler_InterruptMove(flow, work);
        break;
    case 49:
        result = BattleHandler_SendLast(flow, work);
        break;
    case 50:
        result = BattleHandler_SwapPoke(flow, work);
        break;
    case 51:
        result = BattleHandler_Transform(flow, work);
        break;
    case 52:
        result = BattleHandler_IllusionBreak(flow, work);
        break;
    case 53:
        result = BattleHandler_GravityCheck(flow, work);
        break;
    case 54:
        result = BattleHandler_HideTurnCancel(flow, work);
        break;
    case 55:
        result = BattleHandler_EffectAtPos(flow, work);
        break;
    case 56:
        result = BattleHandler_RemoveMessageWindow(flow, work);
        break;
    case 57:
        result = BattleHandler_ChangeForm(flow, work);
        break;
    case 58:
        result = BattleHandler_SetMoveEffectIndex(flow, work);
        break;
    case 59:
        result = BattleHandler_SetMoveEffectEnable(flow, work);
        break;
    }
    SetResult(&flow->actionState, result);
}

u8 func_ov093_021c8c6c(BtlServerFlow *handler, BattleHandlerHeader *param) {
    if (DoesBattleMonExist(&handler->posPoke, param->monId)) {
        ServerDisplay_UseHeldItem(handler, GetPokeParam(handler->pokeCon, param->monId));
        return TRUE;
    }
    return FALSE;
}



// Function name from swan.
u8 BattleHandler_AbilityPopupAdd(BtlServerFlow *handler, BattleHandlerHeader *param) {
    if (DoesBattleMonExist(&handler->posPoke, param->monId)) {
        ServerDisplay_AbilityPopupAdd(handler, GetPokeParam(handler->pokeCon, param->monId));
        return TRUE;
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_AbilityPopupRemove(BtlServerFlow *handler, BattleHandlerPopupParam *param) {
    BattleMon *mon = GetPokeParam(handler->pokeCon, param->monId);
    ServerDisplay_AbilityPopupRemove(handler, mon);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_RecoverHP(BtlServerFlow *handler, const BattleHandlerRecoverHPParam *param, u16 itemId) {
    BattleMon *mon;
    BattleMon *target;
    u8 result = FALSE;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    target = GetPokeParam(handler->pokeCon, param->targetIndex);
    if (!ServerControl_RecoverHPCheckFail(handler, target)) {
        if (param->popup) {
            ServerDisplay_AbilityPopupAdd(handler, mon);
        }
        if (!param->skipCheck) {
            result = !ServerControl_RecoverHPCheckFailSpecial(handler, target, TRUE);
        } else {
            result = TRUE;
        }
        if (result) {
            ServerControl_RecoverHPCore(handler, target, param->amount);
            if (param->string.enabled) {
                BattleHandler_SetString(handler, &param->string);
            } else if (itemId != 0) {
                func_ov093_021cd64c(handler->queue, 0x5b, 0x38c, param->targetIndex, itemId, 0xffff0000);
            }
        }
        if (param->popup) {
            ServerDisplay_AbilityPopupRemove(handler, mon);
        }
    }
    return result;
}

// Function name from swan.
u8 BattleHandler_Drain(BtlServerFlow *handler, BattleHandlerDrainParam *param, u16 itemId) {
    BattleMon *source;
    BattleMon *mon;

    source = NULL;
    if (param->sourceIndex != 0x1f) {
        source = GetPokeParam(handler->pokeCon, param->sourceIndex);
    }
    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (!IsFainted(mon)) {
            if (ServerControl_DrainCore(handler, mon, source, param->amount)) {
                if (param->string.enabled) {
                    BattleHandler_SetString(handler, &param->string);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_Damage(BtlServerFlow *handler, BattleHandlerDamageParam *param) {
    BattleMon *mon;
    BattleMon *source;

    if (DoesBattleMonExist(&handler->posPoke, param->targetIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->targetIndex);
        source = NULL;
        if (param->sourceIndex != 0x1f) {
            source = GetPokeParam(handler->pokeCon, param->sourceIndex);
        }
        if (!IsFainted(mon)) {
            if (!param->checkSemi || !IsSemiInvulnMove(mon)) {
                if (ServerControl_CheckSimpleDamageEnabled(handler, mon, param->amount)) {
                    if (param->popup) {
                        ServerDisplay_AbilityPopupAdd(handler, source);
                    }
                    if (param->showViewEffect) {
                        ServerControl_ViewEffect(handler, param->effect, param->effectArg1, param->effectArg2, 0, 0);
                    }
                    ServerControl_SimpleDamageCore(handler, mon, param->amount, &param->string);
                    if (param->popup) {
                        ServerDisplay_AbilityPopupRemove(handler, source);
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_ChangeHP(BtlServerFlow *handler, BattleHandlerChangeHPParam *param) {
    u8 result;
    u32 i;
    BattleMon *mon;

    result = FALSE;
    for (i = 0; i < param->count; i++) {
        if (DoesBattleMonExist(&handler->posPoke, param->monIds[i])) {
            mon = GetPokeParam(handler->pokeCon, param->monIds[i]);
            if (!IsFainted(mon)) {
                ServerDisplay_SimpleHP(handler, mon, param->hpChanges[i], param->suppress == 0);
                if (param->skipReaction == 0) {
                    ServerControl_CheckItemReaction(handler, mon, 1);
                }
                result = TRUE;
            }
        }
    }
    return result;
}




// Function name from swan.
u8 BattleHandler_RecoverPP(BtlServerFlow *handler, const BattleHandlerPPParam *param, u16 itemId) {
    BattleMon *user;
    BattleMon *mon;
    BOOL original;

    user = GetPokeParam(handler->pokeCon, param->header.monId);
    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (CanPokemonBattle(mon) || param->allowFainted) {
        original = !param->currentMoves;
        if (!Move_IsPPFull(mon, param->moveIndex, original)) {
            ServerDisplay_RecoverPP(handler, mon, param->moveIndex, param->amount, original);
            BattleHandler_SetString(handler, &param->string);
            return TRUE;
        }
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_DecrementPP(BtlServerFlow *handler, BattleHandlerPPParam *param, u16 itemId) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon) || param->allowFainted) {
        if (ServerControl_DecrementPP(handler, mon, param->moveIndex, param->amount)) {
            BattleHandler_SetString(handler, &param->string);
            if (ServerEvent_DecrementPP(handler, mon, param->moveIndex)) {
                ServerControl_UseHeldItem(handler, mon);
            }
        }
        return TRUE;
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_CureCondition(BtlServerFlow *handler, const struct BattleHandlerCureConditionParam *param, u16 context) {
    BattleMon *target;
    u8 changed;
    u32 i;
    BattleMon *mon;
    s32 condition;
    s32 code;
    BattleCondition prev;

    target = GetPokeParam(handler->pokeCon, param->monIndex);
    changed = FALSE;
    if (param->popup) {
        ServerDisplay_AbilityPopupAdd(handler, target);
    }
    for (i = 0; i < param->count; i++) {
        mon = GetPokeParam(handler->pokeCon, param->monIds[i]);
        if (!CanPokemonBattle(mon)) {
            continue;
        }
        condition = param->condition;
        while ((code = ConvertConditionCode(mon, &condition)) != 0) {
            changed = TRUE;
            ServerControl_CureCondition(handler, mon, code, &prev);
            if (param->useString == 0) {
                if (data_ov095_0689c465(code, prev, mon, context, &handler->message)) {
                    BattleHandler_SetString(handler, &handler->message);
                    BattleHandler_StrClear(&handler->message);
                }
            } else {
                BattleHandler_SetString(handler, &param->string);
            }
            if (code == 0x13) {
                ServerControl_CheckItemReaction(handler, mon, 0);
            }
        }
    }
    if (param->popup) {
        ServerDisplay_AbilityPopupRemove(handler, target);
    }
    return changed;
}



// Function name from swan.
u8 BattleHandler_AddCondition(BtlServerFlow *handler, const BattleHandlerAddConditionParam *param) {
    BattleMon *mon;
    u8 showMessage;
    BattleMon *target;

    mon = param->monIndex != 0x1f ? GetPokeParam(handler->pokeCon, param->monIndex) : NULL;
    showMessage = !BattleHandler_StrIsEnabled(&param->string) && !param->noMessage ? TRUE : FALSE;
    if (DoesBattleMonExist(&handler->posPoke, param->targetIndex)) {
        target = GetPokeParam(handler->pokeCon, param->targetIndex);
        if (!IsFainted(target)
            && !ServerControl_AddConditionCheckFail(handler, target, mon, param->condition, param->value,
                                                    param->overwrite, param->showFail)) {
            if (param->popup) {
                ServerDisplay_AbilityPopupAdd(handler, mon);
            }
            ServerControl_AddCondition(handler, target, mon, param->condition, param->value, showMessage,
                                       param->skipItemReaction, &param->string);
            if (param->popup) {
                ServerDisplay_AbilityPopupRemove(handler, mon);
            }
            return TRUE;
        }
    }
    return FALSE;
}





// Function name from swan.
u8 BattleHandler_StatChange(BtlServerFlow *handler, const struct BattleHandlerStatChangeParam *param, u16 context) {
    BattleMon *popupMon;
    BattleMon *mon;
    BOOL valid;
    u8 result;
    u32 i;
    u32 stat;

    popupMon = GetPokeParam(handler->pokeCon, param->monIndex);
    valid = FALSE;
    result = FALSE;
    for (i = 0; i < param->count; i++) {
        if (DoesBattleMonExist(&handler->posPoke, param->monIds[i])) {
            mon = GetPokeParam(handler->pokeCon, param->monIds[i]);
            if (!IsFainted(mon) && IsStatChangeValid(mon, param->stat, param->change)) {
                valid = TRUE;
                break;
            }
        }
    }
    if (valid && param->popup) {
        ServerDisplay_AbilityPopupAdd(handler, popupMon);
    }
    for (i = 0; i < param->count; i++) {
        if (DoesBattleMonExist(&handler->posPoke, param->monIds[i])) {
            mon = GetPokeParam(handler->pokeCon, param->monIds[i]);
            if (!IsFainted(mon)) {
                stat = param->stat;
                if (func_ov093_021c3008(handler, param->monIndex, mon, stat, param->change, 0x1f, context, param->value,
                                        param->unk0e, param->flag == 0)) {
                    BattleHandler_SetString(handler, &param->string);
                    result = TRUE;
                }
            }
        }
    }
    if (valid && param->popup) {
        ServerDisplay_AbilityPopupRemove(handler, popupMon);
    }
    return result;
}







// Function name from swan.
u8 BattleHandler_SetStatStage(BtlServerFlow *handler, const BattleHandlerSetStatStageParam *param) {
    BattleMon *mon;

    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (!IsFainted(mon)) {
            func_ov093_021d5ea8(mon, 1, param->attack);
            func_ov093_021d5ea8(mon, 2, param->defense);
            func_ov093_021d5ea8(mon, 3, param->spAttack);
            func_ov093_021d5ea8(mon, 4, param->spDefense);
            func_ov093_021d5ea8(mon, 5, param->speed);
            func_ov093_021d5ea8(mon, 6, param->accuracy);
            func_ov093_021d5ea8(mon, 7, param->evasion);
            func_ov093_021cd4b0(handler->queue, 0xb, param->monIndex, param->attack, param->defense, param->spAttack,
                                param->spDefense, param->speed, param->accuracy, param->evasion);
            return TRUE;
        }
    }
    return FALSE;
}




// Function names from swan.
u8 BattleHandler_RecoverStatStage(BtlServerFlow *handler, BattleHandlerRecoverStatStageParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        func_ov093_021cd4b0(handler->queue, 0xc, param->monIndex);
        return StatStageRecover(mon);
    }
    return FALSE;
}

u8 BattleHandler_ResetStatStage(BtlServerFlow *handler, BattleHandlerResetStatStageParam *param) {
    u32 i;
    u8 result;
    BattleMon *mon;

    result = FALSE;
    for (i = 0; i < param->count; i++) {
        mon = GetPokeParam(handler->pokeCon, param->monIndices[i]);
        if (!IsFainted(mon)) {
            func_ov093_021cd4b0(handler->queue, 0xd, param->monIndices[i]);
            StatStageReset(mon);
            result = TRUE;
        }
    }
    return result;
}

// Function name from swan.
u8 BattleHandler_SetStatus(BtlServerFlow *handler, const BattleHandlerSetStatusParam *param) {
    BattleMon *mon;

    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (param->setAttack) {
            SetBaseStatus(mon, 8, param->attack);
            func_ov093_021cd4b0(handler->queue, 0x13, param->monIndex, 8, (u8)param->attack);
        }
        if (param->setDefense) {
            SetBaseStatus(mon, 9, param->defense);
            func_ov093_021cd4b0(handler->queue, 0x13, param->monIndex, 9, (u8)param->defense);
        }
        if (param->setSpAttack) {
            SetBaseStatus(mon, 10, param->spAttack);
            func_ov093_021cd4b0(handler->queue, 0x13, param->monIndex, 10, (u8)param->spAttack);
        }
        if (param->setSpDefense) {
            SetBaseStatus(mon, 11, param->spDefense);
            func_ov093_021cd4b0(handler->queue, 0x13, param->monIndex, 11, (u8)param->spDefense);
        }
        if (param->setSpeed) {
            SetBaseStatus(mon, 12, param->speed);
            func_ov093_021cd4b0(handler->queue, 0x13, param->monIndex, 12, (u8)param->speed);
        }
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_Faint(BtlServerFlow *handler, BattleHandlerFaintParam *param) {
    BattleMon *mon;

    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (!IsFainted(mon) || param->force) {
            BattleHandler_SetString(handler, &param->string);
            ServerControl_FaintPokemon(handler, mon);
            return TRUE;
        }
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_ChangeType(BtlServerFlow *handler, BattleHandlerChangeTypeParam *param) {
    BattleMon *mon;

    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (!IsFainted(mon) && !IsArceus(GetBattleMonSpecies(mon))) {
            func_ov093_021cd4b0(handler->queue, 0x16, param->monIndex, param->type);
            ChangePokeType(mon, param->type);
            if (!param->suppressMessage && PokeTypePair_IsMonotype(param->type)) {
                func_ov093_021cd64c(handler->queue, 0x5b, 0x380, param->monIndex, PokeTypePair_GetType1(param->type),
                                    0xffff0000);
            }
            return TRUE;
        }
    }
    return FALSE;
}







u8 BattleHandler_Message(BtlServerFlow *handler, BattleHandlerMessageParam *param) {
    BattleMon *mon;

    mon = NULL;
    if (param->monIndex != 0x1f) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
    }
    if (param->popup && mon != NULL) {
        ServerDisplay_AbilityPopupAdd(handler, mon);
    }
    BattleHandler_SetString(handler, &param->string);
    if (param->popup && mon != NULL) {
        ServerDisplay_AbilityPopupRemove(handler, mon);
    }
    return TRUE;
}

u8 BattleHandler_SetTurnFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        func_ov093_021d5fc0(mon, param->flag);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_ResetTurnFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        func_ov093_021d6440(mon, param->flag);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_SetContinueFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        scPut_SetContFlag(handler, mon, param->flag);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_ResetContinueFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        scPut_ResetContFlag(handler, mon, param->flag);
        return TRUE;
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_AddSideEffect(BtlServerFlow *handler, const BattleHandlerAddSideEffectParam *param) {
    BattleCondition cont = param->cont;

    ServerEvent_CheckSideEffectParam(handler, param->header.monId, param->effect, param->side, &cont);
    if (data_ov095_06898c11(param->side, param->effect, cont)) {
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_RemoveSideEffectCore(BtlServerFlow *handler, const BattleHandlerRemoveSideEffectParam *param) {
    u8 removed;
    u32 i;

    removed = FALSE;
    for (i = 0; i < 14; i++) {
        if (BattleHandler_IsFlagSet(param->effects, i) && ServerDisplay_RemoveSideEffect(param->side, i)) {
            func_ov093_021c4bac(handler, i, param->side);
            removed = TRUE;
        }
    }
    return removed;
}




u8 BattleHandler_AddFieldEffect(BtlServerFlow *handler, BattleHandlerAddFieldEffectParam *param) {
    if (ServerControl_FieldEffectCore(handler, param->effect, param->value, param->duration)) {
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_RemoveFieldEffect(BtlServerFlow *handler, BattleHandlerRemoveFieldEffectParam *param) {
    if (FieldStatusRemoveEffect(param->effect)) {
        ServerControl_FieldEffectEnd(handler, param->effect);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_ChangeWeather(BtlServerFlow *handler, BattleHandlerChangeWeatherParam *param) {
    BattleMon *mon;
    u32 result;
    u32 state;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    result = FALSE;
    if (param->weather != 0) {
        if (ServerControl_ChangeWeatherCheck(handler, param->weather, param->duration)) {
            if (param->popup) {
                ServerDisplay_AbilityPopupAdd(handler, mon);
            }
            ServerControl_ChangeWeatherCore(handler, param->weather, param->duration);
            BattleHandler_SetString(handler, &param->string);
            result = TRUE;
            if (param->popup) {
                ServerDisplay_AbilityPopupRemove(handler, mon);
            }
        }
    } else if (param->notifyAirLock != 0) {
        if (param->popup) {
            ServerDisplay_AbilityPopupAdd(handler, mon);
        }
        BattleHandler_SetString(handler, &param->string);
        state = PushState(&handler->actionState, 0x3c6b);
        func_ov093_021c7bf8(handler);
        PopState(&handler->actionState, state, 0x3c6d);
        result = TRUE;
        if (param->popup) {
            ServerDisplay_AbilityPopupRemove(handler, mon);
        }
    }
    return result;
}

// Function name from swan.
BOOL BattleHandler_SetString(BtlServerFlow *handler, const BattleHandlerString *string) {
    u16 soundEffect;
    u32 flags;

    flags = string->flags;

    if (!((flags << 16) >> 31)) {
        switch ((flags & 0xff)) {
        case 1:
            ServerDisplay_StandardMessage(handler, string->message, ((flags << 17) >> 25), string->args);
            return TRUE;
        case 2:
            ServerDisplay_SetMessage(handler, string->message, ((flags << 17) >> 25), string->args);
            return TRUE;
        }
    } else {
        soundEffect = string->soundEffect;
        switch ((flags & 0xff)) {
        case 1:
            ServerDisplay_StandardMessageEx(handler, string->message, soundEffect, ((flags << 17) >> 25), string->args);
            return TRUE;
        case 2:
            ServerDisplay_SetMessageEx(handler, string->message, soundEffect, ((flags << 17) >> 25), string->args);
            return TRUE;
        }
    }
    return FALSE;
}

// Adds an effect that waits at a position, as Wish and Future Sight do

u8 func_ov093_021c9a24(BtlServerFlow *handler, const BattleHandlerPosEffectParam *param) {
    return PosEventAdd(param->effect, param->pos, param->header.monId, param->args, param->argCount) ? TRUE : FALSE;
}



u8 BattleHandler_AbilityChange(BtlServerFlow *flow, BattleHandlerAbilityChangeParam *param) {
    BattleMon *mon;
    u16 oldAbility;
    u32 state;

    mon = GetPokeParam(flow->pokeCon, param->targetIndex);
    oldAbility = GetBattleMonStat(mon, 0x10);
    if (IsAbilityMultitype(oldAbility)) {
        return FALSE;
    }
    if (IsFainted(mon)) {
        return FALSE;
    }
    if (param->force || param->ability != oldAbility) {
        if (param->popup) {
            func_ov093_021cd4b0(flow->queue, 0x57, (u8)param->monIndex);
        }
        func_ov093_021cd4b0(flow->queue, 0x49, param->targetIndex, param->ability);
        BattleHandler_SetString(flow, &param->string);
        state = PushState(&flow->actionState, 0x3cc9);
        func_ov093_021c7b3c(flow, param->targetIndex, oldAbility, param->ability);
        PopState(&flow->actionState, state, 0x3ccb);
        AbilityEvent_RemoveItem(mon);
        ChangeAbility(mon, param->ability);
        func_ov093_021cd4b0(flow->queue, 0x1d, param->targetIndex, param->ability);
        AbilityEvent_AddItem(mon);
        func_ov093_021cd4b0(flow->queue, 0x58, param->targetIndex);
        if (oldAbility != param->ability) {
            state = PushState(&flow->actionState, 0x3cd9);
            func_ov093_021c7b7c(flow, param->targetIndex);
            PopState(&flow->actionState, state, 0x3cdb);
        }
        if (param->popup) {
            func_ov093_021cd4b0(flow->queue, 0x58, (u8)param->monIndex);
        }
        if (!CheckCondition(mon, 0x10)) {
            if (oldAbility == 0x67) {
                ServerControl_CheckItemReaction(flow, mon, 0);
            }
            if (oldAbility == 0x7f) {
                ServerControl_UnnerveAction(flow, mon);
            }
        }
        return TRUE;
    }
    return FALSE;
}




// Function name from swan.
u8 BattleHandler_SetItem(BtlServerFlow *handler, BattleHandlerSetItemParam *param) {
    BattleMon *mon;
    u8 result;
    u32 state;

    mon = GetPokeParam(handler->pokeCon, param->targetIndex);
    if (param->monIndex != param->targetIndex) {
        state = PushState(&handler->actionState, 0x3cff);
        result = func_ov093_021c7a54(handler, mon, param->item);
        PopState(&handler->actionState, state, 0x3d01);
        if (result) {
            state = PushState(&handler->actionState, 0x3d05);
            func_ov093_021c7aa0(handler, mon);
            PopState(&handler->actionState, state, 0x3d07);
            return FALSE;
        }
    }
    if (param->popup) {
        func_ov093_021cd4b0(handler->queue, 0x57, (u8)param->monIndex);
    }
    BattleHandler_SetString(handler, &param->string);
    ServerControl_ChangeHeldItem(handler, mon, param->item, 0);
    if (param->popup) {
        func_ov093_021cd4b0(handler->queue, 0x58, (u8)param->monIndex);
    }
    if (param->clearConsumed) {
        ClearConsumedItem(mon);
        func_ov093_021cd4b0(handler->queue, 0x2b, (u8)param->targetIndex);
    }
    if (param->clearOtherConsumed) {
        ClearConsumedItem(GetPokeParam(handler->pokeCon, param->otherIndex));
        func_ov093_021cd4b0(handler->queue, 0x2b, (u8)param->otherIndex);
    }
    ServerControl_CheckItemReaction(handler, mon, 0);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_SwapItem(BtlServerFlow *handler, BattleHandlerSwapItemParam *param) {
    BattleMon *first;
    BattleMon *second;
    u16 firstItem;
    u16 secondItem;
    u8 result;
    u32 state;

    first = GetPokeParam(handler->pokeCon, param->otherIndex);
    second = GetPokeParam(handler->pokeCon, param->monIndex);
    firstItem = GetBattleMonHeldItem(second);
    secondItem = GetBattleMonHeldItem(first);
    state = PushState(&handler->actionState, 0x3d37);
    result = func_ov093_021c7a54(handler, first, firstItem);
    PopState(&handler->actionState, state, 0x3d39);
    if (result) {
        state = PushState(&handler->actionState, 0x3d3c);
        func_ov093_021c7aa0(handler, first);
        PopState(&handler->actionState, state, 0x3d3e);
        return FALSE;
    }
    if (param->popup) {
        ServerDisplay_AbilityPopupAdd(handler, second);
    }
    BattleHandler_SetString(handler, &param->firstString);
    BattleHandler_SetString(handler, &param->secondString);
    BattleHandler_SetString(handler, &param->thirdString);
    if (param->popup) {
        ServerDisplay_AbilityPopupRemove(handler, second);
    }
    ServerControl_ChangeHeldItem(handler, second, secondItem, 0);
    ServerControl_ChangeHeldItem(handler, first, firstItem, 0);
    ServerControl_CheckItemReaction(handler, second, 0);
    ServerControl_CheckItemReaction(handler, first, 0);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_CheckHeldItem(BtlServerFlow *handler, BattleHandlerCheckHeldItemParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    ServerControl_CheckItemReaction(handler, mon, param->reaction);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_UseHeldItem(BtlServerFlow *handler, BattleHandlerUseHeldItemParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon) || param->allowFainted) {
        if (param->checkFullHp && IsMonFullHP(mon)) {
            return FALSE;
        }
        if (ServerControl_UseHeldItem(handler, mon)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_ForceUseItem(BtlServerFlow *handler, BattleHandlerForceUseItemParam *param) {
    BattleMon *mon;
    void *temp;
    u32 reserve;
    u32 state;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon)) {
        temp = ItemEvent_TempAdd(mon, param->item);
        if (temp != NULL) {
            reserve = SCQUE_RESERVE_Pos(handler->queue, 0x42);
            state = PushState(&handler->actionState, 0x3d8b);
            ServerEvent_EquipTempItem(handler, mon, param->monIndex2);
            if (BattleHandler_Result(handler) == 2) {
                func_ov093_021cd568(handler->queue, reserve, 0x42, param->monIndex);
            }
            PopState(&handler->actionState, state, 0x3d92);
            func_ov093_021dcf9c(temp);
            return TRUE;
        }
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_ConsumeItem(BtlServerFlow *handler, BattleHandlerConsumeItemParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (param->skipDisplay == 0) {
        ServerDisplay_UseHeldItem(handler, mon);
        BattleHandler_SetString(handler, &param->string);
    }
    ServerControl_ChangeHeldItem(handler, mon, 0, 1);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_UpdateMove(BtlServerFlow *handler, const BattleHandlerUpdateMoveParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    func_ov093_021cd4b0(handler->queue, 0x1f, param->monIndex, param->slot, param->maxPP, param->updateCurrent,
                        param->move);
    Move_UpdateID(mon, param->slot, param->move, param->maxPP, param->updateCurrent);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_SetCounter(BtlServerFlow *handler, BattleHandlerSetCounterParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    ServerControl_SetMonCounter(handler, mon, param->counter, param->value);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_DelayMoveDamage(BtlServerFlow *handler, const BattleHandlerDelayMoveDamageParam *param) {
    BattleMon *attacker;
    BattleMon *target;
    BtlFlowMoveParam moveParam;
    BattleMoveEffectState savedEffect;
    BOOL result;

    attacker = GetPokeParam(handler->pokeCon, param->attackerIndex);
    target = GetPokeParam(handler->pokeCon, param->targetIndex);
    ServerEvent_GetMoveParam(handler, param->move, attacker, &moveParam);
    data_ov095_0689cb3d(handler->unk860);
    data_ov095_0689cb3d(handler->unk868);
    data_ov095_0689cb49(handler->unk868, target);
    data_ov095_0689cd49(handler->unk868, 1);
    data_ov095_0689cee5(handler->unk868);
    if (data_ov095_0689cd51(handler->unk868)) {
        func_ov093_021c5718(handler, attacker, moveParam.move);
        return FALSE;
    }
    func_ov093_021bf83c(handler, &moveParam, attacker, handler->unk868);
    func_ov093_021bf3d8(handler, &moveParam, attacker, handler->unk868, handler->unk1F88);
    func_ov093_021bf4ac(handler, &moveParam, attacker, handler->unk868, handler->unk1F88);
    func_ov093_021bf8d4(handler, &moveParam, attacker, handler->unk868);
    if (data_ov095_0689cd51(handler->unk868)) {
        return FALSE;
    }
    savedEffect = *handler->moveEffect;
    func_ov093_021bd1e0(handler->moveEffect);
    func_ov093_021bd200(handler->moveEffect, handler, attacker, handler->unk868);
    handler->moveEffect->index = 1;
    func_ov093_021c091c(handler, &moveParam, attacker, handler->unk868, handler->unk1F88, TRUE);
    result = handler->moveEffect->enabled;
    *handler->moveEffect = savedEffect;
    return result;
}






















// Function name from swan.
u8 BattleHandler_QuitBattle(BtlServerFlow *handler, BattleHandlerQuitBattleParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (ServerControl_EscapeSub(handler, mon, TRUE)) {
        handler->unk14 = 5;
        return TRUE;
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_Switch(BtlServerFlow *handler, BattleHandlerSwitchParam *param) {
    BattleMon *mon;
    u8 pos;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!ServerControl_CheckMatchup(handler) && !func_ov093_021c8374(handler, param->monIndex) && handler->unk14 == 0) {
        BattleHandler_SetString(handler, &param->firstString);
        if (ServerControl_SwitchOut(handler, mon, param->flag)) {
            pos = MonIDToBattlePos(handler->mainModule, handler->pokeCon, param->monIndex);
            RequestChangePokemon(handler->server, pos);
            BattleHandler_SetString(handler, &param->secondString);
            handler->unk14 = 1;
            return TRUE;
        }
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_BatonPass(BtlServerFlow *handler, BattleHandlerBatonPassParam *param) {
    BattleMon *source;
    BattleMon *target;
    u8 pos;

    source = GetPokeParam(handler->pokeCon, param->sourceMonIndex);
    target = GetPokeParam(handler->pokeCon, param->targetMonIndex);
    if (CheckCondition(source, 0x10)) {
        ServerEvent_GastroAcidConfirmed(handler, target);
    }
    CopyBatonPassParams(target, source);
    func_ov093_021cd4b0(handler->queue, 0x26, param->sourceMonIndex, param->targetMonIndex);
    if (IsSubstituteActive(target)) {
        pos = GetBattlePos(&handler->posPoke, param->targetMonIndex);
        func_ov093_021cd4b0(handler->queue, 0x51, pos);
    }
    return TRUE;
}




// Function name from swan.
u8 BattleHandler_Flinch(BtlServerFlow *handler, BattleHandlerFlinchParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (ServerControl_FlinchCore(handler, mon, param->flag)) {
        return TRUE;
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_Revive(BtlServerFlow *handler, BattleHandlerReviveParam *param) {
    BattleMon *mon;
    u8 pos;
    u8 target;
    u8 slot;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    HPAdd(mon, param->amount);
    func_ov093_021cd4b0(handler->queue, 2, param->monIndex, param->amount);
    handler->unk7A9[param->monIndex] = 0;
    BattleHandler_SetString(handler, &param->string);
    pos = MonIDToBattlePos(handler->mainModule, handler->pokeCon, param->monIndex);
    if (pos != BTL_POS_MAX) {
        target = func_ov093_021b8ddc(param->monIndex);
        slot = func_ov093_021b8dec(handler->mainModule, pos);
        ServerControl_SwitchInFillSlot(handler, target, slot, slot, TRUE);
        ServerControl_AfterSwitchIn(handler);
    }
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_SetWeight(BtlServerFlow *handler, BattleHandlerSetWeightParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    SetWeight(mon, param->weight);
    func_ov093_021cd4b0(handler->queue, 0x14, param->monIndex, param->weight);
    BattleHandler_SetString(handler, &param->string);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_ForceSwitch(BtlServerFlow *handler, const BattleHandlerForceSwitchParam *param) {
    BattleMon *target;
    BOOL failed;

    target = GetPokeParam(handler->pokeCon, param->targetIndex);
    return ServerControl_ForceSwitchCore(handler, GetPokeParam(handler->pokeCon, param->header.monId), target,
                                         param->forced, &failed, param->effect, param->ignoreLevel, &param->string)
             ? TRUE
             : FALSE;
}

// Function names from swan.
u8 BattleHandler_InterruptAction(BtlServerFlow *handler, BattleHandlerInterruptParam *param) {
    if (ActionOrder_InterruptReserve(handler, param->monId)) {
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_InterruptMove(BtlServerFlow *handler, BattleHandlerInterruptParam *param) {
    return ActionOrder_InterruptReserveByMove(handler, param->moveId) != 0;
}

u8 BattleHandler_SendLast(BtlServerFlow *handler, BattleHandlerInterruptParam *param) {
    if (ActionOrder_SendToLast(handler, param->monId)) {
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}

u8 BattleHandler_SwapPoke(BtlServerFlow *handler, BattleHandlerSwapPokeParam *param) {
    u8 clientId;
    BattleMon *first;
    BattleMon *second;
    BattleParty *party;
    s16 firstSlot;
    s16 secondSlot;

    if (param->firstMonIndex != param->secondMonIndex) {
        clientId = func_ov093_021b8ddc(param->firstMonIndex);
        if (clientId == func_ov093_021b8ddc(param->secondMonIndex)) {
            first = GetPokeParam(handler->pokeCon, param->firstMonIndex);
            second = GetPokeParam(handler->pokeCon, param->secondMonIndex);
            if (!IsFainted(first) && !IsFainted(second)) {
                party = GetPartyData(handler->pokeCon, clientId);
                firstSlot = FindPartyMon(party, first);
                secondSlot = FindPartyMon(party, second);
                if (firstSlot >= 0 && secondSlot >= 0) {
                    ServerControl_MoveCore(handler, clientId, firstSlot, secondSlot, 0);
                    BattleHandler_SetString(handler, &param->string);
                    ServerControl_AfterMove(handler, clientId, firstSlot, secondSlot);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

u8 BattleHandler_Transform(BtlServerFlow *flow, BattleHandlerTransformParam *param) {
    BattleMon *mon;
    BattleMon *target;
    u16 ability;
    u8 monId;
    u8 targetId;
    u32 state;

    mon = GetPokeParam(flow->pokeCon, param->monIndex);
    target = GetPokeParam(flow->pokeCon, param->targetIndex);
    if (!IsIllusionEnabled(mon) && !IsIllusionEnabled(target) && !IsSemiInvulnMove(mon)) {
        ability = GetBattleMonStat(mon, 0x10);
        if (TransformSet(mon, target)) {
            monId = GetMonID(mon);
            targetId = GetMonID(target);
            if (param->popup) {
                ServerDisplay_AbilityPopupAdd(flow, mon);
            }
            RemoveForceAll(mon);
            AbilityEvent_RemoveItem(mon);
            AbilityEvent_AddItem(mon);
            func_ov093_021cd4b0(flow->queue, 0x53, monId, targetId);
            BattleHandler_SetString(flow, &param->string);
            if (param->popup) {
                ServerDisplay_AbilityPopupRemove(flow, mon);
            }
            if (ability != GetBattleMonStat(mon, 0x11)) {
                state = PushState(&flow->actionState, 0x3f0a);
                func_ov093_021c7b7c(flow, monId);
                PopState(&flow->actionState, state, 0x3f0c);
            }
            return TRUE;
        }
    }
    return FALSE;
}


u8 BattleHandler_IllusionBreak(BtlServerFlow *handler, BattleHandlerIllusionBreakParam *param) {
    BattleMon *mon;

    if (DoesBattleMonExist(&handler->posPoke, param->monIndex)) {
        mon = GetPokeParam(handler->pokeCon, param->monIndex);
        if (IsIllusionEnabled(mon)) {
            IllusionBreak(mon);
            func_ov093_021cd4b0(handler->queue, 0x4b, param->monIndex);
            BattleHandler_SetString(handler, &param->string);
            return TRUE;
        }
    }
    return FALSE;
}




u8 BattleHandler_GravityCheck(BtlServerFlow *handler, BattleHandlerGravityCheckParam *param) {
    u8 monIds[6];
    u8 count;
    u8 i;
    BattleMon *mon;
    u32 changed;
    u16 code;
    u8 pos;

    pos = MonIDToBattlePos(handler->mainModule, handler->pokeCon, param->monIndex);
    code = (2 << 10) | pos;
    count = HandlerGetAlivePartyCount(handler, code, monIds);
    for (i = 0; i < count; i++) {
        mon = GetPokeParam(handler->pokeCon, monIds[i]);
        changed = FALSE;
        if (GetAdditionalConditionFlag(mon, 3)) {
            ServerControl_HideTurnCancel(handler, mon, 3);
            if (BtlFlow_GetSkyDropTarget(mon) != 0x1f) {
                GFL_STD_MemCmp(handler, mon, 1);
            }
            changed = TRUE;
        }
        if (ServerEvent_CheckFloating(handler, mon, 1)) {
            changed = TRUE;
        }
        if (CheckCondition(mon, 0x1e)) {
            ServerControl_CureCondition(handler, mon, 0x1e, 0);
            changed = TRUE;
        }
        if (CheckCondition(mon, 0x20)) {
            ServerControl_CureCondition(handler, mon, 0x20, 0);
            changed = TRUE;
        }
        if (changed) {
            func_ov093_021cd64c(handler->queue, 0x5b, 0x43b, monIds[i], 0xffff0000);
        }
    }
    return TRUE;
}

u8 BattleHandler_HideTurnCancel(BtlServerFlow *handler, BattleHandlerHideTurnParam *param) {
    BattleMon *mon;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon) && ServerControl_HideTurnCancel(handler, mon, param->flag)) {
        BattleHandler_SetString(handler, &param->string);
        return TRUE;
    }
    return FALSE;
}

// Function name from swan.
u8 BattleHandler_EffectAtPos(BtlServerFlow *handler, const BattleHandlerEffectAtPosParam *param) {
    if (param->hideMessageWindow) {
        func_ov093_021cd4b0(handler->queue, 0x56, 0);
    }
    ServerControl_ViewEffect(handler, param->effect, param->pos1, param->pos2, param->reserved, param->reserve);
    BattleHandler_SetString(handler, &param->string);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_RemoveMessageWindow(BtlServerFlow *handler, BattleHandlerHeader *header) {
    func_ov093_021cd4b0(handler->queue, 0x56, 0);
    return TRUE;
}

// Function name from swan.
u8 BattleHandler_ChangeForm(BtlServerFlow *handler, BattleHandlerChangeFormParam *param) {
    BattleMon *mon;
    u8 currentForm;

    mon = GetPokeParam(handler->pokeCon, param->monIndex);
    if (!IsFainted(mon) && !TransformCheck(mon)) {
        currentForm = GetBattleMonStat(mon, 0x13);
        if (currentForm != param->form) {
            if (param->popup) {
                ServerDisplay_AbilityPopupAdd(handler, mon);
            }
            ChangeForm(mon, param->form);
            func_ov093_021cd4b0(handler->queue, 0x4f, param->monIndex, param->form);
            BattleHandler_SetString(handler, &param->string);
            if (param->popup) {
                ServerDisplay_AbilityPopupRemove(handler, mon);
            }
            return TRUE;
        }
    }
    return FALSE;
}

u8 BattleHandler_SetMoveEffectIndex(BtlServerFlow *handler, BattleHandlerMoveEffectParam *param) {
    handler->moveEffect->index = param->index;
    return TRUE;
}

u8 BattleHandler_SetMoveEffectEnable(BtlServerFlow *handler, BattleHandlerHeader *header) {
    if (!handler->moveEffect->enabled) {
        handler->moveEffect->enabled = 1;
    }
    return TRUE;
}


