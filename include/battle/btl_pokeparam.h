// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_pokeparam.h, the lines in use
// so far.
#ifndef BWPORT_BATTLE_BTL_POKEPARAM_H
#define BWPORT_BATTLE_BTL_POKEPARAM_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"
#include "gfl/heap.h"

struct BattleCondition {
    union {
        u32 raw;
        struct {
            u32 type : 3;
            u32 turns : 6;
            u32 unk09 : 23;
        } common;
        struct {
            u32 type : 3;
            u32 turns : 6;
            u32 param : 16;
            u32 unk25 : 7;
        } timed;
        struct {
            u32 unk00 : 9;
            u32 monId : 6;
            u32 unk15 : 17;
        } mon;
        struct {
            u32 unk00 : 25;
            u32 flag : 1;
            u32 unk26 : 6;
        } one;
        struct {
            u32 unk00 : 31;
            u32 flag : 1;
        } four;
    };
};

// The condition word returned by GetConditionContinuationParam.
typedef BattleCondition BattleConditionCont;

BattleCondition ZeroConditionTurns(void);
BOOL func_ov093_021e8928(BattleCondition condition);
BattleCondition SetConditionTurns(u8 turns);
BattleCondition AddTurnCondition(u32 turns, u16 param);
BattleCondition func_ov093_021e899c(u32 turns);
BattleCondition MakeConditionPermanent(void);
BattleCondition MakeConditionParamPermanent(u16 param);
BattleCondition func_ov093_021e89f8(u8 turns, u16 param);
BattleCondition func_ov093_021e8a28(u32 monId, u8 turns);
BattleCondition func_ov093_021e8a58(void);
BOOL Condition_IsBadlyPoisoned(BattleConditionCont cont);
u8 Condition_GetMonID(BattleCondition condition);
void func_ov093_021e8ac8(BattleCondition *condition, u32 value);
u8 func_ov093_021e8afc(BattleCondition condition);
void func_ov093_021e8b28(BattleCondition *condition, u16 value);
u16 Condition_GetParam(BattleCondition condition);
void SetConditionFlag(BattleCondition *condition, u32 flag);
u32 func_ov093_021e8c24(BattleCondition condition);
void IncrementTurn(BattleCondition *condition, u32 amount);
void SetTurns(BattleCondition *condition, u32 turns);

// Two types, the first in bits 8 to 15 and the second in bits 0 to 7
typedef u16 PokeTypePair;

PokeTypePair PokeTypePair_Make(u32 type1, u32 type2);
PokeTypePair func_ov093_021e8cf0(u32 type);
u8 PokeTypePair_GetType1(PokeTypePair pair);
u32 PokeTypePair_GetType2(PokeTypePair pair);
void func_ov093_021e8d0c(PokeTypePair pair, u8 *type1, u8 *type2);
BOOL func_ov093_021e8d24(PokeTypePair pair, u32 type);
BOOL func_ov093_021e8d48(PokeTypePair first, PokeTypePair second);
BOOL PokeTypePair_IsMonotype(PokeTypePair pair);

BOOL CanPokemonBattle(BattleMon *mon);
BOOL CheckCondition(const BattleMon *mon, u32 index);
u32 GetAdditionalConditionFlag(BattleMon *mon, u32 flag);
u32 GetTurnFlag(BattleMon *mon, u32 flag);
u8 GetBattleMonMoveCount(BattleMon *mon);
u32 GetBattleMonStat(const BattleMon *mon, u32 stat);
u32 GetBattleMonStatus(BattleMon *mon);
u16 GetDisabledMove(BattleMon *mon, u32 index);
u8 GetConditionCount(BattleMon *mon, u32 index);
u32 GetConsecutiveMoveCount(BattleMon *mon);
fx32 GetHPRatio(BattleMon *mon);
u8 GetMonID(BattleMon *mon);
u16 GetMovePP(BattleMon *mon, u8 index);
PokeTypePair GetPokeType(BattleMon *mon);
u16 GetPreviousMoveID(BattleMon *mon);
BOOL IsFainted(const BattleMon *mon);
BOOL TransformCheck(BattleMon *mon);
void ChangeForm(BattleMon *mon, u8 form);
void ChangeAbility(BattleMon *mon, u32 ability);
void SetWeight(BattleMon *mon, u16 weight);
u16 GetBattleMonWeight(BattleMon *mon);
void HPAdd(BattleMon *mon, u16 amount);
void HPZero(BattleMon *mon);
BOOL IsMonFullHP(BattleMon *mon);
BOOL StatStageRecover(BattleMon *mon);
BOOL IsSubstituteActive(BattleMon *mon);
void ResetSpActPriority(BattleMon *mon);
void ComboMove_ClearParam(BattleMon *mon);
BOOL IsIllusionEnabled(BattleMon *mon);
void IllusionBreak(BattleMon *mon);
BOOL IsSemiInvulnMove(BattleMon *mon);
BOOL TransformSet(BattleMon *mon, BattleMon *target);
void RemoveForceAll(BattleMon *mon);
void AbilityEvent_RemoveItem(BattleMon *mon);
BattleEventItem *AbilityEvent_AddItem(BattleMon *mon);
u16 MoveGetID(BattleMon *mon, u8 index);
u16 GetPreviousMoveUsed(BattleMon *mon);
u8 GetPrevTargetPos(BattleMon *mon);
void MoveWork_UpdateNumber(BattleMoveWork *work, u16 move, u8 maxPP, BOOL updateCurrent);
void MoveCore_UpdateNumber(BattleMoveCore *core, u16 move, u8 maxPP);
void ClearUsedMoveFlag(BattleMon *mon);
void ClearMoveStatusWork(BattleMon *mon, u32 flag);
void ClearCounter(BattleMon *mon);
void setupBySrcData(BattleMon *mon, PartyPkm *src, BOOL readHP, BOOL readAbility);
void MoveWork_ClearSurface(BattleMon *mon);
void ClearFormChange(BattleMon *mon);
BOOL Move_IsPPFull(BattleMon *mon, u8 index, BOOL truth);
u16 Move_IncrementPP(BattleMon *mon, u8 index, u8 amount);
u16 Move_IncrementPP_Org(BattleMon *mon, u8 index, u8 amount);
void Move_UpdateID(BattleMon *mon, u8 index, u16 move, u8 maxPP, BOOL updateCurrent);
BOOL MoveIsUsable(const BattleMon *mon, u16 move);
void SetBaseStatus(BattleMon *mon, u32 stat, u16 value);
s32 RawBattleMonStat(const BattleMon *mon, u32 stat);
void splitTypeCore(BattleMon *mon, u8 *type1, u8 *type2);
BOOL DoesMonHaveType(BattleMon *mon, u32 type);
void SetIllusionDisguise(BattleMon *mon, PartyPkm *disguise);
BOOL IsStatChangeValid(BattleMon *mon, u32 stat, s32 change);
BOOL AreStatsLowered(BattleMon *mon);
void SetMoveCondition(BattleMon *mon, u32 condition, BattleCondition value);
void CureCondition(BattleMon *mon);
void CureDependentCondition(BattleMon *mon, u32 condition);
void CureMoveCondition(BattleMon *mon, u32 condition);
u32 GetNumMoves(BattleMon *mon, PartyPkm *src, BOOL reset);
void SetMovesAndPP(BattleMon *mon);
void setupBySrcDataBase(BattleMon *mon, PartyPkm *src, BOOL readTypes);
u8 CheckIfMoveWasUsed(BattleMon *mon, u8 index);
void Clear_ForFainted(BattleMon *mon);
BOOL GetDamageReceived(const BattleMon *mon, u8 turnsAgo, u8 index, BattleMonDamageRecord *record);
void COUNTER_Set(BattleMon *mon, u32 index, u8 value);

// A move slot of a BattleMon: its true move and the surface one in use, which a move like Mimic or Transform
// replaces (MoveWork_ClearSurface resets it)
struct BattleMoveCore {
    u16 id;
    union {
        struct {
            u8 pp;
            u8 maxPP;
        };
        u16 ppPair;
    };
    union {
        struct {
            u8 ppUp;
            u8 flagsLow : 4;
            u8 flagsHigh : 4;
        };
        u16 flagsPair;
    };
};

struct BattleMoveWork {
    BattleMoveCore truth;
    BattleMoveCore surface;
    // Set while the surface is the true move, so PP changes go to both
    u8 linked;
};

// A BattleMon's stat stages, from 0 to 12 with 6 neutral
struct BattleMonStatStages {
    s8 attack;
    s8 defense;
    s8 spAttack;
    s8 spDefense;
    s8 speed;
    s8 accuracy;
    s8 evasion;
};

// Black 2's; twinned code confirms the offsets of move, damage, attackerId and attackerPos in Black
struct BattleMonDamageRecord {
    u16 move;
    u16 damage;
    u8 type;
    u8 attackerId;
    u8 attackerPos;
    u8 unk7;
};

// Black 2's, not checked against Black
// A condition's ID. The CONDITION_* values are #defines in constants/battle.h, which tr_ai.inc includes too. MWCC
// keeps a local of this type in a register where it folds an integer one into each use, as HandlerChatter's status
typedef enum {
    CONDITION_ID_COUNT = 36,
} BattleConditionID;

// Black 2's; twinned code confirms the offsets of src, illusionDisguise, exp, species, maxHP, hp, heldItem,
// baseAbility, level, monId, unk1a, baseForm, transformed, illusion, unk1b_7, conditions, conditionCounters, unkD0Count
// and unkD1 in Black
// The first part of a BattleMon, which TransformSet keeps while it copies the rest from the target
struct BattleMonCore {
    PartyPkm *src;
    PartyPkm *illusionDisguise;
    u32 exp;
    u16 species;
    u16 maxHP;
    u16 hp;
    u16 heldItem;
    u16 consumedItem;
    u16 baseAbility;
    u8 level;
    u8 monId;
    u8 unk1a;
    u8 baseForm : 5;
    u8 transformed : 1;
    u8 illusion : 1;
    u8 unk1b_7 : 1;
    BattleCondition conditions[CONDITION_ID_COUNT];
    u8 conditionCounters[CONDITION_ID_COUNT];
    // The IDs of up to 24 Pokémon, each recorded once
    u8 unkD0Count;
    u8 unkD1[24];
};

// Black 2's; twinned code confirms the offsets of core, attack, defense, spAttack, spDefense, speed, type1, type2, sex,
// statStages, moves, ability, weight, moveCount, form, unk142, unk143, unk144, unk146, unk148, prevMoveUsed,
// prevMoveId, consecutiveMoveCount, prevTargetPos, turnFlags, conditionFlags, counters, damageRecords,
// damageRecordCounts, damageRecordTurn, unk1f0, substituteHP, comboMove and comboMonId in Black
// A Pokémon in battle, 0x1f8 bytes from BattleMon_Create. Offsets are from btl_pokeparam.c's accessors.
struct BattleMon {
    BattleMonCore core;
    u16 unkEC;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
    u8 type1;
    u8 type2;
    u8 sex;
    u8 unkFB;
    BattleMonStatStages statStages;
    u8 unk103;
    BattleMoveWork moves[4];
    u16 ability;
    u16 weight;
    u8 moveCount;
    u8 form;
    u8 unk142;
    u8 unk143;
    u8 unk144;
    u8 unk145;
    u16 unk146;
    u16 unk148;
    u16 prevMoveUsed;
    u16 prevMoveId;
    u16 consecutiveMoveCount;
    u8 unk150[2];
    u8 prevTargetPos;
    u8 turnFlags[2];
    u8 conditionFlags[2];
    u8 counters[5];
    // The hits of the last 3 turns, 6 each
    BattleMonDamageRecord damageRecords[3][6];
    u8 damageRecordCounts[3];
    u8 damageRecordTurn;
    u8 unk1f0;
    u8 unk1f1;
    u16 substituteHP;
    u16 comboMove;
    u8 comboMonId;
    u8 unk1f7;
};

struct BattleMonLevelUp {
    u8 level;
    u16 hp;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
};

void ChangePokeType(BattleMon *mon, u16 type);
void ClearConsumedItem(BattleMon *mon);
void ConsumeItem(BattleMon *mon, u16 item);
void ResetStatStages(BattleMonStatStages *stages);
u32 CritAtkDefLevel(BattleMon *mon, u32 stat);
void Clear_ForSwitch(BattleMon *mon);
u32 GetExpForLv100(BattleMon *mon);
void func_ov093_021d50b4(BattleMon *mon);
void func_ov093_021d51cc(BattleMoveWork *move);
BOOL func_ov093_021d5244(BattleMoveCore *core, PartyPkm *src, u8 index);
BOOL func_ov093_021d53dc(BattleMonStatStages *stages);
u8 func_ov093_021d5450(BattleMon *mon);
// pokebw2's CountUsedMoves
u8 func_ov093_021d5478(const BattleMon *mon);
u8 func_ov093_021d54b4(BattleMon *mon);
u16 func_ov093_021d54d0(BattleMon *mon, u8 index);
void func_ov093_021d54f4(BattleMon *mon, BattleMon *dest);
u16 func_ov093_021d5528(BattleMon *mon, u8 index, u8 *pp, u8 *maxPP);
// pokebw2's GetMovePPUsed
u8 func_ov093_021d554c(BattleMon *mon, u8 index);
u8 func_ov093_021d5568(BattleMon *mon, u8 index);
u8 func_ov093_021d55a0(const BattleMon *mon, u16 move);
void func_ov093_021d5608(BattleMon *mon, u8 index, u8 amount);
void func_ov093_021d5640(BattleMon *mon, u8 index, u8 amount);
void func_ov093_021d56cc(BattleMon *mon, u8 index);
u8 func_ov093_021d5778(BattleMon *mon, u16 move);
void func_ov093_021d5854(BattleMon *mon);
PartyPkm *func_ov093_021d5864(const BattleMon *mon);
u32 func_ov093_021d587c(const BattleMon *mon, u32 stat);
void func_ov093_021d590c(BattleMon *mon, BattleMonLevelUp *stats);
u16 func_ov093_021d5ba4(BattleMon *mon);
u32 func_ov093_021d5c08(BattleMon *mon);
s8 *func_ov093_021d5cb4(BattleMon *mon, u32 stat, s8 *min, s8 *max);
s32 func_ov093_021d5d50(BattleMon *mon, u32 stat);
u32 func_ov093_021d5dc0(BattleMon *mon, u32 stat, u8 amount);
u32 func_ov093_021d5e38(BattleMon *mon, u32 stat, u8 amount);
void func_ov093_021d5ea8(BattleMon *mon, u32 stat, u8 value);
u8 func_ov093_021d5f14(BattleMon *mon);
BOOL func_ov093_021d5f38(BattleMon *mon, s32 amount);
void func_ov093_021d5f90(BattleMon *mon, u16 amount);
void func_ov093_021d5fc0(BattleMon *mon, u32 flag);
void func_ov093_021d5fe4(BattleMon *mon, u32 flag);
void func_ov093_021d6008(BattleMon *mon, u32 flag);
BOOL func_ov093_021d6064(BattleMon *mon, u32 index, BattleCondition *prev, BOOL *cured);
BOOL func_ov093_021d6130(BattleMon *mon);
BOOL func_ov093_021d61a8(BattleMon *mon);
void func_ov093_021d6264(BattleMon *mon, u32 monId);
u8 func_ov093_021d631c(BattleMon *mon, u32 index);
u32 func_ov093_021d6324(BattleMon *mon, u32 index);
void func_ov093_021d63ec(BattleMon *mon, u16 turn);
void func_ov093_021d6408(BattleMon *mon);
void func_ov093_021d6440(BattleMon *mon, u32 flag);
void func_ov093_021d6580(BattleMon *mon);
void func_ov093_021d6744(BattleMon *mon, u8 targetPos, BOOL success, u8 unk144, u16 moveId, u16 moveUsed);
u8 func_ov093_021d67b0(BattleMon *mon);
BOOL func_ov093_021d67d0(BattleMon *mon);
void func_ov093_021d67f4(BattleMon *mon);
void func_ov093_021d6824(BattleMon *mon);
void func_ov093_021d6848(BattleMon *mon, const BattleMonDamageRecord *record);
u8 func_ov093_021d6920(BattleMon *mon, u8 turnsAgo);
BOOL func_ov093_021d69b8(BattleMon *mon, u32 *exp, BattleMonLevelUp *levelUp);
void func_ov093_021d6b84(BattleMon *mon, BOOL keepBaseForm);
void func_ov093_021d6bfc(BattleMon *mon);
void func_ov093_021d6ea0(PartyPkm **pkm);
void func_ov093_021d6c3c(BattleMon *mon, PartyPkm *src);
void func_ov093_021d6d5c(BattleMon *mon, u16 value);
u32 func_ov093_021d6d90(BattleMon *mon);
BOOL func_ov093_021d6d9c(BattleMon *mon, u16 *damage);
void func_ov093_021d6dc4(BattleMon *mon);
void func_ov093_021d6dcc(BattleMon *mon, u8 monId);
u8 func_ov093_021d6e04(BattleMon *mon);
u8 func_ov093_021d6e0c(BattleMon *mon, u8 index);
void func_ov093_021d6e24(BattleMon *mon, u16 item);
void func_ov093_021d6e40(BattleMon *mon, u8 monId, u16 move);
BOOL func_ov093_021d6e50(BattleMon *mon, u8 *monId, u16 *move);
BOOL func_ov093_021d6e74(BattleMon *mon);


// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
PartyPkm *GetSrcData(const BattleMon *mon);
void CopyBatonPassParams(BattleMon *target, BattleMon *source);
u32 GetBattleMonHeldItem(BattleMon *mon);
u16 GetBattleMonSpecies(const BattleMon *mon);
BattleConditionCont GetConditionContinuationParam(BattleMon *mon, u32 index);
void StatStageReset(BattleMon *mon);
BattleMon *BattleMon_Create(PartyPkm *src, u8 monId, HeapID heapId);
void SetItem(BattleMon *mon, u16 item);
extern const u32 data_ov093_021f040c[];
void func_ov093_021d53b8(BattleMon *mon);
void func_ov093_021d53c0(BattleMonStatStages *stages);
// pokebw2's GetConsumedItem
u16 func_ov093_021d6740(BattleMon *mon);

// Black 2's, not checked against Black
static inline void BattleMonDamageRecord_Init(BattleMonDamageRecord *record, u8 attackerId, u8 attackerPos, u16 move,
                                              u8 type, u16 damage) {
    record->move = move;
    record->damage = damage;
    record->type = type;
    record->attackerId = attackerId;
    record->attackerPos = attackerPos;
}

#endif // BWPORT_BATTLE_BTL_POKEPARAM_H
