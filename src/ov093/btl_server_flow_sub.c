// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_server_flow_sub.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_ability.h"
#include "battle/btl_event.h"
#include "battle/btl_faintrec.h"
#include "battle/btl_handler.h"
#include "battle/btl_handler_work.h"
#include "battle/btl_main.h"
#include "battle/btl_monset.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_pospoke.h"
#include "battle/btl_server.h"
#include "battle/btl_server_cmd.h"
#include "battle/btl_server_flow.h"
#include "battle/btl_server_flow_sub.h"
#include "battle/btl_tables.h"
#include "battle/handler_common.h"
#include "battle/side_handlers.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/types.h"
#include "gfl/std.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "save/high_link.h"
#include "save/player_info.h"
#include "system/rtc.h"
#include "battle/btl_setup.h"
#include "constants/battle.h"
#include "field/zone.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"

static u8 func_ov093_021ca8f8(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets);
static u8 func_ov093_021caa24(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, u8 pos, u8 targetPos);
static u8 func_ov093_021caa58(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets);
static u8 func_ov093_021cacf4(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets);
static BOOL func_ov093_021cafd8(BtlServerFlow *flow, u32 style, BattleMon *mon, BtlFlowMoveParam *param, u8 target,
                                void *targets);
static BattleMon *func_ov093_021cb174(BtlServerFlow *flow, u8 pos, u8 index);
static BattleMon *func_ov093_021cb1a4(BtlServerFlow *flow, u8 pos);
static u8 func_ov093_021cb1d4(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u8 targetId);
static u8 func_ov093_021cb214(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param);
static u8 func_ov093_021cb274(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u8 targetId);
static u32 ScaleExpGainedByLevel(BattleMon *mon, u32 exp, u16 level, u16 defeatedLevel);
static void AddEVs(BattleMon *mon, BattleMon *defeated, BtlFlowExpEntry *entry);
static BOOL func_ov093_021cba40(BtlServerFlow *flow, BattleMon *mon, u16 item);
static BOOL func_ov093_021cbb34(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 item, u8 *shakes,
                                u8 *critical);
static fx32 func_ov093_021cbcf4(BtlServerFlow *flow);
static fx32 func_ov093_021cbd48(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 item);
static BOOL func_ov093_021cbea8(BtlServerFlow *flow, fx32 value);
static BOOL func_ov093_021cbf30(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf40(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf50(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf60(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf70(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf80(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbf90(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cbfa0(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc008(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc098(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0a8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0b8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0c8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0d8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0e8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc0f8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc178(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 slot);
static BOOL func_ov093_021cc214(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc2c0(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc350(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u32 condition);
static BOOL func_ov093_021cc398(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc438(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u32 stat);
static BOOL func_ov093_021cc4ac(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static BOOL func_ov093_021cc508(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static u32 func_ov093_021cc768(BtlServerFlow *flow, const u8 *alive, const u8 *total, u8 side);
static void func_ov093_021cc800(BtlServerFlow *flow, s32 *hp, s32 *percent, u8 side);


// Fills targets with the mons a move hits, from its target type and the battle style
u8 func_ov093_021ca7f4(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, void *targets) {
    u32 style;
    u8 redirect;
    u8 count;
    BattleMon *first;

    style = BtlSetup_GetBattleStyle(flow->mainModule);
    redirect = 0x1f;
    if (!param->flags.unk0) {
        redirect = func_ov093_021cb214(flow, mon, param);
    }
    data_ov095_0689cb3d(targets);
    flow->unk77F = 6;
    switch (style) {
    case BTL_STYLE_SINGLE:
    default:
        count = func_ov093_021ca8f8(flow, mon, target, param, redirect, targets);
        break;
    case BTL_STYLE_DOUBLE:
        count = func_ov093_021caa58(flow, mon, target, param, redirect, targets);
        break;
    case BTL_STYLE_TRIPLE:
        count = func_ov093_021cacf4(flow, mon, target, param, redirect, targets);
        break;
    case BTL_STYLE_ROTATION:
        count = func_ov093_021ca8f8(flow, mon, target, param, redirect, targets);
        break;
    }
    if (count == 1) {
        first = data_ov095_0689cc71(targets, 0);
        if (first != NULL) {
            flow->unk77F = data_ov095_0689d5f5(&flow->posPoke, GetMonID(first));
        }
        if (!func_ov093_021cafd8(flow, style, mon, param, target, targets)) {
            count = 0;
        }
    }
    return count;
}


#pragma thumb reset


#pragma thumb reset


// The targets in a single or rotation battle
static u8 func_ov093_021ca8f8(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets) {
    u8 pos;

    pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
    switch (param->targetType) {
    case 0:
    case 3:
    case 4:
    case 5:
    case 9:
        data_ov095_0689cb49(targets, redirect == 0x1f ? func_ov093_021cb174(flow, pos, 0)
                                                      : GetPokeParam(flow->pokeCon, redirect));
        return 1;
    case 1:
    case 7:
        if (redirect == 0x1f) {
            data_ov095_0689cb49(targets, mon);
        } else {
            data_ov095_0689cb49(targets, GetPokeParam(flow->pokeCon, redirect));
        }
        return 1;
    case 8:
        data_ov095_0689cb49(targets, mon);
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 0));
        return 2;
    case 13:
        if (redirect != 0x1f) {
            data_ov095_0689cb49(targets, GetPokeParam(flow->pokeCon, redirect));
            return 1;
        }
        return 0;
    }
    return 0;
}


#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset


#pragma thumb reset


// Curse picks its own target: the user itself, or a foe for a Ghost type
static u8 func_ov093_021caa24(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, u8 pos, u8 targetPos) {
    u16 move = param->move;

    if (move == MOVE_CURSE) {
        if (param->targetType == 0) {
            if (pos == targetPos || targetPos == 6) {
                return func_ov093_021d80bc(flow->mainModule, flow->pokeCon, mon, MOVE_CURSE);
            }
        } else if (param->targetType == 7 && pos != targetPos) {
            return pos;
        }
    }
    return targetPos;
}

// The targets in a double battle
static u8 func_ov093_021caa58(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets) {
    u8 pos;
    u8 targetPos;
    BattleMon *single;
    u8 newTarget;

    pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
    targetPos = func_ov093_021caa24(flow, param, mon, pos, target);
    switch (param->targetType) {
    case 0:
        single = func_ov093_021b98a4(flow->pokeCon, targetPos);
        break;
    case 3:
        single = func_ov093_021b98a4(flow->pokeCon, targetPos);
        break;
    case 9:
        single = func_ov093_021cb174(flow, pos, BattleRandom(2));
        break;
    case 5:
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 0));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 1));
        return 2;
    case 4:
        data_ov095_0689cb49(targets, func_ov093_021cb1a4(flow, pos));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 0));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 1));
        return 3;
    case 8:
        data_ov095_0689cb49(targets, mon);
        data_ov095_0689cb49(targets, func_ov093_021cb1a4(flow, pos));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 0));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 1));
        return 3;
    case 7:
        if (redirect == 0x1f) {
            data_ov095_0689cb49(targets, mon);
        } else {
            data_ov095_0689cb49(targets, GetPokeParam(flow->pokeCon, redirect));
        }
        return 1;
    case 1:
        data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, targetPos));
        return 1;
    case 2:
        data_ov095_0689cb49(targets, func_ov093_021cb1a4(flow, pos));
        return 1;
    case 13:
        if (redirect != 0x1f) {
            single = GetPokeParam(flow->pokeCon, redirect);
            break;
        }
        return 0;
    default:
        return 0;
    }
    if (single != NULL) {
        newTarget = func_ov093_021cb1d4(flow, mon, param, GetMonID(single));
        if (newTarget != 0x1f) {
            single = GetPokeParam(flow->pokeCon, newTarget);
        }
        data_ov095_0689cb49(targets, single);
        return 1;
    }
    return 0;
}


#pragma thumb reset


#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset


// The targets in a triple battle, where only the mons next to each other reach each other
static u8 func_ov093_021cacf4(BtlServerFlow *flow, BattleMon *mon, u8 target, BtlFlowMoveParam *param, u8 redirect,
                              void *targets) {
    u8 pos;
    const AdjacentOpponentData *adjacent;
    u8 targetPos;
    BattleMon *single;
    u32 i;
    u32 j;
    u32 count;
    u8 newTarget;
    u8 index;

    pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
    adjacent = func_ov093_021b99e0(pos);
    targetPos = func_ov093_021caa24(flow, param, mon, pos, target);
    switch (param->targetType) {
    case 0:
        single = func_ov093_021b98a4(flow->pokeCon, targetPos);
        break;
    case 3:
        single = func_ov093_021b98a4(flow->pokeCon, targetPos);
        break;
    case 9:
        index = BattleRandom(adjacent->count1);
        single = func_ov093_021b98a4(flow->pokeCon, adjacent->list1[index]);
        break;
    case 5:
        for (i = 0; i < adjacent->count1; i++) {
            data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, adjacent->list1[i]));
        }
        return adjacent->count1;
    case 4:
        count = 0;
        for (i = 0; i < adjacent->count1; i++) {
            data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, adjacent->list1[i]));
            count++;
        }
        for (j = 0; j < adjacent->count2; j++) {
            if (pos != adjacent->list2[j]) {
                data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, adjacent->list2[j]));
                count++;
            }
        }
        return count;
    case 8:
        data_ov095_0689cb49(targets, mon);
        for (i = 0; i < 3; i++) {
            u8 allyPos = func_ov093_021b8c38(pos, i);

            if (allyPos != pos) {
                data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, allyPos));
            }
        }
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 0));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 1));
        data_ov095_0689cb49(targets, func_ov093_021cb174(flow, pos, 2));
        return 6;
    case 7:
        if (redirect == 0x1f) {
            data_ov095_0689cb49(targets, mon);
        } else {
            data_ov095_0689cb49(targets, GetPokeParam(flow->pokeCon, redirect));
        }
        return 1;
    case 1:
        data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, targetPos));
        return 1;
    case 2:
        data_ov095_0689cb49(targets, func_ov093_021b98a4(flow->pokeCon, targetPos));
        return 1;
    case 13:
        if (redirect != 0x1f) {
            single = GetPokeParam(flow->pokeCon, redirect);
            break;
        }
        return 0;
    default:
        return 0;
    }
    if (single != NULL) {
        newTarget = func_ov093_021cb1d4(flow, mon, param, GetMonID(single));
        if (newTarget != 0x1f) {
            single = GetPokeParam(flow->pokeCon, newTarget);
        }
        data_ov095_0689cb49(targets, single);
        return 1;
    }
    return 0;
}


// When a single target has fainted before the move, the move goes to the nearest foe still standing instead, or the
// one with more HP, or a random one of two
static BOOL func_ov093_021cafd8(BtlServerFlow *flow, u32 style, BattleMon *mon, BtlFlowMoveParam *param, u8 target,
                                void *targets) {
    BattleMon *fainted;
    u8 pos;
    u8 numPositions;
    u8 positions[8];
    BattleMon *mons[3];
    u8 i;
    u8 count;
    s32 dist0;
    s32 dist1;
    u8 index;
    u16 code;

    if (func_ov093_021d7f00(style) > 1 && data_ov095_0689cd39(targets) == 1) {
        fainted = data_ov095_0689cc71(targets, 0);
        if (IsFainted(fainted) && !IsAllyMonID(GetMonID(mon), GetMonID(fainted))) {
            pos = MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon));
            data_ov095_0689cc15(targets, fainted);
            if (getMoveFlag(param->move, 0xb)) {
                code = 0x600 | pos;
            } else {
                code = 0x100 | pos;
            }
            numPositions = func_ov093_021b8778(flow->mainModule, code, positions);
            for (i = 0; i < 3; i++) {
                mons[i] = NULL;
            }
            for (count = 0, i = 0; i < numPositions; i++) {
                mons[count] = func_ov093_021b98a4(flow->pokeCon, positions[i]);
                if (!IsFainted(mons[count])) {
                    positions[count] = positions[i];
                    count++;
                }
            }
            if (count == 0) {
                return FALSE;
            }
            if (count != 2) {
                index = 0;
            } else {
                dist0 = positions[0] - target;
                if (dist0 < 0) {
                    dist0 *= -1;
                }
                dist1 = positions[1] - target;
                if (dist1 < 0) {
                    dist1 *= -1;
                }
                if (dist0 == dist1) {
                    dist0 = GetBattleMonStat(mons[0], 0xd);
                    dist1 = GetBattleMonStat(mons[1], 0xd);
                }
                if (dist0 < dist1) {
                    index = 0;
                } else if (dist1 < dist0) {
                    index = 1;
                } else {
                    index = BattleRandom(2);
                }
            }
            if (mons[index] != NULL) {
                data_ov095_0689cb49(targets, mons[index]);
                return TRUE;
            }
        }
    }
    return FALSE;
}


// The mon on the other side at an index from a position
static BattleMon *func_ov093_021cb174(BtlServerFlow *flow, u8 pos, u8 index) {
    u8 clientId;
    u8 slot;

    func_ov093_021b8e28(flow->mainModule, func_ov093_021b8c50(flow->mainModule, pos, index), &clientId, &slot);
    return func_ov093_021b9bf0(func_ov093_021bb85c(flow->server, clientId)->party, slot);
}

// The mon beside a position
static BattleMon *func_ov093_021cb1a4(BtlServerFlow *flow, u8 pos) {
    u8 clientId;
    u8 slot;

    func_ov093_021b8e28(flow->mainModule, func_ov093_021b8cb0(flow->mainModule, pos), &clientId, &slot);
    return func_ov093_021b9bf0(func_ov093_021bb85c(flow->server, clientId)->party, slot);
}

static u8 func_ov093_021cb1d4(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u8 targetId) {
    u32 state;
    u8 result;

    state = PushState(&flow->actionState, 0x263);
    result = func_ov093_021cb274(flow, mon, param, targetId);
    PopState(&flow->actionState, state, 0x265);
    return result;
}

// The mon that draws a move to itself, as Follow Me and Lightning Rod do, or 0x1f for none
static u8 func_ov093_021cb214(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param) {
    u8 monId;

    BattleEventVar_Push(0x277);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(4, 0x1f);
    BattleEventVar_SetRewriteOnceValue(0x50, FALSE);
    BattleEvent_CallHandlers(flow, 0x29);
    monId = BattleEventVar_GetValue(4);
    BattleEventVar_Pop(0x27f);
    return monId;
}

// The mon that takes a move over from its target, as with Lightning Rod, or 0x1f for none
static u8 func_ov093_021cb274(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u8 targetId) {
    u8 newTarget;

    BattleEventVar_Push(0x290);
    BattleEventVar_SetConstValue(3, GetMonID(mon));
    BattleEventVar_SetConstValue(0x16, param->type);
    BattleEventVar_SetConstValue(0x12, param->move);
    BattleEventVar_SetRewriteOnceValue(4, targetId);
    BattleEvent_CallHandlers(flow, 0x2a);
    newTarget = BattleEventVar_GetValue(4);
    BattleEventVar_Pop(0x297);
    if (newTarget == targetId) {
        newTarget = 0x1f;
    }
    return newTarget;
}

void AddExpAndEVs(BtlServerFlow *flow, BattleParty *party, BattleMon *defeated, BtlFlowExpEntry *entries) {
    u32 baseExp;
    u16 numMons;
    u16 numExpShare;
    u16 i;
    BattleMon *mon;
    u32 shareExp;
    u8 numFaced;
    u8 numStanding;
    u32 exp;
    PlayerInfo *player;
    PartyPkm *pkm;
    u16 level;
    u16 defeatedLevel;
    BtlFlowExpEntry *entry;
    u8 monId;
    u8 k;

    baseExp = CalcBaseExpGain(defeated);
    numMons = GetNumMonsInParty(party);
    numExpShare = 0;
    if (BtlSetup_GetBattleType(flow->mainModule) == 1) {
        baseExp = baseExp * 15 / 10;
    }
    for (i = 0; i < 6; i++) {
        MI_CpuFill8(&entries[i], 0, sizeof(BtlFlowExpEntry));
    }
    for (i = 0; i < numMons; i++) {
        mon = GetBattleMonFromParty(party, i);
        if (!IsFainted(mon) && GetBattleMonHeldItem(mon) == ITEM_EXP_SHARE) {
            numExpShare++;
        }
    }
    if (numExpShare != 0) {
        shareExp = baseExp / 2;
        baseExp -= shareExp;
        shareExp /= numExpShare;
        if (shareExp == 0) {
            shareExp = 1;
        }
        for (i = 0; i < numMons; i++) {
            mon = GetBattleMonFromParty(party, i);
            if (!IsFainted(mon) && GetBattleMonHeldItem(mon) == ITEM_EXP_SHARE) {
                entries[i].exp = shareExp;
            }
        }
    }
    numFaced = func_ov093_021d6e04(defeated);
    numStanding = 0;
    for (i = 0; i < numFaced; i++) {
        if (!IsFainted(GetPokeParam(flow->pokeCon, func_ov093_021d6e0c(defeated, i)))) {
            numStanding++;
        }
    }
    exp = baseExp / numStanding;
    if (exp == 0) {
        exp = 1;
    }
    for (defeatedLevel = 0; defeatedLevel < numMons; defeatedLevel++) {
        mon = GetBattleMonFromParty(party, defeatedLevel);
        if (!IsFainted(mon)) {
            monId = GetMonID(mon);
            for (k = 0; k < numFaced; k++) {
                if (monId == func_ov093_021d6e0c(defeated, k)) {
                    entries[defeatedLevel].exp += exp;
                }
            }
        }
    }
    for (i = 0; i < numMons; i++) {
        entry = &entries[i];
        if (entry->exp != 0) {
            player = func_ov093_021b871c(flow->mainModule);
            mon = GetBattleMonFromParty(party, i);
            pkm = GetSrcData(mon);
            level = GetBattleMonStat(mon, 0xf);
            defeatedLevel = GetBattleMonStat(defeated, 0xf);
            entry->exp = ScaleExpGainedByLevel(mon, entry->exp, level, defeatedLevel);
            if (!IsTrainerOT(pkm, player)) {
                entry->exp = fixed_round(entry->exp,
                                         PokeParty_GetParam(pkm, 0xc, NULL) != TrainerInfo_GetRegion(player) ? 0x1b33
                                                                                                            : 0x1800);
                entry->boosted = TRUE;
            }
            if (GetBattleMonHeldItem(mon) == ITEM_LUCKY_EGG) {
                entry->exp = fixed_round(entry->exp, 0x1800);
                entry->boosted = TRUE;
            }
            entry->exp = PassPower_ApplyEXP(entry->exp);
        }
    }
    for (i = 0; i < numMons; i++) {
        if (entries[i].exp != 0) {
            AddEVs(func_ov093_021b9bf0(party, i), defeated, &entries[i]);
        }
    }
}


// Function name from swan.
static u32 ScaleExpGainedByLevel(BattleMon *mon, u32 exp, u16 level, u16 defeatedLevel) {
    u32 num = defeatedLevel * 2 + 10;
    u32 den = defeatedLevel + level + 10;
    fx32 denSqrt;
    u32 result;
    u32 max;

    num = (FX_Sqrt(FX32_CONST(num)) * (num * num)) >> FX32_SHIFT;
    denSqrt = FX_Sqrt(FX32_CONST(den));
    den = den * den;
    result = exp * num / ((den * denSqrt) >> FX32_SHIFT) + 1;
    max = GetExpForLv100(mon);
    if (result > max) {
        result = max;
    }
    return result;
}

// For each stat: its effort value in the personal data, its field in the party data, and its Power item
typedef struct {
    u8 personalParam;
    u16 field;
    u16 powerItem;
} BtlFlowEVParam;

static const BtlFlowEVParam data_ov093_021efdc0[6] = {
    { 0x0a, 0x0d, 0x126 },
    { 0x0b, 0x0e, 0x121 },
    { 0x0c, 0x0f, 0x122 },
    { 0x0d, 0x10, 0x125 },
    { 0x0e, 0x11, 0x123 },
    { 0x0f, 0x12, 0x124 },
};

// Function name from swan.
static void AddEVs(BattleMon *mon, BattleMon *defeated, BtlFlowExpEntry *entry) {
    u16 species;
    u16 form;
    u8 evs[6];
    u8 i;
    PartyPkm *pkm;
    BOOL wasEncrypted;
    u32 field;
    u32 total;

    species = GetBattleMonSpecies(defeated);
    form = GetBattleMonStat(defeated, 0x13);
    for (i = 0; i < 6; i++) {
        evs[i] = PML_PersonalGetParamSingle(species, form, data_ov093_021efdc0[i].personalParam);
    }
    if (GetBattleMonHeldItem(mon) == ITEM_MACHO_BRACE) {
        for (i = 0; i < 6; i++) {
            evs[i] *= 2;
        }
    }
    for (i = 0; i < 6; i++) {
        u16 item = data_ov093_021efdc0[i].powerItem;

        if (item == GetBattleMonHeldItem(mon)) {
            evs[i] += (u8)ItemGetParam(item, 2);
        }
    }
    pkm = GetSrcData(mon);
    wasEncrypted = PokeParty_DecryptPkm(pkm);
    if (PokeParty_GetParam(pkm, 0x97, NULL)) {
        for (i = 0; i < 6; i++) {
            evs[i] *= 2;
        }
    }
    for (i = 0; i < 6; i++) {
        if (evs[i] != 0) {
            field = data_ov093_021efdc0[i].field;
            total = evs[i] + PokeParty_GetParam(pkm, field, NULL);
            if (total > 255) {
                total = 255;
            }
            PokeParty_SetParam(pkm, field, total);
            switch (i) {
            case 0:
                entry->hp = evs[i];
                break;
            case 1:
                entry->attack = evs[i];
                break;
            case 2:
                entry->defense = evs[i];
                break;
            case 3:
                entry->speed = evs[i];
                break;
            case 4:
                entry->spAttack = evs[i];
                break;
            case 5:
                entry->spDefense = evs[i];
                break;
            }
        }
    }
    PokeParty_EncryptPkm(pkm, wasEncrypted);
}

// An effect of the items a trainer uses from the bag, which applies to the items whose data has the parameter, or to
// the item itself when exact is set
typedef struct {
    u16 param;
    // 0 for any mon, 1 for one in battle, 2 only where any item can be used
    u8 place;
    u8 exact;
    BOOL (*func)(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
} BtlFlowItemEffect;

BOOL func_ov093_021cc5f4(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
BOOL func_ov093_021cc558(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param);
static const BtlFlowItemEffect data_ov093_021efde4[23] = {
    { 0x12, 0, FALSE, func_ov093_021cbf30 },
    { 0x13, 0, FALSE, func_ov093_021cbf40 },
    { 0x14, 0, FALSE, func_ov093_021cbf50 },
    { 0x15, 0, FALSE, func_ov093_021cbf60 },
    { 0x16, 0, FALSE, func_ov093_021cbf70 },
    { 0x17, 0, FALSE, func_ov093_021cbf80 },
    { 0x18, 0, FALSE, func_ov093_021cbf90 },
    { 0x19, 1, FALSE, func_ov093_021cbfa0 },
    { 0x1a, 0, FALSE, func_ov093_021cc008 },
    { 0x1e, 1, FALSE, func_ov093_021cc098 },
    { 0x1f, 1, FALSE, func_ov093_021cc0a8 },
    { 0x20, 1, FALSE, func_ov093_021cc0b8 },
    { 0x21, 1, FALSE, func_ov093_021cc0c8 },
    { 0x22, 1, FALSE, func_ov093_021cc0d8 },
    { 0x23, 1, FALSE, func_ov093_021cc0e8 },
    { 0x24, 1, FALSE, func_ov093_021cc0f8 },
    { 0x27, 0, FALSE, func_ov093_021cc178 },
    { 0x28, 0, FALSE, func_ov093_021cc214 },
    { 0x29, 0, FALSE, func_ov093_021cc2c0 },
    { ITEM_ITEM_URGE, 2, TRUE, func_ov093_021cc4ac },
    { ITEM_ABILITY_URGE, 2, TRUE, func_ov093_021cc508 },
    { ITEM_ITEM_DROP, 2, TRUE, func_ov093_021cc558 },
    { ITEM_RESET_URGE, 2, TRUE, func_ov093_021cc5f4 },
};

// A trainer uses an item from the bag on a party mon
u8 func_ov093_021cb730(BtlServerFlow *flow, BattleMon *mon, u16 item, u8 param, u8 slot) {
    u8 numOut;
    BattleParty *party;
    u8 clientId;
    u8 targetId;
    u8 place;
    BattleMon *target;
    u32 state;
    u8 targetPos;
    BOOL used;
    u32 reserve;
    BOOL usable;
    u32 placed;
    u32 i;
    const BtlFlowItemEffect *effect;
    BOOL result;
    s32 value;
    u16 effectId;

    clientId = func_ov093_021b8ddc(GetMonID(mon));
    target = NULL;
    placed = 0;
    if (slot != 6) {
        party = GetPartyData(flow->pokeCon, clientId);
        numOut = func_ov093_021b99b8(flow->mainModule, clientId);
        target = func_ov093_021b9bf0(party, slot);
        targetId = GetMonID(target);
        targetPos = GetBattlePos(&flow->posPoke, targetId);
        // 0 for a mon in battle, 1 for one in a rotation battle's back slots, 2 for one in the party
        if (slot >= numOut) {
            placed = 2;
        }
        place = placed;
        if (BtlSetup_GetBattleStyle(flow->mainModule) == BTL_STYLE_ROTATION && place == 2 && slot < 3) {
            place = 1;
        }
    }
    if (flow->unk18 != 1) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x21, clientId, item, 0xffff0000);
        if (ItemGetParam(item, 0xf) == 4) {
            if (func_ov093_021cba40(flow, mon, item)) {
                BattleClient_SubItem(flow->mainModule, clientId, item);
            }
            return FALSE;
        }
        if (ItemGetParam(item, 7) == 3) {
            BattleClient_SubItem(flow->mainModule, clientId, item);
            return TRUE;
        }
    } else {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x24, clientId, targetId, item, 0xffff0000);
        if (clientId == GetPlayerClientID(flow->mainModule)) {
            func_ov093_021ba1b4(flow->mainModule, 0x6f);
        }
    }
    usable = place == 0 && targetPos != 6 ? TRUE : FALSE;
    if (!usable) {
        for (i = 0; i < 23; i++) {
            effect = &data_ov093_021efde4[i];
            if ((effect->exact && item == effect->param) || (!effect->exact && ItemGetParam(item, effect->param))) {
                if (effect->place == 0) {
                    usable = TRUE;
                    break;
                }
                if (effect->place == 1 && place < 2 && targetPos != 6) {
                    usable = TRUE;
                    break;
                }
            }
        }
    }
    if (!usable) {
        func_ov093_021cd64c(flow->queue, 0x5a, 0xc4, 0xffff0000);
        return FALSE;
    }
    if (flow->unk18 == 1) {
        if (targetPos != 6) {
            func_ov093_021cd4b0(flow->queue, 0x4d, targetPos, 0x282);
        }
        for (i = 0; i < 23; i++) {
            if (data_ov093_021efde4[i].exact && item == data_ov093_021efde4[i].param) {
                result = FALSE;
                if (!func_ov093_021c8338(flow, targetId)) {
                    u32 reservePos = SCQUE_RESERVE_Pos(flow->queue, 0x4d);

                    result = data_ov093_021efde4[i].func(flow, target, item, 0, param);
                    if (result == TRUE) {
                        func_ov093_021cd568(flow->queue, reservePos, 0x4d, targetPos, 0x25e);
                        func_ov093_021cd4b0(flow->queue, 0x4c, 0x236);
                        return FALSE;
                    }
                }
                if (result == FALSE) {
                    func_ov093_021cd64c(flow->queue, 0x5a, 0x44, 0xffff0000);
                }
                func_ov093_021cd4b0(flow->queue, 0x4c, 0x236);
                return FALSE;
            }
        }
    }
    used = FALSE;
    reserve = SCQUE_RESERVE_Pos(flow->queue, 0x4d);
    state = PushState(&flow->actionState, 0x4c1);
    for (i = 0; i < 23; i++) {
        effect = &data_ov093_021efde4[i];
        value = ItemGetParam(item, data_ov093_021efde4[i].param);
        if (value != 0 && effect->func(flow, target, item, value, param)) {
            used = TRUE;
        }
    }
    if (used) {
        effectId = targetPos != 6 ? 0x25e : 0x292;
        func_ov093_021cd568(flow->queue, reserve, 0x4d, targetPos, effectId);
        if (flow->unk18 != 1) {
            BattleClient_SubItem(flow->mainModule, clientId, item);
            func_ov093_021ba250(flow->mainModule, target, item);
        }
    } else {
        func_ov093_021cd64c(flow->queue, 0x5a, 0x44, 0xffff0000);
    }
    PopState(&flow->actionState, state, 0x4de);
    func_ov093_021cd4b0(flow->queue, 0x4c, 0x236);
    return FALSE;
}


// Throws a ball at the first foe standing; only a wild one can be caught
static BOOL func_ov093_021cba40(BtlServerFlow *flow, BattleMon *mon, u16 item) {
    u8 positions[6];
    u8 shakes;
    u8 critical;
    BattleMon *target;
    u8 targetPos;
    u8 numPositions;
    u8 i;
    u8 caught;
    u8 isNew;

    target = NULL;
    targetPos = 6;
    numPositions = func_ov093_021b8778(flow->mainModule,
                                       0x600 | MonIDToBattlePos(flow->mainModule, flow->pokeCon, GetMonID(mon)),
                                       positions);
    for (i = 0; i < numPositions; i++) {
        target = func_ov093_021b98a4(flow->pokeCon, positions[i]);
        if (!IsFainted(target)) {
            targetPos = positions[i];
            break;
        }
    }
    if (BtlSetup_GetBattleType(flow->mainModule) == 0) {
        if (targetPos != 6) {
            caught = func_ov093_021cbb34(flow, mon, target, item, &shakes, &critical);
            if (caught) {
                flow->unk14 = 6;
                flow->unk784 = targetPos;
                isNew = !func_ov093_021b8724(flow->mainModule, target) ? TRUE : FALSE;
                func_ov093_021d6e24(target, item);
            } else {
                isNew = FALSE;
            }
            func_ov093_021cd4b0(flow->queue, 0x46, targetPos, shakes, caught, isNew, critical, item);
        }
        return TRUE;
    }
    if (targetPos != 6) {
        func_ov093_021cd4b0(flow->queue, 0x47, targetPos, item);
    }
    return FALSE;
}

// Whether a ball catches its target, with how many times it shakes and whether it was a critical capture
static BOOL func_ov093_021cbb34(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 item, u8 *shakes,
                                u8 *critical) {
    u32 maxHP;
    u32 base;
    fx32 rate;
    u16 species;
    u16 form;
    fx32 value;
    u32 numShakes;
    u32 i;

    *critical = FALSE;
    if (item == ITEM_MASTER_BALL) {
        *shakes = 3;
        return TRUE;
    }
    maxHP = GetBattleMonStat(target, 0xe) * 3;
    base = maxHP - GetBattleMonStat(target, 0xd) * 2;
    rate = FX32_CONST(base);
    if (BtlSetup_IsBattleType(flow->mainModule, 0x20)) {
        rate = FX_MUL(rate, func_ov093_021cbcf4(flow));
    }
    species = GetBattleMonSpecies(target);
    form = GetBattleMonStat(target, 0x13);
    rate *= (u16)PML_PersonalGetParamSingle(species, form, 8);
    rate = (u32)FX_MUL(rate, func_ov093_021cbd48(flow, mon, target, item)) / maxHP;
    switch (GetBattleMonStatus(target)) {
    case 2:
    case 3:
        rate = FX_MUL(rate, FX32_CONST(2.5));
        break;
    case 1:
    case 4:
    case 5:
        rate = FX_MUL(rate, FX32_CONST(1.5));
        break;
    }
    value = PassPower_ApplyCapture(rate);
    *critical = func_ov093_021cbea8(flow, value);
    if (value >= FX32_CONST(255)) {
        *shakes = *critical ? 1 : 3;
        return TRUE;
    }
    numShakes = *critical ? 1 : 3;
    value = FX_Div(FX32_CONST(65536), FX_Sqrt(FX_Sqrt(FX_Div(FX32_CONST(255), value)))) >> FX32_SHIFT;
    *shakes = 0;
    for (i = 0; i < numShakes; i++) {
        if (BattleRandom(65536) < value) {
            (*shakes)++;
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

// In dark grass, catching gets harder the fewer species the player has caught
static fx32 func_ov093_021cbcf4(BtlServerFlow *flow) {
    u32 count = func_ov093_021b873c(flow->mainModule);

    if (count > 600) {
        return FX32_ONE;
    }
    if (count > 450) {
        return FX32_CONST(0.9);
    }
    if (count > 300) {
        return FX32_CONST(0.8);
    }
    if (count > 150) {
        return FX32_CONST(0.7);
    }
    if (count > 30) {
        return FX32_CONST(0.5);
    }
    return FX32_CONST(0.3);
}

// The catch rate multiplier of a ball
static fx32 func_ov093_021cbd48(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 item) {
    switch (item) {
    case ITEM_GREAT_BALL:
        return FX32_CONST(1.5);
    case ITEM_ULTRA_BALL:
        return FX32_CONST(2);
    case ITEM_NET_BALL:
        if (DoesMonHaveType(target, TYPE_WATER) || DoesMonHaveType(target, TYPE_BUG)) {
            return FX32_CONST(3);
        }
        break;
    case ITEM_DIVE_BALL:
        if (BtlSetup_IsBattleType(flow->mainModule, 1)) {
            return FX32_CONST(3.5);
        }
        if (GetFieldEffectData(flow->mainModule)->env.terrain == 6) {
            return FX32_CONST(3.5);
        }
        break;
    case ITEM_NEST_BALL: {
        u16 level = GetBattleMonStat(target, 0xf);
        if (level < 30) {
            u16 bonus = 41 - level;
            if (bonus > 40) {
                bonus = 40;
            }
            return FX32_CONST(bonus) / 10;
        }
        break;
    }
    case ITEM_REPEAT_BALL:
        if (func_ov093_021b8724(flow->mainModule, target)) {
            return FX32_CONST(3);
        }
        break;
    case ITEM_TIMER_BALL: {
        fx32 ratio = flow->unk10 * FX32_CONST(0.3) + FX32_ONE;
        if (ratio > FX32_CONST(4)) {
            ratio = FX32_CONST(4);
        }
        return ratio;
    }
    case ITEM_DUSK_BALL: {
        const BtlFieldSituation *field = GetFieldEffectData(flow->mainModule);
        u8 period = GetDayPeriod(field->env.season, field->env.hour);
        if (field->env.bgType == 4 || field->env.bgType == 5) {
            return FX32_CONST(3.5);
        }
        if (data_ov095_0689c9a1(field->env.bgType) && (period == 3 || period == 4)) {
            return FX32_CONST(3.5);
        }
        break;
    }
    case ITEM_QUICK_BALL:
        if (flow->unk10 == 0) {
            return FX32_CONST(5);
        }
        break;
    }
    return FX32_ONE;
}


// A critical capture, likelier the more species the player has caught
static BOOL func_ov093_021cbea8(BtlServerFlow *flow, fx32 value) {
    u32 count = func_ov093_021b873c(flow->mainModule);
    fx32 ratio;

    if (count > 600) {
        ratio = FX32_CONST(2.5);
    } else if (count > 450) {
        ratio = FX32_CONST(2);
    } else if (count > 300) {
        ratio = FX32_CONST(1.5);
    } else if (count > 150) {
        ratio = FX32_CONST(1);
    } else if (count > 30) {
        ratio = FX32_CONST(0.5);
    } else {
        return FALSE;
    }
    if (value > FX32_CONST(255)) {
        value = FX32_CONST(255);
    }
    value = FX_MUL(value, ratio) / 6;
    if (BattleRandom(256) < (value >> FX32_SHIFT)) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov093_021cbf30(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_SLEEP);
}

static BOOL func_ov093_021cbf40(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_POISON);
}

static BOOL func_ov093_021cbf50(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_BURN);
}

static BOOL func_ov093_021cbf60(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_FREEZE);
}

static BOOL func_ov093_021cbf70(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_PARALYSIS);
}

static BOOL func_ov093_021cbf80(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc350(flow, mon, item, value, CONDITION_CONFUSION);
}

static BOOL func_ov093_021cbf90(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc398(flow, mon, item, value, 7);
}

// Guard Spec. sets up Mist on the user's side
static BOOL func_ov093_021cbfa0(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u8 side;
    BattleCondition cont;
    BattleHandlerMessageParam *message;

    side = GetSideFromMonID(GetMonID(mon));
    cont = SetConditionTurns(5);
    if (data_ov095_06898c11(side, 3, cont)) {
        message = BattleHandler_PushWork(flow, 4, 0x1f);
        BattleHandler_StrSetup(&message->string, 1, 0x88);
        BattleHandler_AddArg(&message->string, side);
        BattleHandler_PopWork(flow, message);
        return TRUE;
    }
    return FALSE;
}


// Revives a fainted mon with the HP the item gives
static BOOL func_ov093_021cc008(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u8 monId;
    BattleHandlerReviveParam *revive;

    if (IsFainted(mon)) {
        monId = GetMonID(mon);
        revive = BattleHandler_PushWork(flow, 0x2c, monId);
        revive->monIndex = monId;
        switch (ItemGetParam(item, 0x3a)) {
        case 0xff:
            revive->amount = GetBattleMonStat(mon, 0xe);
            break;
        case 0xfe:
            revive->amount = DivideMaxHPZeroCheck(mon, 2);
            break;
        case 0xfd:
            revive->amount = DivideMaxHPZeroCheck(mon, 4);
            break;
        default:
            revive->amount = ItemGetParam(item, 0x3a);
            break;
        }
        BattleHandler_StrSetup(&revive->string, 2, 3);
        BattleHandler_AddArg(&revive->string, monId);
        BattleHandler_PopWork(flow, revive);
        func_ov093_021c84e0(flow, monId);
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov093_021cc098(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 1);
}

static BOOL func_ov093_021cc0a8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 2);
}

static BOOL func_ov093_021cc0b8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 3);
}

static BOOL func_ov093_021cc0c8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 4);
}

static BOOL func_ov093_021cc0d8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 5);
}

static BOOL func_ov093_021cc0e8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    return func_ov093_021cc438(flow, mon, item, value, 6);
}

// Dire Hit pushes the critical-hit stage up
static BOOL func_ov093_021cc0f8(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    BOOL used = FALSE;

    if (!GetAdditionalConditionFlag(mon, 9)) {
        func_ov093_021d5fe4(mon, 9);
        func_ov093_021cd4b0(flow->queue, 0x19, GetMonID(mon), 9);
        used = TRUE;
    }
    if (value > 1 && func_ov093_021d5f38(mon, value - 1)) {
        func_ov093_021cd4b0(flow->queue, 0xe, GetMonID(mon), value - 1);
        used = TRUE;
    }
    if (used) {
        func_ov093_021cd64c(flow->queue, 0x5b, 0x411, GetMonID(mon), 0xffff0000);
        return TRUE;
    }
    return FALSE;
}

// An Ether restores the PP of one move
static BOOL func_ov093_021cc178(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 slot) {
    u8 monId;
    u8 amount;
    u8 maxAmount;
    BattleHandlerPPParam *pp;

    if (func_ov093_021d5450(mon) > slot) {
        monId = GetMonID(mon);
        amount = func_ov093_021d5568(mon, slot);
        maxAmount = ItemGetParam(item, 0x3b);
        if (maxAmount != 0x7f && amount > maxAmount) {
            amount = maxAmount;
        }
        if (amount != 0) {
            pp = BattleHandler_PushWork(flow, 9, monId);
            pp->amount = amount;
            pp->monIndex = monId;
            pp->moveIndex = slot;
            pp->allowFainted = 1;
            BattleHandler_StrSetup(&pp->string, 2, 0x186);
            BattleHandler_AddArg(&pp->string, monId);
            BattleHandler_AddArg(&pp->string, func_ov093_021d54d0(mon, slot));
            BattleHandler_PopWork(flow, pp);
            return TRUE;
        }
    }
    return FALSE;
}

// An Elixir restores the PP of every move
static BOOL func_ov093_021cc214(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u8 monId;
    u32 numMoves;
    BOOL used;
    u8 maxAmount;
    u8 amount;
    u32 i;
    BattleHandlerPPParam *pp;
    BattleHandlerMessageParam *message;

    monId = GetMonID(mon);
    maxAmount = ItemGetParam(item, 0x3b);
    used = FALSE;
    numMoves = func_ov093_021d5450(mon);
    for (i = 0; i < numMoves; i++) {
        amount = func_ov093_021d5568(mon, i);
        if (amount > maxAmount) {
            amount = maxAmount;
        }
        if (amount != 0) {
            pp = BattleHandler_PushWork(flow, 9, monId);
            pp->moveIndex = i;
            pp->monIndex = monId;
            pp->amount = amount;
            pp->allowFainted = 1;
            BattleHandler_PopWork(flow, pp);
            used = TRUE;
        }
    }
    if (used) {
        message = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&message->string, 2, 0x189);
        BattleHandler_AddArg(&message->string, monId);
        BattleHandler_PopWork(flow, message);
    }
    return used;
}

// A Potion restores HP
static BOOL func_ov093_021cc2c0(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u8 monId;
    BattleHandlerRecoverHPParam *recover;
    u32 amount;

    if (!IsMonFullHP(mon) && !IsFainted(mon)) {
        monId = GetMonID(mon);
        recover = BattleHandler_PushWork(flow, 5, monId);
        recover->targetIndex = monId;
        recover->skipCheck = TRUE;
        amount = ItemGetParam(item, 0x3a);
        switch (amount) {
        case 0xff:
            recover->amount = GetBattleMonStat(mon, 0xe);
            break;
        case 0xfe:
            recover->amount = DivideMaxHPZeroCheck(mon, 2);
            break;
        case 0xfd:
            recover->amount = DivideMaxHPZeroCheck(mon, 4);
            break;
        default:
            recover->amount = amount;
            break;
        }
        BattleHandler_StrSetup(&recover->string, 2, 0x183);
        BattleHandler_AddArg(&recover->string, monId);
        BattleHandler_PopWork(flow, recover);
        return TRUE;
    }
    return FALSE;
}

// Cures a condition of a mon
static BOOL func_ov093_021cc350(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u32 condition) {
    u8 monId;
    BattleHandlerCureConditionParam *param;

    if (!IsFainted(mon) && CheckCondition(mon, condition)) {
        monId = GetMonID(mon);
        param = BattleHandler_PushWork(flow, 0xb, monId);
        param->count = 1;
        param->monIds[0] = monId;
        param->condition = condition;
        BattleHandler_PopWork(flow, param);
        return TRUE;
    }
    return FALSE;
}

// Cures every major condition of a mon that isn't in battle
static BOOL func_ov093_021cc398(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    BOOL cured;
    u32 i;
    u32 condition;
    u8 monId;
    BattleHandlerCureConditionParam *work;

    if (!IsFainted(mon) && !DoesBattleMonExist(&flow->posPoke, GetMonID(mon))) {
        cured = FALSE;
        i = 0;
        while (TRUE) {
            condition = data_ov095_0689c9e5(i++);
            if (condition == CONDITION_NONE) {
                break;
            }
            if (CheckCondition(mon, condition)) {
                monId = GetMonID(mon);
                work = BattleHandler_PushWork(flow, 0xb, monId);
                work->count = 1;
                work->monIds[0] = monId;
                work->condition = condition;
                BattleHandler_PopWork(flow, work);
                cured = TRUE;
            }
        }
        return cured;
    }
    return FALSE;
}


// Raises a stat of a mon in battle
static BOOL func_ov093_021cc438(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u32 stat) {
    u8 monId;
    BattleHandlerStatChangeParam *param;

    monId = GetMonID(mon);
    if (GetBattlePos(&flow->posPoke, monId) != 6 && !IsFainted(mon) && IsStatChangeValid(mon, stat, value)) {
        param = BattleHandler_PushWork(flow, 0xe, monId);
        param->count = 1;
        param->monIds[0] = monId;
        param->stat = stat;
        param->change = value;
        param->unk0e = 1;
        BattleHandler_PopWork(flow, param);
        return TRUE;
    }
    return FALSE;
}


// Uses a mon's held item: 1 when it was used, 2 when it only set the flag, 0 when it can't be used
static BOOL func_ov093_021cc4ac(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u8 flag;
    u8 monId;
    u8 result;

    if (!data_ov095_0689c8fd(GetBattleMonHeldItem(mon))) {
        monId = GetMonID(mon);
        result = func_ov093_021c84ac(flow, mon, &flag);
        if (!result) {
            result = flag ? 2 : 0;
            return result;
        }
        return 1;
    }
    return 0;
}


BOOL func_ov093_021cc508(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u32 state;
    u32 result;

    if (GetBattleMonStat(mon, 0x10) != ABILITY_IMPOSTER) {
        state = PushState(&flow->actionState, 0x781);
        func_ov093_021c84d0(flow, mon);
        result = func_ov093_021c8910(flow);
        PopState(&flow->actionState, state, 0x786);
        if (result == 2) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov093_021cc558(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u16 heldItem;
    u32 state;
    u8 monId;
    BattleHandlerSetItemParam *setItem;

    if (!func_ov093_021e869c(flow, GetMonID(mon)) && (heldItem = GetBattleMonHeldItem(mon)) != 0) {
        state = PushState(&flow->actionState, 0x798);
        monId = GetMonID(mon);
        setItem = BattleHandler_PushWork(flow, 0x20, monId);
        setItem->targetIndex = monId;
        setItem->item = 0;
        BattleHandler_StrSetup(&setItem->string, 2, 0xe1);
        BattleHandler_AddArg(&setItem->string, monId);
        BattleHandler_AddArg(&setItem->string, heldItem);
        BattleHandler_PopWork(flow, setItem);
        PopState(&flow->actionState, state, 0x7a5);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov093_021cc5f4(BtlServerFlow *flow, BattleMon *mon, u16 item, s32 value, u8 param) {
    u32 state;
    u8 monId;
    BattleHandlerResetStatStageParam *reset;
    BattleHandlerMessageParam *message;

    state = PushState(&flow->actionState, 0x7b0);
    monId = GetMonID(mon);
    reset = BattleHandler_PushWork(flow, 0x10, monId);
    reset->count = 1;
    reset->monIndices[0] = monId;
    BattleHandler_PopWork(flow, reset);
    message = BattleHandler_PushWork(flow, 4, monId);
    BattleHandler_StrSetup(&message->string, 2, 0xe4);
    BattleHandler_AddArg(&message->string, monId);
    BattleHandler_PopWork(flow, message);
    PopState(&flow->actionState, state, 0x7c0);
    return TRUE;
}

u32 func_ov093_021cc668(BtlServerFlow *flow) {
    u8 alive[2];
    u8 total[2];
    u32 i;
    BattleParty *party;
    u8 side;
    u8 otherSide;
    u8 monId;

    alive[0] = alive[1] = 0;
    total[0] = total[1] = 0;
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(flow->mainModule, i)) {
            party = GetPartyData(flow->pokeCon, i);
            if (GetNumMonsInParty(party)) {
                side = GetClientSide(flow->mainModule, i);
                alive[side] += GetAlivePartyCount(party);
                total[side] += GetNumMonsInParty(party);
            }
        }
    }
    side = GetClientSide(flow->mainModule, GetPlayerClientID(flow->mainModule));
    otherSide = side ^ 1;
    if (alive[side] == 0 && alive[otherSide] != 0) {
        return 0;
    }
    if (alive[otherSide] == 0 && alive[side] != 0) {
        return 1;
    }
    if (!func_ov093_021ba534(flow->mainModule)) {
        return 2;
    }
    if (alive[0] == 0) {
        monId = data_ov095_0689d1d5(&flow->faintRec);
        if (monId != 0x1f) {
            u8 monSide = GetSideFromMonID(monId);

            if (monSide == side) {
                return 1;
            }
            return 0;
        }
        return 2;
    }
    return func_ov093_021cc768(flow, alive, total, side);
}


#pragma thumb reset


#pragma thumb reset


// When both sides are still standing: the side with more mons left wins, then the one with more HP left
static u32 func_ov093_021cc768(BtlServerFlow *flow, const u8 *alive, const u8 *total, u8 side) {
    s32 down0;
    s32 down1;
    s32 hp[2];
    s32 percent[2];
    u8 winner;

    down0 = total[0] - alive[0];
    down1 = total[1] - alive[1];
    if (down0 != down1) {
        winner = down0 >= down1 ? 1 : 0;
        if (winner == side) {
            return 1;
        }
        return 0;
    }
    func_ov093_021cc800(flow, &hp[0], &percent[0], 0);
    func_ov093_021cc800(flow, &hp[1], &percent[1], 1);
    if (percent[0] != percent[1]) {
        winner = percent[0] > percent[1] ? 0 : 1;
        if (winner == side) {
            return 1;
        }
        return 0;
    }
    if (hp[0] != hp[1]) {
        winner = hp[0] > hp[1] ? 0 : 1;
        if (winner == side) {
            return 1;
        }
        return 0;
    }
    return 2;
}

// The HP a side has left, in total and in percent of its max
static void func_ov093_021cc800(BtlServerFlow *flow, s32 *hp, s32 *percent, u8 side) {
    u32 maxHP;
    u32 i;
    BattleParty *party;
    u8 numMons;
    u32 j;
    BattleMon *mon;

    maxHP = 0;
    *hp = 0;
    *percent = 0;
    for (i = 0; i < 4; i++) {
        if (DoesClientExist(flow->mainModule, i) && side == GetClientSide(flow->mainModule, i)) {
            party = GetPartyData(flow->pokeCon, i);
            numMons = GetNumMonsInParty(party);
            for (j = 0; j < numMons; j++) {
                mon = GetBattleMonFromParty(party, j);
                maxHP += GetBattleMonStat(mon, 0xe);
                *hp += GetBattleMonStat(mon, 0xd);
            }
        }
    }
    *percent = *hp * 100 / maxHP;
}

void func_ov093_021cc8a0(u32 *table) {
    u32 i;

    for (i = 0; i < 24; i++) {
        table[i] = 6;
    }
}

void func_ov093_021cc8b0(u32 *table, u8 monId, u32 value) {
    table[monId] = value;
}

u32 func_ov093_021cc8b8(u32 *table, u8 monId) {
    return table[monId];
}

u32 func_ov093_021cc8c0(u32 *table, u8 monId) {
    return table[monId];
}
