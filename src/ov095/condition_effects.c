// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/condition_effects.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/condition_effects.h"
#include "types.h"
#include "battle/btl_calc.h"
#include "battle/btl_event.h"
#include "battle/btl_handler.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/types.h"

// The message of a condition that ended: the one by default and the one with a second form, as negative when the
// condition has none
// Black 2's, not checked against Black
typedef struct ConditionCureMessage {
    u32 condition;
    s16 message;
    s16 messageAlt;
} ConditionCureMessage;

static void func_ov095_0689b7fc(BtlServerFlow *flow, BattleMon *mon, u8 monId, s32 condition);
static void func_ov095_0689b974(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689b9f4(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689bb7c(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689bc30(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689bde0(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689beac(BtlServerFlow *flow, BattleMon *mon, u8 monId);
static void func_ov095_0689bf3c(BtlServerFlow *flow, BattleMon *mon, u8 monId, u32 condition, BattleCondition cont);
static void func_ov095_0689bff8(BtlServerFlow *flow, BattleMon *mon);
static void func_ov095_0689c0a8(BtlServerFlow *flow, BattleMon *mon);
static void func_ov095_0689c104(BtlServerFlow *flow, BattleMon *mon, u8 turns);
static void func_ov095_0689c1a4(BtlServerFlow *flow, BattleMon *mon);
static void func_ov095_0689c214(BtlServerFlow *flow, BattleMon *mon);
static s32 func_ov095_0689c284(s32 condition, BattleCondition value);
s32 func_ov095_0689c424(u32 condition, BOOL alt);
static s32 func_ov095_0689b92c(u32 condition);

// pokebw2's Condition_EndTurnEffect
void data_ov095_0689b7a9(BattleMon *mon, u32 condition, BattleCondition prev, BOOL cured, BtlServerFlow *flow) {
    u8 monId = GetMonID(mon);

    if (cured) {
        func_ov095_0689bf3c(flow, mon, monId, condition, prev);
    } else {
        func_ov095_0689b7fc(flow, mon, monId, condition);
    }
}

// pokebw2's ConditionEndTurnActive
static void func_ov095_0689b7fc(BtlServerFlow *flow, BattleMon *mon, u8 monId, s32 condition) {
    u32 damage;

    if (IsFainted(mon)) {
        return;
    }
    damage = func_ov093_021d6324(mon, condition);
    if (damage != 0) {
        func_ov093_021c4a64(flow, mon, condition, damage);
    }
    switch (condition) {
    case CONDITION_PERISH_SONG:
        func_ov095_0689b974(flow, mon, monId);
        break;
    case CONDITION_LEECH_SEED:
        func_ov095_0689b9f4(flow, mon, monId);
        break;
    case CONDITION_INGRAIN:
        func_ov095_0689bb7c(flow, mon, monId);
        break;
    case CONDITION_BIND:
        func_ov095_0689bc30(flow, mon, monId);
        break;
    case 0x23:
        func_ov095_0689bde0(flow, mon, monId);
        break;
    case 0x17:
        func_ov095_0689beac(flow, mon, monId);
        break;
    }
}

// pokebw2's Condition_SetDamageMessage
BOOL data_ov095_0689b8cd(BattleHandlerString *string, BattleMon *mon, u32 condition) {
    s32 message = func_ov095_0689b92c(condition);

    if (message > 0) {
        BattleHandler_StrSetup(string, 2, message);
        BattleHandler_AddArg(string, GetMonID(mon));
    }
    return FALSE;
}

// pokebw2's GetConditionDamageMessage
static s32 func_ov095_0689b92c(u32 condition) {
    switch (condition) {
    case CONDITION_POISON:
        return 0xf3;
    case CONDITION_BURN:
        return 0x105;
    case CONDITION_NIGHTMARE:
        return 0x144;
    case CONDITION_CURSE:
        return 0x42f;
    }
    return -1;
}

// Counts down the turns of Perish Song
// pokebw2's ConditionEndTurnPerishSong
static void func_ov095_0689b974(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleCondition cont;
    s32 turns;

    if (!IsFainted(mon)) {
        cont = GetConditionContinuationParam(mon, CONDITION_PERISH_SONG);
        turns = func_ov093_021e8afc(cont) - func_ov093_021d631c(mon, CONDITION_PERISH_SONG);
        if (turns > 0) {
            func_ov095_0689c104(flow, mon, turns);
        }
    }
}

// pokebw2's ConditionEndTurnLeechSeed
static void func_ov095_0689b9f4(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleHandlerDamageParam *damage;
    BattleHandlerDrainParam *drain;
    BattleCondition cont;
    u8 pos;
    s32 sourceId;
    u16 amount;
    u16 hp;

    cont = GetConditionContinuationParam(mon, CONDITION_LEECH_SEED);
    pos = Condition_GetParam(cont);
    sourceId = func_ov093_021c7d44(flow, pos);
    if (sourceId != 0x1f) {
        amount = DivideMaxHPZeroCheck(mon, 8);
        hp = GetBattleMonStat(mon, 0xd);
        if (amount > hp) {
            amount = hp;
        }
        damage = BattleHandler_PushWork(flow, 7, monId);
        damage->targetIndex = monId;
        damage->amount = amount;
        BattleHandler_StrSetup(&damage->string, 2, 0x262);
        BattleHandler_AddArg(&damage->string, monId);
        damage->showViewEffect = TRUE;
        damage->effect = 0x275;
        damage->effectArg1 = pos;
        damage->effectArg2 = func_ov093_021c8010(flow, monId);
        BattleHandler_PopWork(flow, damage);
        if (sourceId != 0x1f) {
            drain = BattleHandler_PushWork(flow, 6, monId);
            drain->header.checkPrevResult = TRUE;
            drain->monIndex = sourceId;
            drain->sourceIndex = monId;
            drain->amount = amount;
            BattleHandler_PopWork(flow, drain);
        }
    }
}

// Recovers a sixteenth of the HP of a Pokemon with Ingrain
// pokebw2's ConditionEndTurnIngrain
static void func_ov095_0689bb7c(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleHandlerDrainParam *drain;

    if (!IsMonFullHP(mon)) {
        drain = BattleHandler_PushWork(flow, 6, monId);
        drain->monIndex = monId;
        drain->sourceIndex = 0x1f;
        drain->amount = DivideMaxHPZeroCheck(mon, 16);
        BattleHandler_StrSetup(&drain->string, 2, 0x2e3);
        BattleHandler_AddArg(&drain->string, monId);
        BattleHandler_PopWork(flow, drain);
    }
}

// Damages a Pokemon that a move like Bind holds
// pokebw2's ConditionEndTurnBind
static void func_ov095_0689bc30(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleHandlerDamageParam *damage;
    BattleCondition cont;
    u16 move;
    s32 effect;

    if (!IsFainted(mon)) {
        cont = GetConditionContinuationParam(mon, CONDITION_BIND);
        move = Condition_GetParam(cont);
        effect = -1;
        switch (move) {
        case MOVE_WRAP:
            effect = 0x277;
            break;
        case MOVE_BIND:
            effect = 0x276;
            break;
        case MOVE_FIRE_SPIN:
            effect = 0x278;
            break;
        case MOVE_MAGMA_STORM:
            effect = 0x279;
            break;
        case MOVE_CLAMP:
            effect = 0x27a;
            break;
        case MOVE_WHIRLPOOL:
            effect = 0x27b;
            break;
        case MOVE_SAND_TOMB:
            effect = 0x27c;
            break;
        }
        damage = BattleHandler_PushWork(flow, 7, monId);
        damage->targetIndex = monId;
        if (func_ov093_021e8c24(cont) != 0) {
            damage->amount = DivideMaxHPZeroCheck(mon, 8);
        } else {
            damage->amount = DivideMaxHPZeroCheck(mon, 16);
        }
        if (effect != -1) {
            damage->showViewEffect = TRUE;
            damage->effect = effect;
            damage->effectArg1 = func_ov093_021c8010(flow, monId);
            damage->effectArg2 = 6;
        }
        BattleHandler_StrSetup(&damage->string, 2, 0x174);
        BattleHandler_AddArg(&damage->string, monId);
        BattleHandler_AddArg(&damage->string, move);
        BattleHandler_PopWork(flow, damage);
    }
}

// Recovers a sixteenth of the HP of a Pokemon with Aqua Ring
// pokebw2's ConditionEndTurnAquaRing
static void func_ov095_0689bde0(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleHandlerDrainParam *drain;

    if (!IsMonFullHP(mon) && !IsFainted(mon)) {
        drain = BattleHandler_PushWork(flow, 6, monId);
        drain->monIndex = monId;
        drain->sourceIndex = 0x1f;
        drain->amount = DivideMaxHPZeroCheck(mon, 16);
        BattleHandler_StrSetup(&drain->string, 2, 0x25c);
        BattleHandler_AddArg(&drain->string, monId);
        BattleHandler_PopWork(flow, drain);
    }
}

// Cures the condition of a Pokemon whose move is gone
// pokebw2's ConditionEndTurnEncore
static void func_ov095_0689beac(BtlServerFlow *flow, BattleMon *mon, u8 monId) {
    BattleHandlerCureConditionParam *cure;
    u32 condition = CONDITION_ENCORE;

    if (!func_ov093_021d55a0(mon, Condition_GetParam(GetConditionContinuationParam(mon, condition)))) {
        cure = BattleHandler_PushWork(flow, 0xb, monId);
        cure->condition = condition;
        cure->monIds[0] = monId;
        cure->count = 1;
        BattleHandler_PopWork(flow, cure);
    }
}

// pokebw2's ConditionCured
static void func_ov095_0689bf3c(BtlServerFlow *flow, BattleMon *mon, u8 monId, u32 condition, BattleCondition cont) {
    BattleHandlerMessageParam *message;
    BattleHandlerString string;

    if (data_ov095_0689c465(condition, cont, mon, 0, &string)) {
        message = BattleHandler_PushWork(flow, 4, monId);
        message->string = string;
        BattleHandler_PopWork(flow, message);
    }
    switch (condition) {
    case CONDITION_YAWN:
        func_ov095_0689bff8(flow, mon);
        break;
    case CONDITION_PERISH_SONG:
        func_ov095_0689c0a8(flow, mon);
        break;
    case CONDITION_EMBARGO:
        func_ov095_0689c1a4(flow, mon);
        break;
    case CONDITION_HEAL_BLOCK:
        func_ov095_0689c214(flow, mon);
        break;
    }
}

// Yawn puts the Pokemon to sleep for a few turns if it has no status condition
// pokebw2's ConditionCuredYawn
static void func_ov095_0689bff8(BtlServerFlow *flow, BattleMon *mon) {
    BattleHandlerAddConditionParam *add;
    u8 monId;
    s32 turns;

    turns = func_ov093_021d7b8c(2, 4);
    if (turns > 0) {
        if (GetBattleMonStatus(mon) == 0) {
            monId = GetMonID(mon);
            add = BattleHandler_PushWork(flow, 0xc, monId);
            add->showFail = TRUE;
            add->condition = CONDITION_SLEEP;
            add->value = SetConditionTurns(turns);
            add->targetIndex = monId;
            BattleHandler_PopWork(flow, add);
        }
    }
}

// Faints the Pokemon when the Perish Song count reaches 0
// pokebw2's ConditionCuredPerishSong
static void func_ov095_0689c0a8(BtlServerFlow *flow, BattleMon *mon) {
    BattleHandlerFaintParam *faint;
    s32 monId;

    func_ov095_0689c104(flow, mon, 0);
    monId = GetMonID(mon);
    faint = BattleHandler_PushWork(flow, 0x13, monId);
    faint->monIndex = monId;
    BattleHandler_PopWork(flow, faint);
}

// pokebw2's PerishSongMessage
static void func_ov095_0689c104(BtlServerFlow *flow, BattleMon *mon, u8 turns) {
    BattleHandlerMessageParam *message;
    s32 monId;

    monId = GetMonID(mon);
    message = BattleHandler_PushWork(flow, 4, monId);
    BattleHandler_StrSetup(&message->string, 2, 0x35c);
    BattleHandler_AddArg(&message->string, monId);
    BattleHandler_AddArg(&message->string, turns);
    BattleHandler_PopWork(flow, message);
}

// Checks the held item again when Embargo ends
// pokebw2's ConditionCuredEmbargo
static void func_ov095_0689c1a4(BtlServerFlow *flow, BattleMon *mon) {
    BattleHandlerCheckHeldItemParam *check;

    check = BattleHandler_PushWork(flow, 0x21, GetMonID(mon));
    check->monIndex = GetMonID(mon);
    check->reaction = 0;
    BattleHandler_PopWork(flow, check);
}

// Checks the held item again when Heal Block ends
// pokebw2's ConditionCuredHealBlock
static void func_ov095_0689c214(BtlServerFlow *flow, BattleMon *mon) {
    BattleHandlerCheckHeldItemParam *check;

    check = BattleHandler_PushWork(flow, 0x21, GetMonID(mon));
    check->monIndex = GetMonID(mon);
    check->reaction = 1;
    BattleHandler_PopWork(flow, check);
}

// pokebw2's GetConditionAddMessage
static s32 func_ov095_0689c284(s32 condition, BattleCondition value) {
    s32 message = -1;

    switch (condition) {
    case CONDITION_BURN:
        message = 0xff;
        break;
    case CONDITION_PARALYSIS:
        message = 0x111;
        break;
    case CONDITION_FREEZE:
        message = 0x120;
        break;
    case CONDITION_SLEEP:
        message = 0x132;
        break;
    case CONDITION_CONFUSION:
        message = 0x159;
        break;
    case CONDITION_ATTRACT:
        message = 0x147;
        break;
    case CONDITION_NIGHTMARE:
        message = 0x141;
        break;
    case CONDITION_TAUNT:
        message = 0x238;
        break;
    case CONDITION_TORMENT:
        message = 0x241;
        break;
    case CONDITION_YAWN:
        message = 0x29b;
        break;
    case CONDITION_LEECH_SEED:
        message = 0x25f;
        break;
    case CONDITION_FORESIGHT:
        message = 0x171;
        break;
    case CONDITION_INGRAIN:
        message = 0x2e0;
        break;
    case CONDITION_EMBARGO:
        message = 0x2d7;
        break;
    case CONDITION_DISABLE:
        message = 0x250;
        break;
    case CONDITION_HEAL_BLOCK:
        message = 0x371;
        break;
    case CONDITION_POISON:
        if (Condition_IsBadlyPoisoned(value)) {
            message = 0xed;
        } else {
            message = 0xea;
        }
        break;
    }
    return message;
}

// pokebw2's Condition_SetAddMessage
void data_ov095_0689c365(u32 condition, BattleCondition value, BattleMon *mon, BattleHandlerString *string) {
    s32 message = func_ov095_0689c284(condition, value);

    if (message >= 0) {
        BattleHandler_StrSetup(string, 2, message);
        BattleHandler_AddArg(string, GetMonID(mon));
        if (condition == CONDITION_BIND || condition == CONDITION_DISABLE) {
            BattleHandler_AddArg(string, Condition_GetParam(value));
        }
    } else {
        BattleHandler_StrClear(string);
    }
}

// The messages of conditions that ended
// pokebw2's sConditionCureMessages
static const ConditionCureMessage data_ov095_0689e21c[] = {
    { CONDITION_POISON, 0xf6, 0x395 },
    { CONDITION_BURN, 0x108, 0x3a1 },
    { CONDITION_SLEEP, 0x138, 0x39b },
    { CONDITION_FREEZE, 0x126, 0x39e },
    { CONDITION_PARALYSIS, 0x117, 0x398 },
    { 0x17, 0x232, -1 },
    { CONDITION_DISABLE, 0x256, -1 },
    { CONDITION_EMBARGO, 0x2da, -1 },
    { CONDITION_BIND, 0x177, 0x177 },
    { CONDITION_LEECH_SEED, 0x177, 0x177 },
    { 0x20, 0x477, -1 },
    { CONDITION_TAUNT, 0x23e, -1 },
    { 0x1e, 0x295, -1 },
    { CONDITION_HEAL_BLOCK, 0x374, -1 },
    { CONDITION_TORMENT, 0x247, -1 },
    { CONDITION_CONFUSION, 0x15f, 0x3a4 },
    { CONDITION_ATTRACT, 0x153, 0x3a7 },
};

// pokebw2's GetConditionCureMessage
s32 func_ov095_0689c424(u32 condition, BOOL alt) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov095_0689e21c); i++) {
        if (condition == data_ov095_0689e21c[i].condition) {
            s32 message;

            if (alt) {
                message = data_ov095_0689e21c[i].messageAlt;
            } else {
                message = data_ov095_0689e21c[i].message;
            }
            if (message < 0) {
                return data_ov095_0689e21c[i].message;
            }
            return message;
        }
    }
    return -1;
}

BOOL data_ov095_0689c465(u32 condition, BattleCondition cont, BattleMon *mon, u16 count, BattleHandlerString *string) {
    BOOL hasCount = count != 0;
    s32 message = func_ov095_0689c424(condition, hasCount);

    if (message >= 0) {
        BattleHandler_StrSetup(string, 2, message);
        BattleHandler_AddArg(string, GetMonID(mon));
        if (hasCount) {
            BattleHandler_AddArg(string, count);
        }
        switch (condition) {
        case CONDITION_BIND:
            BattleHandler_AddArg(string, Condition_GetParam(cont));
            break;
        case CONDITION_LEECH_SEED:
            BattleHandler_AddArg(string, 0x49);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Condition_CheckTypeImmunity
void data_ov095_0689c541(BtlServerFlow *flow, BattleMon *target) {
    BattleCondition cont;
    u8 pos;

    if (CheckCondition(target, CONDITION_FORESIGHT)) {
        cont = GetConditionContinuationParam(target, CONDITION_FORESIGHT);
        pos = Condition_GetParam(cont);
        if (pos == (u8)BattleEventVar_GetValue(0x15)) {
            if (func_ov093_021e8c24(cont) == 0) {
                BattleEventVar_RewriteValue(0x4b, TRUE);
            } else {
                BattleEventVar_RewriteValue(0x4c, TRUE);
            }
        }
    }
    if (CheckCondition(target, CONDITION_INGRAIN) && BattleEventVar_GetValue(0x16) == TYPE_GROUND) {
        BattleEventVar_RewriteValue(0x4b, TRUE);
    }
    if (CheckCondition(target, 0x1f) && BattleEventVar_GetValue(0x16) == TYPE_GROUND) {
        BattleEventVar_RewriteValue(0x4b, TRUE);
    }
}

// pokebw2's Condition_CheckTrapped
void data_ov095_0689c68d(BtlServerFlow *flow, BattleMon *mon) {
    if (CheckCondition(mon, CONDITION_MEAN_LOOK) || CheckCondition(mon, CONDITION_BIND)) {
        BattleEventVar_RewriteValue(0x41, TRUE);
    }
}

void Condition_CheckFloating(BtlServerFlow *flow, BattleMon *mon) {
    if (CheckCondition(mon, CONDITION_INGRAIN)) {
        BattleEventVar_RewriteValue(0x41, TRUE);
    }
    if (CheckCondition(mon, 0x1f)) {
        BattleEventVar_RewriteValue(0x41, TRUE);
    }
    if (CheckCondition(mon, 0x20) || CheckCondition(mon, 0x1e)) {
        BattleEventVar_RewriteValue(0x51, TRUE);
    }
}

// pokebw2's Condition_CheckIngrainForceSwitch
void data_ov095_0689c7a5(BtlServerFlow *flow, BattleMon *mon) {
    BattleHandlerMessageParam *message;
    s32 monId;

    if (CheckCondition(mon, CONDITION_INGRAIN) && BattleEventVar_RewriteValue(0x41, TRUE)) {
        monId = GetMonID(mon);
        message = BattleHandler_PushWork(flow, 4, monId);
        BattleHandler_StrSetup(&message->string, 2, 0x2e6);
        BattleHandler_AddArg(&message->string, monId);
        BattleHandler_PopWork(flow, message);
    }
}

// The conditions that Baton Pass passes on
// pokebw2's sBatonPassConditions
static const u16 data_ov095_0689e200[14] = {
    CONDITION_CONFUSION,
    CONDITION_HEAL_BLOCK,
    CONDITION_GASTRO_ACID,
    CONDITION_LEECH_SEED,
    CONDITION_EMBARGO,
    CONDITION_PERISH_SONG,
    CONDITION_INGRAIN,
    CONDITION_MEAN_LOOK,
    0x1d,
    0x1e,
    CONDITION_MEAN_LOOK,
    CONDITION_CURSE,
    0x20,
    0x23,
};

// pokebw2's Condition_IsPassedByBatonPass
BOOL data_ov095_0689c869(u32 condition) {
    u32 i;

    for (i = 0; i < NELEMS(data_ov095_0689e200); i++) {
        if (condition == data_ov095_0689e200[i]) {
            return TRUE;
        }
    }
    return FALSE;
}
