// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/side_handlers.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/side_handlers.h"
#include "types.h"
#include "battle/btl_calc.h"
#include "battle/btl_event.h"
#include "battle/btl_handler.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "constants/battle.h"
#include "constants/types.h"
#include "gfl/std.h"
#include "pml/waza.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BOOL func_ov167_021abc8c(BtlServerFlow *flow, u8 monId);
u32 func_ov167_021abc9c(BtlServerFlow *flow);
BOOL func_ov167_021abd74(BtlServerFlow *flow, u8 monId);
BOOL func_ov167_021abf14(BtlServerFlow *flow);

#define NUM_SIDES 2
#define NUM_SIDE_EFFECTS 14

// One entry of the side effect table: an effect, the function that returns its handlers, and how many layers it takes
// Black 2's, not checked against Black
typedef struct SideEffectEventAddEntry {
    u32 effect;
    const BattleEventHandlerEntry *(*eventAdd)(u32 *numHandlers);
    u32 maxLayers;
} SideEffectEventAddEntry;

// A side effect that is set on a side
typedef struct SideEffectSlot {
    BattleEventItem *item;
    BattleCondition cont;
    u32 turnsElapsed;
    u32 layers;
} SideEffectSlot;

// pokebw2's sSideEffects
static SideEffectSlot data_ov095_0689e7e0[NUM_SIDES][NUM_SIDE_EFFECTS];

static u8 func_ov095_06898de0(BattleEventItem *item, u32 side);
static const BattleEventHandlerEntry *EventAddSideReflect(u32 *numHandlers);
static void HandlerSideReflect(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideLightScreen(u32 *numHandlers);
static void HandlerSideLightScreen(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static void CommonScreenEffect(BtlServerFlow *flow, u8 side, u32 screen);
static const BattleEventHandlerEntry *EventAddSideSafeguard(u32 *numHandlers);
static void HandlerSideSafeguardCheckFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static void HandlerSideSafeguardFixFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideMist(u32 *numHandlers);
static void HandlerSideMistCheckFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static void HandlerSideMistFixFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideTailwind(u32 *numHandlers);
static void HandlerSideTailwind(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideLuckyChant(u32 *numHandlers);
static void HandlerSideLuckyChant(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideStealthRock(u32 *numHandlers);
static void HandlerSideStealthRock(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideWideGuard(u32 *numHandlers);
static void HandlerSideWideGuard(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideQuickGuard(u32 *numHandlers);
static void HandlerSideQuickGuard(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideSpikes(u32 *numHandlers);
static void HandlerSideSpikes(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideToxicSpikes(u32 *numHandlers);
static void HandlerSideToxicSpikes(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideRainbow(u32 *numHandlers);
static void HandlerSideRainbow(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static void HandlerSideRainbowShrink(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideSeaOfFire(u32 *numHandlers);
static void HandlerSideSeaOfFire(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);
static const BattleEventHandlerEntry *EventAddSideSwamp(u32 *numHandlers);
static void HandlerSideSwamp(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work);

// Each side effect and the function that returns its handlers
// pokebw2's sSideEffectEventAddTable
static const SideEffectEventAddEntry data_ov095_0689d780[NUM_SIDE_EFFECTS] = {
    { SIDE_CONDITION_REFLECT, EventAddSideReflect, 1 },
    { SIDE_CONDITION_LIGHT_SCREEN, EventAddSideLightScreen, 1 },
    { SIDE_CONDITION_SAFEGUARD, EventAddSideSafeguard, 1 },
    { SIDE_CONDITION_MIST, EventAddSideMist, 1 },
    { SIDE_CONDITION_TAILWIND, EventAddSideTailwind, 1 },
    { SIDE_CONDITION_LUCKY_CHANT, EventAddSideLuckyChant, 1 },
    { SIDE_CONDITION_SPIKES, EventAddSideSpikes, 3 },
    { SIDE_CONDITION_TOXIC_SPIKES, EventAddSideToxicSpikes, 2 },
    { SIDE_CONDITION_STEALTH_ROCK, EventAddSideStealthRock, 1 },
    { SIDE_CONDITION_WIDE_GUARD, EventAddSideWideGuard, 1 },
    { SIDE_CONDITION_QUICK_GUARD, EventAddSideQuickGuard, 1 },
    { SIDE_CONDITION_RAINBOW, EventAddSideRainbow, 1 },
    { SIDE_CONDITION_SEA_OF_FIRE, EventAddSideSeaOfFire, 1 },
    { SIDE_CONDITION_SWAMP, EventAddSideSwamp, 1 },
};

// pokebw2's SideEffectsInit
void data_ov095_06898bfd(void) {
    MI_CpuFill8(data_ov095_0689e7e0, 0, sizeof(data_ov095_0689e7e0));
}

// pokebw2's SideEffectEventAdd
BattleEventItem *data_ov095_06898c11(u8 side, u32 effect, BattleCondition cont) {
    u32 i;
    u32 numHandlers;
    const BattleEventHandlerEntry *handlers;
    BattleEventItem *item;
    SideEffectSlot *slot = &data_ov095_0689e7e0[side][effect];

    for (i = 0; i < NUM_SIDE_EFFECTS; i++) {
        if (effect == data_ov095_0689d780[i].effect) {
            if (slot->layers == 0) {
                handlers = data_ov095_0689d780[i].eventAdd(&numHandlers);
                item = BattleEvent_AddItem(2, effect, 2, 0, side, handlers, numHandlers);
                BattleEventItem_SetWorkValue(item, 6, cont.raw);
                slot->layers = 1;
                slot->turnsElapsed = 0;
                slot->cont = cont;
                slot->item = item;
                return item;
            }
            if (slot->layers < data_ov095_0689d780[i].maxLayers) {
                slot->layers++;
                return slot->item;
            }
        }
    }
    return NULL;
}

// pokebw2's GetSideEffectLayers
u32 data_ov095_06898ce1(u8 side, u32 effect) {
    return data_ov095_0689e7e0[side][effect].layers;
}

// pokebw2's IsSideEffectSet
BOOL data_ov095_06898cf5(u8 side, u32 effect) {
    if (data_ov095_0689e7e0[side][effect].item != NULL) {
        return TRUE;
    }
    return FALSE;
}

BOOL ServerDisplay_RemoveSideEffect(u8 side, u32 effect) {
    SideEffectSlot *slot = &data_ov095_0689e7e0[side][effect];

    if (slot->item != NULL) {
        BattleEventItem_Remove(slot->item);
        slot->item = NULL;
        slot->layers = 0;
        slot->cont.common.type = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's SideEffectsEndTurn
void data_ov095_06898d55(void (*callback)(u8 side, u32 effect, BtlServerFlow *flow), BtlServerFlow *flow) {
    u32 side;
    u32 effect;
    SideEffectSlot *slot;

    for (side = 0; side < NUM_SIDES; side++) {
        for (effect = 0; effect < NUM_SIDE_EFFECTS; effect++) {
            slot = &data_ov095_0689e7e0[side][effect];
            if (slot->layers != 0 && slot->cont.common.type == 2) {
                slot->turnsElapsed++;
                if (slot->turnsElapsed >= slot->cont.common.turns) {
                    slot->layers = 0;
                    slot->cont.common.type = 0;
                    BattleEventItem_Remove(slot->item);
                    slot->item = NULL;
                    callback(side, effect, flow);
                }
            }
        }
    }
}

// How many layers the side effect of the item has on the side
// pokebw2's GetSideEffectLayersOfItem
static u8 func_ov095_06898de0(BattleEventItem *item, u32 side) {
    s32 effect = BattleEventItem_GetSubID(item);

    if (effect < NUM_SIDE_EFFECTS && side < NUM_SIDES) {
        return data_ov095_0689e7e0[side][effect].layers;
    }
    return 0;
}

// pokebw2's sHandlersSideReflect
static const BattleEventHandlerEntry data_ov095_0689d6f0[] = {
    { 0x47, HandlerSideReflect },
};

static const BattleEventHandlerEntry *EventAddSideReflect(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d6f0);
    return data_ov095_0689d6f0;
}

static void HandlerSideReflect(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    CommonScreenEffect(flow, side, 1);
}

// pokebw2's sHandlersSideLightScreen
static const BattleEventHandlerEntry data_ov095_0689d710[] = {
    { 0x47, HandlerSideLightScreen },
};

static const BattleEventHandlerEntry *EventAddSideLightScreen(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d710);
    return data_ov095_0689d710;
}

static void HandlerSideLightScreen(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    CommonScreenEffect(flow, side, 2);
}

// Reflect and Light Screen cut the damage of the screen's category of moves, by a third in a double battle
static void CommonScreenEffect(BtlServerFlow *flow, u8 side, u32 screen) {
    u32 battleType;
    u32 multiplier;

    if (side == GetSideFromMonID(BattleEventVar_GetValue(4)) && screen == BattleEventVar_GetValue(0x1a) &&
        BattleEventVar_GetValue(0x45) == 0) {
        battleType = func_ov093_021c815c(flow);
        if (battleType == 0 || battleType == 3) {
            multiplier = 0x800;
        } else {
            multiplier = 0xa8f;
        }
        BattleEventVar_MulValue(0x35, multiplier);
    }
}

// pokebw2's sHandlersSideSafeguard
static const BattleEventHandlerEntry data_ov095_0689d748[] = {
    { 0x65, HandlerSideSafeguardCheckFail },
    { 0x67, HandlerSideSafeguardFixFail },
};

static const BattleEventHandlerEntry *EventAddSideSafeguard(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d748);
    return data_ov095_0689d748;
}

static void HandlerSideSafeguardCheckFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 attackerId;
    u32 condition;

    attackerId = BattleEventVar_GetValue(4);
    if (side == GetSideFromMonID(attackerId) && attackerId != BattleEventVar_GetValue(3)) {
        condition = BattleEventVar_GetValue(0x1d);
        if (IsBasicStatus(condition) || condition == 6 || condition == 0xe) {
            work[0] = BattleEventVar_RewriteValue(0x41, 1);
        }
    }
}

static void HandlerSideSafeguardFixFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    BattleHandlerMessageParam *message;
    u8 attackerId;

    if (work[0] != 0) {
        attackerId = BattleEventVar_GetValue(4);
        message = BattleHandler_PushWork(flow, 4, attackerId);
        BattleHandler_StrSetup(&message->string, 2, 0x347);
        BattleHandler_AddArg(&message->string, attackerId);
        BattleHandler_PopWork(flow, message);
        work[0] = 0;
    }
}

// pokebw2's sHandlersSideMist
static const BattleEventHandlerEntry data_ov095_0689d758[] = {
    { 0x5b, HandlerSideMistCheckFail },
    { 0x5c, HandlerSideMistFixFail },
};

static const BattleEventHandlerEntry *EventAddSideMist(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d758);
    return data_ov095_0689d758;
}

static void HandlerSideMistCheckFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 targetId;

    targetId = BattleEventVar_GetValue(2);
    if (side == GetSideFromMonID(targetId) && (s32)BattleEventVar_GetValue(0x20) < 0 &&
        targetId != BattleEventVar_GetValue(3)) {
        work[0] = BattleEventVar_RewriteValue(0x41, 1);
    }
}

static void HandlerSideMistFixFail(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    BattleHandlerMessageParam *message;
    int move;
    u8 targetId;

    if (work[0] != 0) {
        move = BattleEventVar_GetValue(0x19);
        if (move == 0 || work[1] != move) {
            targetId = BattleEventVar_GetValue(2);
            message = BattleHandler_PushWork(flow, 4, targetId);
            BattleHandler_StrSetup(&message->string, 2, 0x34a);
            BattleHandler_AddArg(&message->string, targetId);
            BattleHandler_PopWork(flow, message);
            work[1] = move;
        }
        work[0] = 0;
    }
}

// pokebw2's sHandlersSideTailwind
static const BattleEventHandlerEntry data_ov095_0689d738[] = {
    { 0x13, HandlerSideTailwind },
};

static const BattleEventHandlerEntry *EventAddSideTailwind(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d738);
    return data_ov095_0689d738;
}

static void HandlerSideTailwind(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    if (side == GetSideFromMonID(BattleEventVar_GetValue(2))) {
        BattleEventVar_MulValue(0x35, 0x2000);
    }
}

// pokebw2's sHandlersSideLuckyChant
static const BattleEventHandlerEntry data_ov095_0689d730[] = {
    { 0x36, HandlerSideLuckyChant },
};

static const BattleEventHandlerEntry *EventAddSideLuckyChant(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d730);
    return data_ov095_0689d730;
}

static void HandlerSideLuckyChant(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    if (side == GetSideFromMonID(BattleEventVar_GetValue(4))) {
        BattleEventVar_RewriteValue(0x41, 1);
    }
}

// pokebw2's sHandlersSideStealthRock
static const BattleEventHandlerEntry data_ov095_0689d728[] = {
    { 0x55, HandlerSideStealthRock },
};

static const BattleEventHandlerEntry *EventAddSideStealthRock(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d728);
    return data_ov095_0689d728;
}

static void HandlerSideStealthRock(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 monId;
    BattleMon *mon;
    u32 divisor;
    BattleHandlerDamageParam *damage;

    divisor = 2;
    monId = BattleEventVar_GetValue(2);
    if (side == GetSideFromMonID(monId)) {
        mon = GetBattleMon(flow, monId);
        switch (func_ov093_021d799c(TYPE_ROCK, GetPokeType(mon))) {
        case TYPE_EFFECTIVENESS_IMMUNE:
        case TYPE_EFFECTIVENESS_QUARTER:
        default:
            divisor = 32;
            break;
        case TYPE_EFFECTIVENESS_HALF:
            divisor = 16;
            break;
        case TYPE_EFFECTIVENESS_NORMAL:
            divisor = 8;
            break;
        case TYPE_EFFECTIVENESS_DOUBLE:
            divisor = 4;
            break;
        case TYPE_EFFECTIVENESS_QUADRUPLE:
            break;
        }
        damage = BattleHandler_PushWork(flow, 7, 0x1f);
        damage->targetIndex = monId;
        damage->amount = DivideMaxHPZeroCheck(mon, divisor);
        BattleHandler_StrSetup(&damage->string, 2, 0x356);
        BattleHandler_AddArg(&damage->string, monId);
        BattleHandler_PopWork(flow, damage);
    }
}

// pokebw2's sHandlersSideWideGuard
static const BattleEventHandlerEntry data_ov095_0689d700[] = {
    { 0x2c, HandlerSideWideGuard },
};

static const BattleEventHandlerEntry *EventAddSideWideGuard(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d700);
    return data_ov095_0689d700;
}

static void HandlerSideWideGuard(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 targetId;
    u16 move;
    u32 target;
    BattleHandlerString *string;

    targetId = BattleEventVar_GetValue(4);
    if (side == GetSideFromMonID(targetId)) {
        move = BattleEventVar_GetValue(0x12);
        target = PML_MoveGetParam(move, MOVE_PARAM_TARGET);
        if (PML_MoveIsDamaging(move) && (target == MOVE_TARGET_ALL_ADJACENT || target == MOVE_TARGET_ADJACENT_FOES) &&
            BattleEventVar_RewriteValue(0x40, 1)) {
            string = (BattleHandlerString *)BattleEventVar_GetValue(0x3f);
            BattleHandler_StrSetup(string, 2, 0x31d);
            BattleHandler_AddArg(string, targetId);
        }
    }
}

// pokebw2's sHandlersSideQuickGuard
static const BattleEventHandlerEntry data_ov095_0689d718[] = {
    { 0x2c, HandlerSideQuickGuard },
};

static const BattleEventHandlerEntry *EventAddSideQuickGuard(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d718);
    return data_ov095_0689d718;
}

static void HandlerSideQuickGuard(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 attackerId;
    u16 move;
    BattleHandlerString *string;

    attackerId = BattleEventVar_GetValue(4);
    if (side == GetSideFromMonID(attackerId) && attackerId != BattleEventVar_GetValue(3)) {
        move = BattleEventVar_GetValue(0x12);
        if (PML_MoveGetParam(move, MOVE_PARAM_PRIORITY) >= 1 && BattleEventVar_RewriteValue(0x40, 1)) {
            string = (BattleHandlerString *)BattleEventVar_GetValue(0x3f);
            BattleHandler_StrSetup(string, 2, 0x320);
            BattleHandler_AddArg(string, attackerId);
        }
    }
}

// pokebw2's sHandlersSideSpikes
static const BattleEventHandlerEntry data_ov095_0689d740[] = {
    { 0x55, HandlerSideSpikes },
};

static const BattleEventHandlerEntry *EventAddSideSpikes(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d740);
    return data_ov095_0689d740;
}

static void HandlerSideSpikes(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 monId;
    BattleMon *mon;
    u32 divisor;
    BattleHandlerDamageParam *damage;

    monId = BattleEventVar_GetValue(2);
    if (side == GetSideFromMonID(monId) && !func_ov093_021c8234(flow, monId)) {
        switch (func_ov095_06898de0(item, side)) {
        case 1:
        default:
            divisor = 8;
            break;
        case 2:
            divisor = 6;
            break;
        case 3:
            divisor = 4;
            break;
        }
        damage = BattleHandler_PushWork(flow, 7, 0x1f);
        mon = GetBattleMon(flow, monId);
        damage->targetIndex = monId;
        damage->amount = DivideMaxHPZeroCheck(mon, divisor);
        BattleHandler_StrSetup(&damage->string, 2, 0x353);
        BattleHandler_AddArg(&damage->string, monId);
        BattleHandler_PopWork(flow, damage);
    }
}

// pokebw2's sHandlersSideToxicSpikes
static const BattleEventHandlerEntry data_ov095_0689d708[] = {
    { 0x55, HandlerSideToxicSpikes },
};

static const BattleEventHandlerEntry *EventAddSideToxicSpikes(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d708);
    return data_ov095_0689d708;
}

// A grounded Pokémon of the side poisons itself on the spikes, and a Poison type absorbs them
static void HandlerSideToxicSpikes(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 monId;
    BattleHandlerRemoveSideEffectParam *remove;
    BattleHandlerAddConditionParam *condition;

    monId = BattleEventVar_GetValue(2);
    if (side == GetSideFromMonID(monId) && !func_ov093_021c8234(flow, monId)) {
        if (DoesMonHaveType(GetBattleMon(flow, monId), TYPE_POISON)) {
            remove = BattleHandler_PushWork(flow, 0x1a, monId);
            remove->side = side;
            BattleHandler_InitFlags(remove->effects, sizeof(remove->effects));
            BattleHandler_SetFlag(remove->effects, SIDE_CONDITION_TOXIC_SPIKES);
            BattleHandler_PopWork(flow, remove);
        } else {
            condition = BattleHandler_PushWork(flow, 0xc, 0x1f);
            condition->condition = 5;
            if (func_ov095_06898de0(item, side) > 1) {
                condition->value = func_ov093_021e8a58();
            } else {
                condition->value = MakeConditionPermanent();
            }
            condition->targetIndex = monId;
            BattleHandler_PopWork(flow, condition);
        }
    }
}

// pokebw2's sHandlersSideRainbow
static const BattleEventHandlerEntry data_ov095_0689d768[] = {
    { 0x64, HandlerSideRainbow },
    { 0x51, HandlerSideRainbow },
    { 0x6c, HandlerSideRainbowShrink },
};

static const BattleEventHandlerEntry *EventAddSideRainbow(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d768);
    return data_ov095_0689d768;
}

// Doubles the chance of a move's added effect
static void HandlerSideRainbow(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u16 chance;

    if (side == GetSideFromMonID(BattleEventVar_GetValue(3))) {
        chance = BattleEventVar_GetValue(0x26) * 2;
        BattleEventVar_RewriteValue(0x26, chance);
    }
}

static void HandlerSideRainbowShrink(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    if (side == GetSideFromMonID(BattleEventVar_GetValue(3))) {
        BattleEventVar_RewriteValue(0x45, 1);
    }
}

// pokebw2's sHandlersSideSeaOfFire
static const BattleEventHandlerEntry data_ov095_0689d6f8[] = {
    { 0x76, HandlerSideSeaOfFire },
};

static const BattleEventHandlerEntry *EventAddSideSeaOfFire(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d6f8);
    return data_ov095_0689d6f8;
}

static void HandlerSideSeaOfFire(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    u8 monId;
    BattleMon *mon;
    BattleHandlerEffectAtPosParam *effect;
    BattleHandlerDamageParam *damage;

    monId = BattleEventVar_GetValue(2);
    if (monId != 0x1f && func_ov093_021c814c(flow, monId) && !func_ov093_021c83d4(flow) &&
        side == GetSideFromMonID(monId)) {
        mon = GetBattleMon(flow, monId);
        if (!DoesMonHaveType(mon, TYPE_FIRE)) {
            effect = BattleHandler_PushWork(flow, 0x37, monId);
            effect->pos1 = func_ov093_021c8010(flow, monId);
            effect->pos2 = 6;
            effect->effect = 0x28e;
            BattleHandler_PopWork(flow, effect);

            damage = BattleHandler_PushWork(flow, 7, 0x1f);
            damage->targetIndex = monId;
            damage->amount = DivideMaxHPZeroCheck(mon, 8);
            BattleHandler_StrSetup(&damage->string, 2, 0x484);
            BattleHandler_AddArg(&damage->string, monId);
            BattleHandler_PopWork(flow, damage);
        }
    }
}

// pokebw2's sHandlersSideSwamp
static const BattleEventHandlerEntry data_ov095_0689d720[] = {
    { 0x13, HandlerSideSwamp },
};

static const BattleEventHandlerEntry *EventAddSideSwamp(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d720);
    return data_ov095_0689d720;
}

static void HandlerSideSwamp(BattleEventItem *item, BtlServerFlow *flow, u8 side, s32 *work) {
    if (side == GetSideFromMonID(BattleEventVar_GetValue(2))) {
        BattleEventVar_MulValue(0x35, 0x400);
    }
}
