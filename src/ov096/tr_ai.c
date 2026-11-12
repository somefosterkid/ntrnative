// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov170/tr_ai.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_field.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "battle/btlv_effect.h"
#include "battle/tr_ai.h"
#include "constants/abilities.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/tr_ai.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "nitro/os.h"
#include "pml/item.h"
#include "pml/personal.h"
#include "pml/waza.h"
#include "system/vm.h"
#include "constants/battle.h"

// The AI stops thinking for the frame once it has run for this many ticks
#define TRAI_TIME_LIMIT 500

#define TRAI_MOVE_MAX 4
#define TRAI_SCORE_INITIAL 100

// The scripts that are loaded at once, each for one AI flag
#define TRAI_SCRIPT_CACHE_SIZE 4
#define TRAI_SCRIPT_NONE 0xff

// TrAIContext.status
#define TRAI_STATUS_MOVE_DONE 0x1
#define TRAI_STATUS_FLEE 0x2
#define TRAI_STATUS_SKIP_MOVES 0x8
// The AI ran out of time, and continues where it stopped in the next call
#define TRAI_STATUS_SUSPENDED 0x10

// TrAIContext.seq
#define TRAI_SEQ_LOAD_SCRIPT 0
#define TRAI_SEQ_RUN_SCRIPT 1
#define TRAI_SEQ_DONE 2

// How AIConditionalJump compares a value with the script's value
enum {
    AI_CMP_LT,
    AI_CMP_GT,
    AI_CMP_EQ,
    AI_CMP_NE,
    AI_CMP_AND,
    AI_CMP_NAND,
    AI_CMP_LE,
    AI_CMP_GE,
};

// Black 2's; twinned code confirms the offsets of seq, moveIdx, moveId, scores, initialScores, bestScores, bestMoves,
// result, flagsRemaining, aiFlags, status, scriptIndex, chosenMove, chosenTarget, damageRandom, knownMoves,
// knownAbilities, attackerPos, defenderPos, battleStyle, heapId, scriptArc, itemArc, script, mainModule, serverFlow,
// pokeCon, attacker, defender, cmdReturn, unkC8, random and startTick in Black
typedef struct {
    u8 seq;
    u8 moveIdx;
    u16 moveId;
    s32 scores[TRAI_MOVE_MAX];
    s32 initialScores[TRAI_MOVE_MAX];
    // The best score and move against each position, when choosing a target
    s16 bestScores[BTL_POS_MAX];
    s8 bestMoves[BTL_POS_MAX];
    // The value that commands load and compare
    s32 result;
    // The AI flags whose scripts are still to run, shifted so that the next one is bit 0
    u32 flagsRemaining;
    u32 aiFlags;
    u8 status;
    // The AI flag whose script is running, which is also its file in the AI script archive
    u8 scriptIndex;
    u8 chosenMove;
    u8 chosenTarget;
    u8 damageRandom[TRAI_MOVE_MAX];
    // The moves that each position was seen to use
    u16 knownMoves[BTL_POS_MAX][TRAI_MOVE_MAX];
    u8 knownAbilities[BTL_POS_MAX];
    u8 unk82[0x12];
    u8 attackerPos;
    u8 defenderPos;
    u32 battleStyle;
    u32 battleType;
    HeapID heapId;
    ArcTool *scriptArc;
    ArcTool *itemArc;
    void *script;
    BtlMainModule *mainModule;
    BtlServerFlow *serverFlow;
    BtlPokeCon *pokeCon;
    BattleMon *attacker;
    BattleMon *defender;
    // What every command returns, so VM_Run returns after each command
    BOOL cmdReturn;
    u32 unkC8;
    // A random number for the turn
    u32 random;
    u64 startTick;
} TrAIContext;

typedef struct {
    u8 fileIds[TRAI_SCRIPT_CACHE_SIZE];
    u8 refCounts[TRAI_SCRIPT_CACHE_SIZE];
    void *scripts[TRAI_SCRIPT_CACHE_SIZE];
} TrAIScriptCache;

BOOL func_ov096_0218814c(VM *vm, TrAIContext *wk);
BOOL func_ov096_02188284(VM *vm, TrAIContext *wk);
void func_ov096_021885bc(VM *vm, TrAIContext *wk);
BOOL AIGreaterThanRandom(VM *vm, void *work);
BOOL AILessThanRandom(VM *vm, void *work);
BOOL AIEqualToRandom(VM *vm, void *work);
BOOL AINotEqualToRandom(VM *vm, void *work);
void CompareToRandom(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIncrementScore(VM *vm, void *work);
BOOL AIIsHPLessThan(VM *vm, void *work);
BOOL AIIsHPGreaterThan(VM *vm, void *work);
BOOL AIIsHPEqualTo(VM *vm, void *work);
BOOL AIIsHPNotEqualTo(VM *vm, void *work);
void CompareHP(VM *vm, TrAIContext *wk, u32 op);
BOOL AIHasStatus(VM *vm, void *work);
BOOL AIDoesNotHaveStatus(VM *vm, void *work);
void CheckStatusCondition(VM *vm, TrAIContext *wk, u32 op);
BOOL AIHasCondition(VM *vm, void *work);
BOOL AIDoesNotHaveCondition(VM *vm, void *work);
void func_ov096_021888bc(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIsBadlyPoisoned(VM *vm, void *work);
BOOL AIIsNotBadlyPoisoned(VM *vm, void *work);
void func_ov096_02188924(VM *vm, TrAIContext *wk, u32 op);
BOOL AIHasConditionFlag(VM *vm, void *work);
BOOL AIDoesNotHaveConditionFlag(VM *vm, void *work);
void func_ov096_021889a0(VM *vm, TrAIContext *wk, u32 op);
BOOL func_ov096_021889e8(VM *vm, void *work);
BOOL func_ov096_021889f8(VM *vm, void *work);
void func_ov096_02188a08(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfLessThan(VM *vm, void *work);
BOOL AIIfGreaterThan(VM *vm, void *work);
BOOL AIIfEqual(VM *vm, void *work);
BOOL AIIfNotEqual(VM *vm, void *work);
BOOL AIIfBit(VM *vm, void *work);
BOOL AIIfNotBit(VM *vm, void *work);
void func_ov096_02188ab4(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfMove(VM *vm, void *work);
BOOL AIIfNotMove(VM *vm, void *work);
void CheckMove(VM *vm, TrAIContext *wk, u32 op);
BOOL func_ov096_02188b1c(VM *vm, void *work);
BOOL func_ov096_02188b64(VM *vm, void *work);
BOOL AIHasDamagingMove(VM *vm, void *work);
BOOL AIDoesNotHaveDamagingMove(VM *vm, void *work);
BOOL AIGetTurnCount(VM *vm, void *work);
BOOL AIGetType(VM *vm, void *work);
BOOL AIGetBasePower(VM *vm, void *work);
BOOL AIGetHighestDamagingMove(VM *vm, void *work);
BOOL AIGetPreviousMove(VM *vm, void *work);
BOOL AICompareSpeed(VM *vm, void *work);
BOOL AICheckTeamCount(VM *vm, void *work);
BOOL AICheckMoveID(VM *vm, void *work);
BOOL AICheckMoveEffect(VM *vm, void *work);
BOOL AICheckAbility(VM *vm, void *work);
BOOL AINop43(VM *vm, void *work);
BOOL AIIfTypeEffectiveness(VM *vm, void *work);
BOOL AIIfStatusInParty(VM *vm, void *work);
BOOL AIIfStatusNotInParty(VM *vm, void *work);
void func_ov096_02188fcc(VM *vm, TrAIContext *wk, u32 op);
BOOL AIGetWeather(VM *vm, void *work);
BOOL AIHasMoveID(VM *vm, void *work);
BOOL AIDoesNotHaveMoveID(VM *vm, void *work);
void CompareMoveID(VM *vm, TrAIContext *wk, u32 op);
BOOL AIStatStageLessThan(VM *vm, void *work);
BOOL AIStatStageGreaterThan(VM *vm, void *work);
BOOL AIStatStageEqual(VM *vm, void *work);
BOOL AIStatStageNotEqual(VM *vm, void *work);
void CompareStatStage(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfCanFaint(VM *vm, void *work);
BOOL AIIfCannotFaint(VM *vm, void *work);
void func_ov096_02189170(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfHasMove(VM *vm, void *work);
BOOL AIIfDoesNotHaveMove(VM *vm, void *work);
void func_ov096_021891f4(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfHasMoveWithEffect(VM *vm, void *work);
BOOL AIIfDoesNotHaveMoveWithEffect(VM *vm, void *work);
void func_ov096_021892f8(VM *vm, TrAIContext *wk, u32 op);
BOOL AINop60(VM *vm, void *work);
BOOL func_ov096_021893b4(VM *vm, void *work);
BOOL AINop62(VM *vm, void *work);
BOOL AINop63(VM *vm, void *work);
BOOL AIGetHeldItem(VM *vm, void *work);
BOOL AIGetItemEffect(VM *vm, void *work);
BOOL AIGetGender(VM *vm, void *work);
BOOL AIIsFakeOutActive(VM *vm, void *work);
BOOL AIGetStockpileCount(VM *vm, void *work);
BOOL func_ov096_021894b4(VM *vm, void *work);
BOOL func_ov096_021894c4(VM *vm, void *work);
BOOL func_ov096_021894d4(VM *vm, void *work);
BOOL AINop72(VM *vm, void *work);
BOOL AIGetMovePower(VM *vm, void *work);
BOOL AIGetMoveID(VM *vm, void *work);
BOOL AIGetProtectCount(VM *vm, void *work);
BOOL AIJump(VM *vm, void *work);
BOOL AIEnd(VM *vm, void *work);
BOOL AICompareLevel(VM *vm, void *work);
BOOL AIIfTaunted(VM *vm, void *work);
BOOL AIIfNotTaunted(VM *vm, void *work);
void CheckTauntStatus(VM *vm, TrAIContext *wk, u32 op);
BOOL AIIfTargetIsAlly(VM *vm, void *work);
BOOL AIDoesMonHaveType(VM *vm, void *work);
BOOL AIGuessAbility(VM *vm, void *work);
BOOL AIIfFlashFireIsActive(VM *vm, void *work);
BOOL AIIfHasItem(VM *vm, void *work);
BOOL func_ov096_0218979c(VM *vm, void *work);
BOOL func_ov096_021897cc(VM *vm, void *work);
BOOL func_ov096_02189800(VM *vm, void *work);
BOOL func_ov096_0218989c(VM *vm, void *work);
BOOL AIGetFlingPower(VM *vm, void *work);
BOOL AIGetMovePP(VM *vm, void *work);
BOOL func_ov096_021899dc(VM *vm, void *work);
BOOL AIGetMoveCategory(VM *vm, void *work);
BOOL AIGetLastMoveCategory(VM *vm, void *work);
BOOL AIGetOrderInTurn(VM *vm, void *work);
BOOL func_ov096_02189a9c(VM *vm, void *work);
BOOL func_ov096_02189ac0(VM *vm, void *work);
BOOL func_ov096_02189b78(VM *vm, void *work);
BOOL func_ov096_02189bfc(VM *vm, void *work);
BOOL AIGetPositiveStatStageTotal(VM *vm, void *work);
BOOL AIGetStatStageDifference(VM *vm, void *work);
BOOL AINop102(VM *vm, void *work);
BOOL AINop103(VM *vm, void *work);
BOOL AINop104(VM *vm, void *work);
BOOL func_ov096_02189d48(VM *vm, void *work);
BOOL AIIsFainted(VM *vm, void *work);
BOOL AIIsNotFainted(VM *vm, void *work);
void CheckIfFainted(VM *vm, TrAIContext *wk, u32 op);
BOOL AIGetAbility(VM *vm, void *work);
BOOL AIIfSubstitute(VM *vm, void *work);
BOOL AIGetSpecies(VM *vm, void *work);
BOOL func_ov096_02189f58(VM *vm, void *work);
BOOL func_ov096_02189f68(VM *vm, void *work);
BOOL func_ov096_02189f78(VM *vm, void *work);
BOOL func_ov096_02189f88(VM *vm, void *work);
void func_ov096_02189f98(VM *vm, TrAIContext *wk, u32 op);
BOOL func_ov096_02189fc0(VM *vm, void *work);
BOOL func_ov096_0218a034(VM *vm, void *work);
BOOL AIIsAtkLessThanSpAtk(VM *vm, void *work);
BOOL AIIsAtkGreaterThanSpAtk(VM *vm, void *work);
BOOL AIIsAtkEqualToSpAtk(VM *vm, void *work);
void CompareOffensiveStats(VM *vm, TrAIContext *wk, u32 op);
BOOL AIConditionalJump(VM *vm, u32 op, s32 value, s32 ref, s32 offset);
u8 func_ov096_0218a160(TrAIContext *wk, u32 side);
u32 GuessAbility(TrAIContext *wk, u32 side, u8 pos);
BattleMon *func_ov096_0218a2fc(TrAIContext *wk, u8 pos);
BattleMon *func_ov096_0218a308(BattleParty *party, u8 index);
void func_ov096_0218a310(TrAIContext *wk);
void func_ov096_0218a324(TrAIContext *wk);
s32 GetMoveData(TrAIContext *wk, u16 move, u32 param);
BOOL func_ov096_0218a384(TrAIContext *wk, u16 move, u32 flag);
u32 func_ov096_0218a390(TrAIContext *wk, BattleMon *attacker, BattleMon *defender, u32 damageRoll);
s32 func_ov096_0218a3f4(TrAIContext *wk, u16 item, u32 param);
void func_ov096_0218a41c(HeapID heapId);
void func_ov096_0218a458(void);
void *func_ov096_0218a494(TrAIContext *wk);
void func_ov096_0218a528(TrAIContext *wk);

// pokebw2's sTrAICommands
static const VMCommand data_ov096_0218a5a8[] = {
    AIGreaterThanRandom,
    AILessThanRandom,
    AIEqualToRandom,
    AINotEqualToRandom,
    AIIncrementScore,
    AIIsHPLessThan,
    AIIsHPGreaterThan,
    AIIsHPEqualTo,
    AIIsHPNotEqualTo,
    AIHasStatus,
    AIDoesNotHaveStatus,
    AIHasCondition,
    AIDoesNotHaveCondition,
    AIIsBadlyPoisoned,
    AIIsNotBadlyPoisoned,
    AIHasConditionFlag,
    AIDoesNotHaveConditionFlag,
    func_ov096_021889e8,
    func_ov096_021889f8,
    AIIfLessThan,
    AIIfGreaterThan,
    AIIfEqual,
    AIIfNotEqual,
    AIIfBit,
    AIIfNotBit,
    AIIfMove,
    AIIfNotMove,
    func_ov096_02188b1c,
    func_ov096_02188b64,
    AIHasDamagingMove,
    AIDoesNotHaveDamagingMove,
    AIGetTurnCount,
    AIGetType,
    AIGetBasePower,
    AIGetHighestDamagingMove,
    AIGetPreviousMove,
    AIIfEqual,
    AIIfNotEqual,
    AICompareSpeed,
    AICheckTeamCount,
    AICheckMoveID,
    AICheckMoveEffect,
    AICheckAbility,
    AINop43,
    AIIfTypeEffectiveness,
    AIIfStatusInParty,
    AIIfStatusNotInParty,
    AIGetWeather,
    AIHasMoveID,
    AIDoesNotHaveMoveID,
    AIStatStageLessThan,
    AIStatStageGreaterThan,
    AIStatStageEqual,
    AIStatStageNotEqual,
    AIIfCanFaint,
    AIIfCannotFaint,
    AIIfHasMove,
    AIIfDoesNotHaveMove,
    AIIfHasMoveWithEffect,
    AIIfDoesNotHaveMoveWithEffect,
    AINop60,
    func_ov096_021893b4,
    AINop62,
    AINop63,
    AIGetHeldItem,
    AIGetItemEffect,
    AIGetGender,
    AIIsFakeOutActive,
    AIGetStockpileCount,
    func_ov096_021894b4,
    func_ov096_021894c4,
    func_ov096_021894d4,
    AINop72,
    AIGetMovePower,
    AIGetMoveID,
    AIGetProtectCount,
    AIJump,
    AIEnd,
    AICompareLevel,
    AIIfTaunted,
    AIIfNotTaunted,
    AIIfTargetIsAlly,
    AIDoesMonHaveType,
    AIGuessAbility,
    AIIfFlashFireIsActive,
    AIIfHasItem,
    func_ov096_0218979c,
    func_ov096_021897cc,
    func_ov096_02189800,
    func_ov096_0218989c,
    AIGetFlingPower,
    AIGetMovePP,
    func_ov096_021899dc,
    AIGetMoveCategory,
    AIGetLastMoveCategory,
    AIGetOrderInTurn,
    func_ov096_02189a9c,
    func_ov096_02189ac0,
    func_ov096_02189b78,
    func_ov096_02189bfc,
    AIGetPositiveStatStageTotal,
    AIGetStatStageDifference,
    AINop102,
    AINop103,
    AINop104,
    func_ov096_02189d48,
    AIIsFainted,
    AIIsNotFainted,
    AIGetAbility,
    AIIfSubstitute,
    AIGetSpecies,
    func_ov096_02189f58,
    func_ov096_02189f68,
    func_ov096_02189f78,
    func_ov096_02189f88,
    func_ov096_02189fc0,
    func_ov096_0218a034,
    AIIsAtkLessThanSpAtk,
    AIIsAtkGreaterThanSpAtk,
    AIIsAtkEqualToSpAtk,
};

extern const VMInitParam data_ov096_0218a574;

// pokebw2's sScriptCache
static TrAIScriptCache *data_ov096_0218a7c0;

// pokebw2's TrAI_Think

VM *TrAI_CreateVM(BtlMainModule *mainModule, BtlServerFlow *serverFlow, BtlPokeCon *pokeCon, u32 aiFlags, HeapID heapId) {
    TrAIContext *wk = GFL_HeapAllocate(heapId, sizeof(TrAIContext), TRUE, "tr_ai.c", 473);
    VM *vm;

    wk->heapId = heapId;
    wk->cmdReturn = TRUE;
    wk->aiFlags = aiFlags;
    wk->scriptArc = GFL_ArcSysCreateFileHandle(ARCID_TRAI_SCRIPT, heapId);
    wk->itemArc = PML_ItemArcHandleCreate(heapId);
    wk->mainModule = mainModule;
    wk->serverFlow = serverFlow;
    wk->pokeCon = pokeCon;
    wk->battleStyle = BtlSetup_GetBattleStyle(mainModule);
    wk->battleType = BtlSetup_GetBattleType(mainModule);
    vm = VM_Create(heapId, &data_ov096_0218a574);
    VM_ChangeEnv(vm, wk);
    if (!PML_MoveIsDataCachePresent()) {
        PML_MoveInitDataCache(48, heapId);
    }
    func_ov096_0218a41c(wk->heapId);
    return vm;
}

BOOL func_ov096_02187fb0(VM *vm) {
    TrAIContext *wk = VM_GetEnv(vm);
    s32 targets[3];
    s32 count;
    BOOL suspended;

    if (wk->battleStyle == BTL_STYLE_SINGLE) {
        wk->defenderPos = wk->attackerPos ^ 1;
        return func_ov096_0218814c(vm, wk);
    }
    if (wk->battleStyle == BTL_STYLE_DOUBLE || wk->battleStyle == BTL_STYLE_TRIPLE) {
        return func_ov096_02188284(vm, wk);
    }

    // A rotation battle: think against a random opponent
    count = 0;
    for (wk->defenderPos = 0; wk->defenderPos < BTL_POS_MAX; wk->defenderPos++) {
        if (wk->defenderPos != wk->attackerPos && (wk->defenderPos & 1) != (wk->attackerPos & 1) &&
            IsFainted(func_ov096_0218a2fc(wk, wk->defenderPos)) != TRUE) {
            targets[count++] = wk->defenderPos;
        }
    }
    wk->defenderPos = targets[GFL_RandomMTRange(count)];
    suspended = func_ov096_0218814c(vm, wk);
    if (!suspended) {
        wk->chosenTarget = 0;
    }
    return suspended;
}

// pokebw2's TrAI_DeleteVM
void func_ov096_02188080(VM *vm) {
    TrAIContext *wk = VM_GetEnv(vm);

    GFL_ArcToolFree(wk->scriptArc);
    GFL_ArcToolFree(wk->itemArc);
    GFL_HeapFree(wk);
    VM_Free(vm);
    if (PML_MoveIsDataCachePresent() == TRUE) {
        PML_MoveFreeDataCache();
    }
    func_ov096_0218a458();
}

// pokebw2's TrAI_Setup
void func_ov096_021880bc(VM *vm, const u8 *usableMoves, u8 attackerPos) {
    TrAIContext *wk = VM_GetEnv(vm);
    s32 i;

    wk->seq = TRAI_SEQ_LOAD_SCRIPT;
    wk->moveId = 0;
    wk->moveIdx = 0;
    wk->result = 0;
    wk->status = 0;
    wk->scriptIndex = 0;
    wk->attackerPos = attackerPos;
    wk->flagsRemaining = wk->aiFlags;
    wk->unkC8 = 4;
    for (i = 0; i < TRAI_MOVE_MAX; i++) {
        if (usableMoves[i] == 0) {
            wk->initialScores[i] = 0;
        } else {
            wk->initialScores[i] = TRAI_SCORE_INITIAL;
        }
        wk->damageRandom[i] = 100 - GFL_RandomMTRange(16);
    }
    wk->random = GFL_RandomMTRange(256);
}

// pokebw2's TrAI_GetChosenMove
u8 func_ov096_02188134(VM *vm) {
    TrAIContext *wk = VM_GetEnv(vm);

    return wk->chosenMove;
}

// pokebw2's TrAI_GetChosenTarget
u8 func_ov096_02188140(VM *vm) {
    TrAIContext *wk = VM_GetEnv(vm);

    return wk->chosenTarget;
}

// pokebw2's TrAI_ChooseMove
BOOL func_ov096_0218814c(VM *vm, TrAIContext *wk) {
    s32 bestScores[TRAI_MOVE_MAX];
    u8 bestMoves[TRAI_MOVE_MAX];
    s32 count;
    s32 i;
    u8 move;

    wk->attacker = func_ov096_0218a2fc(wk, wk->attackerPos);
    wk->defender = func_ov096_0218a2fc(wk, wk->defenderPos);
    if (!(wk->status & TRAI_STATUS_SUSPENDED)) {
        func_ov096_0218a310(wk);
        func_ov096_0218a324(wk);
    }
    while (wk->flagsRemaining != 0) {
        if (wk->flagsRemaining & 1) {
            if (!(wk->status & TRAI_STATUS_SUSPENDED)) {
                wk->seq = TRAI_SEQ_LOAD_SCRIPT;
            }
            wk->status &= (u8)~TRAI_STATUS_SUSPENDED;
            wk->startTick = OS_GetTick();
            func_ov096_021885bc(vm, wk);
            if (wk->status & TRAI_STATUS_SUSPENDED) {
                return TRUE;
            }
        }
        wk->flagsRemaining >>= 1;
        wk->scriptIndex++;
        wk->moveIdx = 0;
    }

    if (wk->status & TRAI_STATUS_FLEE) {
        move = TRAI_MOVE_FLEE;
    } else {
        bestScores[0] = wk->scores[0];
        count = 1;
        bestMoves[0] = 0;
        for (i = 1; i < TRAI_MOVE_MAX; i++) {
            if (i < GetBattleMonMoveCount(wk->attacker)) {
                if (bestScores[0] == wk->scores[i]) {
                    bestScores[count] = wk->scores[i];
                    bestMoves[count++] = i;
                }
                if (bestScores[0] < wk->scores[i]) {
                    bestScores[0] = wk->scores[i];
                    count = 1;
                    bestMoves[0] = i;
                }
            }
        }
        move = bestMoves[GFL_RandomMTRange(count)];
    }
    wk->chosenMove = move;
    wk->chosenTarget = wk->defenderPos;
    return FALSE;
}

BOOL func_ov096_02188284(VM *vm, TrAIContext *wk) {
    s32 bestScores[TRAI_MOVE_MAX];
    u8 bestMoves[TRAI_MOVE_MAX];
    u8 targets[BTL_POS_MAX];
    s32 posCount;
    s32 count;
    s32 i;
    u8 pos;
    s16 best;
    u16 move;

    if (wk->battleStyle == BTL_STYLE_DOUBLE) {
        posCount = 4;
    } else {
        posCount = 6;
    }
    if (!(wk->status & TRAI_STATUS_SUSPENDED)) {
        wk->defenderPos = 0;
    }
    for (; wk->defenderPos < posCount; wk->defenderPos++) {
        wk->attacker = func_ov096_0218a2fc(wk, wk->attackerPos);
        wk->defender = func_ov096_0218a2fc(wk, wk->defenderPos);
        if (!(wk->status & TRAI_STATUS_SUSPENDED)) {
            if (wk->defender == NULL) {
                wk->bestMoves[wk->defenderPos] = -1;
                wk->bestScores[wk->defenderPos] = -1;
                continue;
            }
            if (wk->defenderPos == wk->attackerPos || IsFainted(wk->defender) == TRUE) {
                wk->bestMoves[wk->defenderPos] = -1;
                wk->bestScores[wk->defenderPos] = -1;
                continue;
            }
            func_ov096_0218a310(wk);
            func_ov096_0218a324(wk);
            wk->scriptIndex = 0;
            wk->moveIdx = 0;
            wk->flagsRemaining = wk->aiFlags;
        }
        while (wk->flagsRemaining != 0) {
            if (wk->flagsRemaining & 1) {
                if (!(wk->status & TRAI_STATUS_SUSPENDED)) {
                    wk->seq = TRAI_SEQ_LOAD_SCRIPT;
                }
                wk->status &= (u8)~TRAI_STATUS_SUSPENDED;
                wk->startTick = OS_GetTick();
                func_ov096_021885bc(vm, wk);
                if (wk->status & TRAI_STATUS_SUSPENDED) {
                    return TRUE;
                }
            }
            wk->flagsRemaining >>= 1;
            wk->scriptIndex++;
            wk->moveIdx = 0;
        }

        if (wk->status & TRAI_STATUS_FLEE) {
            wk->chosenMove = TRAI_MOVE_FLEE;
            continue;
        }
        bestScores[0] = wk->scores[0];
        count = 1;
        bestMoves[0] = 0;
        for (i = 1; i < TRAI_MOVE_MAX; i++) {
            if (i < GetBattleMonMoveCount(wk->attacker)) {
                if (bestScores[0] == wk->scores[i]) {
                    bestScores[count] = wk->scores[i];
                    bestMoves[count++] = i;
                }
                if (bestScores[0] < wk->scores[i]) {
                    bestScores[0] = wk->scores[i];
                    count = 1;
                    bestMoves[0] = i;
                }
            }
        }
        wk->bestMoves[wk->defenderPos] = bestMoves[GFL_RandomMTRange(count)];
        wk->bestScores[wk->defenderPos] = bestScores[0];
        if (!func_ov093_021b8ce8(wk->mainModule, func_ov093_021b8de4(wk->mainModule, wk->attackerPos),
                                 func_ov093_021b8de4(wk->mainModule, wk->defenderPos)) &&
            wk->bestScores[wk->defenderPos] < TRAI_SCORE_INITIAL) {
            wk->bestScores[wk->defenderPos] = -1;
        }
    }

    best = wk->bestScores[0];
    targets[0] = 0;
    count = 1;
    for (pos = 1; pos < posCount; pos++) {
        if (best == wk->bestScores[pos]) {
            targets[count++] = pos;
        }
        if (best < wk->bestScores[pos]) {
            best = wk->bestScores[pos];
            targets[0] = pos;
            count = 1;
        }
    }
    wk->chosenTarget = targets[GFL_RandomMTRange(count)];
    wk->chosenMove = wk->bestMoves[wk->chosenTarget];
    if (GetMoveData(wk, MoveGetID(wk->attacker, wk->chosenMove), 27) == 1 && !(wk->chosenTarget & 1)) {
        wk->chosenTarget = wk->attackerPos;
    }
    move = MoveGetID(wk->attacker, wk->chosenMove);
    if (wk->battleStyle == BTL_STYLE_TRIPLE && !func_ov093_021b9a6c(wk->attackerPos, wk->chosenTarget) &&
        !func_ov096_0218a384(wk, move, 11)) {
        wk->chosenTarget = (wk->chosenTarget & 1) + 2;
    }
    return FALSE;
}


// pokebw2's TrAI_RunScripts
void func_ov096_021885bc(VM *vm, TrAIContext *wk) {
    while (wk->seq != TRAI_SEQ_DONE && !(wk->status & TRAI_STATUS_SUSPENDED)) {
        switch (wk->seq) {
        case TRAI_SEQ_LOAD_SCRIPT:
            if (GetMovePP(wk->attacker, wk->moveIdx) == 0) {
                wk->moveId = 0;
            } else {
                wk->moveId = MoveGetID(wk->attacker, wk->moveIdx);
                wk->script = func_ov096_0218a494(wk);
                VM_LoadScript(vm, wk->script);
            }
            wk->seq++;
            // fallthrough
        case TRAI_SEQ_RUN_SCRIPT:
            // In a triple battle, skip moves that can't reach the defender
            if (wk->battleStyle == BTL_STYLE_TRIPLE && wk->moveId != 0 &&
                !func_ov093_021b9a6c(wk->attackerPos, wk->defenderPos) && !func_ov096_0218a384(wk, wk->moveId, 11)) {
                wk->moveId = 0;
                VM_Halt(vm);
                func_ov096_0218a528(wk);
                wk->script = NULL;
            }
            if (wk->moveId != 0) {
                VM_Run(vm);
                if (wk->status & TRAI_STATUS_MOVE_DONE) {
                    wk->status &= (u8)~TRAI_STATUS_SUSPENDED;
                    func_ov096_0218a528(wk);
                    wk->script = NULL;
                } else if (OS_GetTick() - wk->startTick > TRAI_TIME_LIMIT) {
                    wk->status |= TRAI_STATUS_SUSPENDED;
                    break;
                }
            } else {
                wk->scores[wk->moveIdx] = 0;
                wk->status |= TRAI_STATUS_MOVE_DONE;
            }
            if (wk->status & TRAI_STATUS_MOVE_DONE) {
                wk->moveIdx++;
                if (wk->moveIdx < GetBattleMonMoveCount(wk->attacker) && !(wk->status & TRAI_STATUS_SKIP_MOVES)) {
                    wk->seq = TRAI_SEQ_LOAD_SCRIPT;
                } else {
                    wk->seq++;
                }
                wk->status &= (u8)~TRAI_STATUS_MOVE_DONE;
            }
            break;
        case TRAI_SEQ_DONE:
        default:
            wk->seq = TRAI_SEQ_DONE;
            break;
        }
    }
}

BOOL AIGreaterThanRandom(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareToRandom(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

BOOL AILessThanRandom(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareToRandom(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

BOOL AIEqualToRandom(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareToRandom(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AINotEqualToRandom(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareToRandom(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CompareToRandom(VM *vm, TrAIContext *wk, u32 op) {
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, GFL_RandomMTRange(256), value, offset);
}

BOOL AIIncrementScore(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->scores[wk->moveIdx] += (s32)VM_Read32(vm);
    if (wk->scores[wk->moveIdx] < 0) {
        wk->scores[wk->moveIdx] = 0;
    }
    return wk->cmdReturn;
}

BOOL AIIsHPLessThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareHP(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

BOOL AIIsHPGreaterThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareHP(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

BOOL AIIsHPEqualTo(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareHP(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIsHPNotEqualTo(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareHP(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CompareHP(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, FX_Whole(GetHPRatio(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)))), value, offset);
}

BOOL AIHasStatus(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckStatusCondition(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

BOOL AIDoesNotHaveStatus(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckStatusCondition(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

void CheckStatusCondition(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, GetBattleMonStatus(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side))), 0, offset);
}

BOOL AIHasCondition(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021888bc(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIDoesNotHaveCondition(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021888bc(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckMonCondition
void func_ov096_021888bc(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    u32 condition = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, CheckCondition(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)), condition), TRUE,
                      offset);
}

BOOL AIIsBadlyPoisoned(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188924(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIsNotBadlyPoisoned(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188924(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckBadlyPoisoned
void func_ov096_02188924(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    BattleMon *mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side));
    BOOL poisoned = FALSE;
    BattleConditionCont cont;

    if (CheckCondition(mon, CONDITION_POISON)) {
        cont = GetConditionContinuationParam(mon, CONDITION_POISON);
        if (Condition_IsBadlyPoisoned(cont)) {
            poisoned = TRUE;
        }
    }
    AIConditionalJump(vm, op, poisoned, TRUE, offset);
}

BOOL AIHasConditionFlag(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021889a0(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIDoesNotHaveConditionFlag(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021889a0(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckConditionFlag
void func_ov096_021889a0(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    u32 flag = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, GetAdditionalConditionFlag(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)), flag),
                      TRUE, offset);
}

// pokebw2's AIIfSideEffect
BOOL func_ov096_021889e8(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188a08(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

// pokebw2's AIIfNotSideEffect
BOOL func_ov096_021889f8(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188a08(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckSideEffect
void func_ov096_02188a08(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    u32 sideEffect = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, func_ov093_021c82d0(wk->serverFlow, func_ov096_0218a160(wk, side), sideEffect) != 0, TRUE,
                      offset);
}

BOOL AIIfLessThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

BOOL AIIfGreaterThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

BOOL AIIfEqual(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfNotEqual(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

BOOL AIIfBit(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_AND);
    return wk->cmdReturn;
}

BOOL AIIfNotBit(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188ab4(vm, wk, AI_CMP_NAND);
    return wk->cmdReturn;
}

// pokebw2's CompareResult
void func_ov096_02188ab4(VM *vm, TrAIContext *wk, u32 op) {
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, wk->result, value, offset);
}

BOOL AIIfMove(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckMove(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfNotMove(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckMove(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CheckMove(VM *vm, TrAIContext *wk, u32 op) {
    s32 move = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, wk->moveId, move, offset);
}

// The list is 32-bit values, stored as 16-bit halves, and ends with 0xffffffff
// pokebw2's AIIfResultInList
BOOL func_ov096_02188b1c(VM *vm, void *work) {
    TrAIContext *wk = work;
    const u16 *list = (const u16 *)(vm->pc + (s32)VM_Read32(vm));
    s32 offset = VM_Read32(vm);
    s32 i = 0;
    u32 value;

    do {
        value = list[i] | (list[i + 1] << 16);
        if (wk->result == value) {
            VM_Jump(vm, vm->pc + offset);
            break;
        }
        i += 2;
    } while (value != 0xffffffff);
    return wk->cmdReturn;
}

// pokebw2's AIIfResultNotInList
BOOL func_ov096_02188b64(VM *vm, void *work) {
    TrAIContext *wk = work;
    const u16 *list = (const u16 *)(vm->pc + (s32)VM_Read32(vm));
    s32 offset = VM_Read32(vm);
    s32 i = 0;
    u32 value;

    do {
        value = list[i] | (list[i + 1] << 16);
        if (wk->result == value) {
            return wk->cmdReturn;
        }
        i += 2;
    } while (value != 0xffffffff);
    VM_Jump(vm, vm->pc + offset);
    return wk->cmdReturn;
}

BOOL AIHasDamagingMove(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 offset = VM_Read32(vm);
    s32 i;

    for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
        if (GetMoveData(wk, MoveGetID(wk->attacker, i), MOVE_PARAM_POWER) != 0) {
            break;
        }
    }
    if (i < GetBattleMonMoveCount(wk->attacker)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIDoesNotHaveDamagingMove(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 offset = VM_Read32(vm);
    s32 i;

    for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
        if (GetMoveData(wk, MoveGetID(wk->attacker, i), MOVE_PARAM_POWER) != 0) {
            break;
        }
    }
    if (i == GetBattleMonMoveCount(wk->attacker)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIGetTurnCount(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetTurnCounter(wk->serverFlow);
    return wk->cmdReturn;
}

BOOL AIGetType(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 which = VM_Read32(vm);
    PokeTypePair attackerTypes = GetPokeType(wk->attacker);
    PokeTypePair defenderTypes = GetPokeType(wk->defender);

    switch (which) {
    case LOAD_ATTACKER_TYPE_1:
        wk->result = PokeTypePair_GetType1(attackerTypes);
        break;
    case LOAD_DEFENDER_TYPE_1:
        wk->result = PokeTypePair_GetType1(defenderTypes);
        break;
    case LOAD_ATTACKER_TYPE_2:
        wk->result = PokeTypePair_GetType2(attackerTypes);
        break;
    case LOAD_DEFENDER_TYPE_2:
        wk->result = PokeTypePair_GetType2(defenderTypes);
        break;
    case LOAD_MOVE_TYPE:
        wk->result = GetMoveData(wk, wk->moveId, MOVE_PARAM_TYPE);
        break;
    case LOAD_ATTACKER_PARTNER_TYPE_1:
        wk->result = PokeTypePair_GetType1(
            GetPokeType(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, AI_BATTLER_ATTACKER_PARTNER))));
        break;
    case LOAD_DEFENDER_PARTNER_TYPE_1:
        wk->result = PokeTypePair_GetType1(
            GetPokeType(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, AI_BATTLER_DEFENDER_PARTNER))));
        break;
    case LOAD_ATTACKER_PARTNER_TYPE_2:
        wk->result = PokeTypePair_GetType2(
            GetPokeType(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, AI_BATTLER_ATTACKER_PARTNER))));
        break;
    case LOAD_DEFENDER_PARTNER_TYPE_2:
        wk->result = PokeTypePair_GetType2(
            GetPokeType(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, AI_BATTLER_DEFENDER_PARTNER))));
        break;
    }
    return wk->cmdReturn;
}

BOOL AIGetBasePower(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = PML_MoveGetBasePower(wk->moveId);
    return wk->cmdReturn;
}

// Sets the result to 0 if the move does no damage, 1 if another move does more damage, or 2 otherwise
BOOL AIGetHighestDamagingMove(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 damageRoll = VM_Read32(vm);
    u8 attackerId = GetMonID(wk->attacker);
    u8 defenderId = GetMonID(wk->defender);
    u32 damage = AICalcDamage(wk->serverFlow, attackerId, defenderId, wk->moveId, TRUE, damageRoll);
    s32 i;
    u16 move;

    if (damage == 0) {
        wk->result = 0;
    } else {
        wk->result = 2;
        for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
            move = MoveGetID(wk->attacker, i);
            if (i != wk->moveIdx &&
                AICalcDamage(wk->serverFlow, attackerId, defenderId, move, TRUE, damageRoll) > damage) {
                wk->result = 1;
                break;
            }
        }
    }
    return wk->cmdReturn;
}

BOOL AIGetPreviousMove(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetPreviousMoveID(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))));
    return wk->cmdReturn;
}

BOOL AICompareSpeed(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 op = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u16 attackerSpeed = func_ov093_021c81c8(wk->serverFlow, wk->attacker, TRUE);
    u16 defenderSpeed = func_ov093_021c81c8(wk->serverFlow, wk->defender, TRUE);

    switch (op) {
    case COMPARE_SPEED_FASTER:
        op = AI_CMP_GT;
        break;
    case COMPARE_SPEED_SLOWER:
        op = AI_CMP_LT;
        break;
    case COMPARE_SPEED_TIE:
        op = AI_CMP_EQ;
        break;
    }
    AIConditionalJump(vm, op, attackerSpeed, defenderSpeed, offset);
    return wk->cmdReturn;
}

BOOL AICheckTeamCount(VM *vm, void *work) {
    TrAIContext *wk = work;
    u8 clientId = func_ov093_021b8de4(wk->mainModule, func_ov096_0218a160(wk, VM_Read32(vm)));
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    int i = func_ov093_021b99b8(wk->mainModule, clientId);

    wk->result = 0;
    for (; i < GetNumMonsInParty(party); i++) {
        if (CanPokemonBattle(func_ov096_0218a308(party, i))) {
            wk->result++;
        }
    }
    return wk->cmdReturn;
}



BOOL AICheckMoveID(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = wk->moveId;
    return wk->cmdReturn;
}

BOOL AICheckMoveEffect(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetMoveData(wk, wk->moveId, MOVE_PARAM_EFFECT);
    return wk->cmdReturn;
}

BOOL AICheckAbility(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);

    wk->result = GuessAbility(wk, side, func_ov096_0218a160(wk, side));
    return wk->cmdReturn;
}

BOOL AINop43(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL AIIfTypeEffectiveness(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 attackerId = GetMonID(wk->attacker);

    AIConditionalJump(vm, AI_CMP_EQ,
                      func_ov093_021c7dac(wk->serverFlow, attackerId, GetMonID(wk->defender), wk->moveId), value,
                      offset);
    return wk->cmdReturn;
}

BOOL AIIfStatusInParty(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188fcc(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfStatusNotInParty(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02188fcc(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void func_ov096_02188fcc(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 clientId = func_ov093_021b8de4(wk->mainModule, func_ov096_0218a160(wk, side));
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    s32 i = func_ov093_021b99b8(wk->mainModule, clientId);
    BattleMon *mon;

    for (; i < GetNumMonsInParty(party); i++) {
        mon = func_ov096_0218a308(party, i);
        if (!IsFainted(mon) && AIConditionalJump(vm, op, GetBattleMonStatus(mon), 0, offset)) {
            break;
        }
    }
}


BOOL AIGetWeather(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = func_ov093_021ef884();
    return wk->cmdReturn;
}

BOOL AIHasMoveID(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareMoveID(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIDoesNotHaveMoveID(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareMoveID(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// Compares the move's effect, despite swan's name
void CompareMoveID(VM *vm, TrAIContext *wk, u32 op) {
    s32 effect = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, GetMoveData(wk, wk->moveId, MOVE_PARAM_EFFECT), effect, offset);
}

BOOL AIStatStageLessThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareStatStage(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

BOOL AIStatStageGreaterThan(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareStatStage(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

BOOL AIStatStageEqual(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareStatStage(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIStatStageNotEqual(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareStatStage(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CompareStatStage(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    u32 stat = VM_Read32(vm);
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, GetBattleMonStat(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)), stat), value,
                      offset);
}

BOOL AIIfCanFaint(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189170(vm, wk, AI_CMP_LE);
    return wk->cmdReturn;
}

BOOL AIIfCannotFaint(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189170(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

// pokebw2's CheckCanFaint
void func_ov096_02189170(VM *vm, TrAIContext *wk, u32 op) {
    s32 offset;
    u32 hp;
    u8 attackerId;

    VM_Read32(vm);
    offset = VM_Read32(vm);
    hp = GetBattleMonStat(wk->defender, BATTLEMON_HP);
    attackerId = GetMonID(wk->attacker);
    AIConditionalJump(
        vm, op, hp, AICalcDamage(wk->serverFlow, attackerId, GetMonID(wk->defender), wk->moveId, TRUE, USE_MIN_DAMAGE),
        offset);
}

BOOL AIIfHasMove(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021891f4(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfDoesNotHaveMove(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021891f4(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckHasMove
void func_ov096_021891f4(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    u16 move = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 pos = func_ov096_0218a160(wk, side);
    BOOL found = FALSE;
    BattleMon *mon;
    s32 i;
    u16 other;

    switch (side) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
            other = MoveGetID(wk->attacker, i);
            if (other == move) {
                found = TRUE;
                break;
            }
        }
        break;
    case AI_BATTLER_ATTACKER_PARTNER:
        mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, AI_BATTLER_ATTACKER_PARTNER));
        if (IsFainted(mon)) {
            break;
        }
        for (i = 0; i < GetBattleMonMoveCount(mon); i++) {
            other = MoveGetID(mon, i);
            if (other == move) {
                found = TRUE;
                break;
            }
        }
        break;
    case AI_BATTLER_DEFENDER:
        // Only the moves the defender was seen to use
        for (i = 0; i < TRAI_MOVE_MAX; i++) {
            if (move == wk->knownMoves[pos][i]) {
                found = TRUE;
                break;
            }
        }
        break;
    }
    AIConditionalJump(vm, op, found, TRUE, offset);
}

BOOL AIIfHasMoveWithEffect(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021892f8(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfDoesNotHaveMoveWithEffect(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_021892f8(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CheckHasMoveWithEffect
void func_ov096_021892f8(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 effect = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 pos = func_ov096_0218a160(wk, side);
    BOOL found = FALSE;
    s32 i;
    s32 moveEffect;

    switch (side) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
            moveEffect = GetMoveData(wk, MoveGetID(wk->attacker, i), MOVE_PARAM_EFFECT);
            if (moveEffect == effect) {
                found = TRUE;
                break;
            }
        }
        break;
    case AI_BATTLER_DEFENDER:
        for (i = 0; i < TRAI_MOVE_MAX; i++) {
            if (wk->knownMoves[pos][i] != 0) {
                moveEffect = GetMoveData(wk, wk->knownMoves[pos][i], MOVE_PARAM_EFFECT);
                if (moveEffect == effect) {
                    found = TRUE;
                    break;
                }
            }
        }
        break;
    }
    AIConditionalJump(vm, op, found, TRUE, offset);
}

BOOL AINop60(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

// pokebw2's AIFlee
BOOL func_ov096_021893b4(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->status |= TRAI_STATUS_FLEE;
    return wk->cmdReturn;
}

BOOL AINop62(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL AINop63(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL AIGetHeldItem(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetBattleMonHeldItem(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))));
    return wk->cmdReturn;
}

BOOL AIGetItemEffect(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = func_ov096_0218a3f4(wk, GetBattleMonHeldItem(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm)))), 1);
    return wk->cmdReturn;
}

BOOL AIGetGender(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetBattleMonStat(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))), BATTLEMON_GENDER);
    return wk->cmdReturn;
}

BOOL AIIsFakeOutActive(VM *vm, void *work) {
    TrAIContext *wk = work;
    BattleMon *mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm)));
    BOOL active = FALSE;

    if (GetAdditionalConditionFlag(mon, 0) == 0) {
        active = TRUE;
    }
    wk->result = active;
    return wk->cmdReturn;
}

BOOL AIGetStockpileCount(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetConditionCount(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))), 0);
    return wk->cmdReturn;
}

// pokebw2's AIGetBattleStyle
BOOL func_ov096_021894b4(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = wk->battleStyle;
    return wk->cmdReturn;
}

// pokebw2's AIGetBattleType
BOOL func_ov096_021894c4(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = wk->battleType;
    return wk->cmdReturn;
}

// pokebw2's AIGetConsumedItem
BOOL func_ov096_021894d4(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = func_ov093_021d6740(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))));
    return wk->cmdReturn;
}

BOOL AINop72(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

// Replaces the result, a move, with its power
BOOL AIGetMovePower(VM *vm, void *work) {
    TrAIContext *wk = work;

    if (wk->result != 0) {
        wk->result = GetMoveData(wk, wk->result, MOVE_PARAM_POWER);
    } else {
        wk->result = 0;
    }
    return wk->cmdReturn;
}

// Replaces the result, a move, with its effect
BOOL AIGetMoveID(VM *vm, void *work) {
    TrAIContext *wk = work;

    if (wk->result != 0) {
        wk->result = GetMoveData(wk, wk->result, MOVE_PARAM_EFFECT);
    } else {
        wk->result = -1;
    }
    return wk->cmdReturn;
}

BOOL AIGetProtectCount(VM *vm, void *work) {
    TrAIContext *wk = work;
    BattleMon *mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm)));

    if (GetPreviousMoveID(mon) != MOVE_PROTECT && GetPreviousMoveID(mon) != MOVE_DETECT &&
        GetPreviousMoveID(mon) != MOVE_ENDURE) {
        wk->result = 0;
    } else {
        wk->result = GetConsecutiveMoveCount(mon);
    }
    return wk->cmdReturn;
}

BOOL AIJump(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 offset = VM_Read32(vm);

    VM_Jump(vm, vm->pc + offset);
    return wk->cmdReturn;
}

BOOL AIEnd(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->status |= TRAI_STATUS_MOVE_DONE;
    VM_Halt(vm);
    return TRUE;
}

BOOL AICompareLevel(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 mode = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u32 ops[3] = { AI_CMP_GT, AI_CMP_LT, AI_CMP_EQ };
    u32 attackerLevel = GetBattleMonStat(wk->attacker, BATTLEMON_LEVEL);

    AIConditionalJump(vm, ops[mode], attackerLevel, GetBattleMonStat(wk->defender, BATTLEMON_LEVEL), offset);
    return wk->cmdReturn;
}

const VMInitParam data_ov096_0218a574 = {
    16, 8, data_ov096_0218a5a8, NELEMS(data_ov096_0218a5a8),
};

BOOL AIIfTaunted(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckTauntStatus(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIfNotTaunted(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckTauntStatus(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CheckTauntStatus(VM *vm, TrAIContext *wk, u32 op) {
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, CheckCondition(wk->defender, CONDITION_TAUNT), TRUE, offset);
}

BOOL AIIfTargetIsAlly(VM *vm, void *work) {
    TrAIContext *wk = work;

    AIConditionalJump(vm, AI_CMP_EQ, wk->attackerPos & 1, wk->defenderPos & 1, VM_Read32(vm));
    return wk->cmdReturn;
}

BOOL AIDoesMonHaveType(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u8 type = VM_Read32(vm);
    PokeTypePair types = GetPokeType(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)));

    if (type == PokeTypePair_GetType1(types) || type == PokeTypePair_GetType2(types)) {
        wk->result = TRUE;
    } else {
        wk->result = FALSE;
    }
    return wk->cmdReturn;
}

BOOL AIGuessAbility(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u32 ability = VM_Read32(vm);
    u32 guess = GuessAbility(wk, side, func_ov096_0218a160(wk, side));

    if (guess == ability) {
        wk->result = TRUE;
    } else {
        wk->result = FALSE;
    }
    return wk->cmdReturn;
}

BOOL AIIfFlashFireIsActive(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    if (GetAdditionalConditionFlag(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)), 13)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIIfHasItem(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u32 item = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    if (item == GetBattleMonHeldItem(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)))) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

// pokebw2's AIIfFieldEffect
BOOL func_ov096_0218979c(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 fieldEffect = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, AI_CMP_EQ, func_ov093_021ef948(fieldEffect), TRUE, offset);
    return wk->cmdReturn;
}

// pokebw2's AIGetSideEffect
BOOL func_ov096_021897cc(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u32 sideEffect = VM_Read32(vm);

    wk->result = func_ov093_021c82d0(wk->serverFlow, func_ov096_0218a160(wk, side), sideEffect);
    return wk->cmdReturn;
}

BOOL func_ov096_02189800(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 pos = func_ov096_0218a160(wk, side);
    u8 clientId = func_ov093_021b8de4(wk->mainModule, pos);
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    s32 i = func_ov093_021b99b8(wk->mainModule, clientId);

    for (; i < GetNumMonsInParty(party); i++) {
        if (!IsFainted(func_ov096_0218a308(party, i)) &&
            AIConditionalJump(vm, AI_CMP_LT, GetHPRatio(func_ov096_0218a2fc(wk, pos)), 100, offset)) {
            break;
        }
    }
    return wk->cmdReturn;
}

BOOL func_ov096_0218989c(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 pos = func_ov096_0218a160(wk, side);
    u8 clientId = func_ov093_021b8de4(wk->mainModule, pos);
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    s32 i = func_ov093_021b99b8(wk->mainModule, clientId);
    s32 j;

    for (; i < GetNumMonsInParty(party); i++) {
        if (IsFainted(func_ov096_0218a308(party, i))) {
            continue;
        }
        for (j = 0; j < GetBattleMonMoveCount(func_ov096_0218a2fc(wk, pos)); j++) {
            if (AIConditionalJump(vm, AI_CMP_NE, func_ov093_021d554c(func_ov096_0218a2fc(wk, pos), j), 0, offset)) {
                break;
            }
        }
        if (j != GetBattleMonMoveCount(func_ov096_0218a2fc(wk, pos))) {
            break;
        }
    }
    return wk->cmdReturn;
}



BOOL AIGetFlingPower(VM *vm, void *work) {
    TrAIContext *wk = work;
    u8 pos = func_ov096_0218a160(wk, VM_Read32(vm));

    wk->result = 0;
    if (!CheckCondition(func_ov096_0218a2fc(wk, pos), CONDITION_EMBARGO)) {
        wk->result = func_ov096_0218a3f4(wk, GetBattleMonHeldItem(func_ov096_0218a2fc(wk, pos)), ITEM_PARAM_FLING_POWER);
    }
    return wk->cmdReturn;
}

BOOL AIGetMovePP(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetMovePP(wk->attacker, wk->moveIdx);
    return wk->cmdReturn;
}

// pokebw2's AIIfCanUseLastResort
BOOL func_ov096_021899dc(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    BattleMon *mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side));
    u8 count = func_ov093_021d5478(mon);

    if (count >= GetBattleMonMoveCount(mon) && GetBattleMonMoveCount(mon) > 1) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIGetMoveCategory(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetMoveData(wk, wk->moveId, MOVE_PARAM_CATEGORY);
    return wk->cmdReturn;
}

BOOL AIGetLastMoveCategory(VM *vm, void *work) {
    TrAIContext *wk = work;
    u16 move = GetPreviousMoveID(wk->defender);

    if (move != 0) {
        wk->result = GetMoveData(wk, move, MOVE_PARAM_CATEGORY);
    } else {
        wk->result = 0;
    }
    return wk->cmdReturn;
}

BOOL AIGetOrderInTurn(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = func_ov093_021c81d0(wk->serverFlow, func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))), TRUE);
    return wk->cmdReturn;
}

// Loads the 16-bit value at 0x146 in the Pokemon at the position. Gen 4's AI has its command that loads the turns
// since the Pokemon switched in at this place in the command table
BOOL func_ov096_02189a9c(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = func_ov093_021d5ba4(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))));
    return wk->cmdReturn;
}

BOOL func_ov096_02189ac0(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 damageRoll = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 clientId = func_ov093_021b8de4(wk->mainModule, wk->attackerPos);
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    s32 i = func_ov093_021b99b8(wk->mainModule, clientId);
    u32 damage = func_ov096_0218a390(wk, wk->attacker, wk->defender, damageRoll);
    BattleMon *mon;

    for (; i < GetNumMonsInParty(party); i++) {
        mon = func_ov096_0218a308(party, i);
        GetMonID(mon);
        if (!IsFainted(mon) && func_ov096_0218a390(wk, mon, wk->defender, damageRoll) > damage) {
            VM_Jump(vm, vm->pc + offset);
            break;
        }
    }
    return wk->cmdReturn;
}


// pokebw2's AIIfHasSuperEffectiveMove
BOOL func_ov096_02189b78(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 offset = VM_Read32(vm);
    s32 i;
    u16 move;
    u8 attackerId;
    u32 effectiveness;

    for (i = 0; i < GetBattleMonMoveCount(wk->attacker); i++) {
        move = MoveGetID(wk->attacker, i);
        attackerId = GetMonID(wk->attacker);
        effectiveness = func_ov093_021c7dac(wk->serverFlow, attackerId, GetMonID(wk->defender), move);
        if (effectiveness == TYPE_EFFECTIVENESS_DOUBLE || effectiveness == TYPE_EFFECTIVENESS_QUADRUPLE) {
            VM_Jump(vm, vm->pc + offset);
            break;
        }
    }
    return wk->cmdReturn;
}

// Jumps if the last move of the Pokemon at the position deals more damage to the defender than the attacker's moves
// pokebw2's AIIfLastMoveDealsMoreDamage
BOOL func_ov096_02189bfc(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u32 damageRoll = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    BattleMon *mon = func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side));
    u32 damage = func_ov096_0218a390(wk, wk->attacker, wk->defender, damageRoll);
    u16 move = GetPreviousMoveID(mon);
    u8 monId = GetMonID(mon);

    if (damage < AICalcDamage(wk->serverFlow, monId, GetMonID(wk->defender), move, TRUE, damageRoll)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIGetPositiveStatStageTotal(VM *vm, void *work) {
    TrAIContext *wk = work;
    u8 pos = func_ov096_0218a160(wk, VM_Read32(vm));
    u32 stats[7] = { BATTLEMON_ATTACK_STAGE,     BATTLEMON_DEFENSE_STAGE, BATTLEMON_SP_ATTACK_STAGE,
                     BATTLEMON_SP_DEFENSE_STAGE, BATTLEMON_SPEED_STAGE,   BATTLEMON_ACCURACY_STAGE,
                     BATTLEMON_EVASION_STAGE };
    u32 i;
    s32 stage;

    wk->result = 0;
    for (i = 0; i < 7; i++) {
        stage = GetBattleMonStat(func_ov096_0218a2fc(wk, pos), stats[i]);
        if (stage > BATTLEMON_STAT_STAGE_NEUTRAL) {
            wk->result += stage - BATTLEMON_STAT_STAGE_NEUTRAL;
        }
    }
    return wk->cmdReturn;
}

BOOL AIGetStatStageDifference(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    u32 stat = VM_Read32(vm);

    wk->result = GetBattleMonStat(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)), stat) -
                 GetBattleMonStat(wk->attacker, stat);
    return wk->cmdReturn;
}

BOOL AINop102(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL AINop103(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL AINop104(VM *vm, void *work) {
    TrAIContext *wk = work;

    return wk->cmdReturn;
}

BOOL func_ov096_02189d48(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 damageRoll = VM_Read32(vm);
    u8 clientId = func_ov093_021b8de4(wk->mainModule, wk->attackerPos);
    BattleParty *party = func_ov093_021b9af8(wk->pokeCon, clientId);
    s32 i;
    s32 attackerIndex;
    u8 attackerId;
    u8 defenderId;
    u32 damage;
    s32 count;
    s32 j;
    BattleMon *mon;
    u8 monId;
    u16 move;

    count = func_ov093_021b99b8(wk->mainModule, clientId);
    attackerIndex = FindPartyMon(party, wk->attacker);
    attackerId = GetMonID(wk->attacker);
    defenderId = GetMonID(wk->defender);
    damage = AICalcDamage(wk->serverFlow, attackerId, defenderId, wk->moveId, TRUE, damageRoll);

    if (attackerIndex < 0 || damage == 0) {
        wk->result = 0;
        return wk->cmdReturn;
    }
    for (i = 0; i < count; i++) {
        mon = func_ov096_0218a308(party, i);
        monId = GetMonID(mon);
        if (CanPokemonBattle(mon) && i != attackerIndex) {
            wk->result = 2;
            for (j = 0; j < GetBattleMonMoveCount(mon); j++) {
                move = MoveGetID(mon, j);
                if (j != wk->moveIdx && AICalcDamage(wk->serverFlow, monId, defenderId, move, TRUE, damageRoll) > damage) {
                    wk->result = 1;
                    break;
                }
            }
        }
    }
    return wk->cmdReturn;
}


BOOL AIIsFainted(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckIfFainted(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

BOOL AIIsNotFainted(VM *vm, void *work) {
    TrAIContext *wk = work;

    CheckIfFainted(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

void CheckIfFainted(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, IsFainted(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side))), TRUE, offset);
}

BOOL AIGetAbility(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetBattleMonStat(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))), 17);
    return wk->cmdReturn;
}

BOOL AIIfSubstitute(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    if (IsSubstituteActive(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, side)))) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIGetSpecies(VM *vm, void *work) {
    TrAIContext *wk = work;

    wk->result = GetBattleMonSpecies(func_ov096_0218a2fc(wk, func_ov096_0218a160(wk, VM_Read32(vm))));
    return wk->cmdReturn;
}

// pokebw2's AIGreaterThanTurnRandom
BOOL func_ov096_02189f58(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189f98(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

// pokebw2's AILessThanTurnRandom
BOOL func_ov096_02189f68(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189f98(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

// pokebw2's AIEqualToTurnRandom
BOOL func_ov096_02189f78(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189f98(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

// pokebw2's AINotEqualToTurnRandom
BOOL func_ov096_02189f88(VM *vm, void *work) {
    TrAIContext *wk = work;

    func_ov096_02189f98(vm, wk, AI_CMP_NE);
    return wk->cmdReturn;
}

// pokebw2's CompareToTurnRandom
void func_ov096_02189f98(VM *vm, TrAIContext *wk, u32 op) {
    s32 value = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    AIConditionalJump(vm, op, wk->random, value, offset);
}

// Jumps to the entry for the move's effect in a table of offsets, or ends the script if the table has no entry.
// Modes other than 0 end the script
// pokebw2's AIJumpByMoveEffect
BOOL func_ov096_02189fc0(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 mode = VM_Read32(vm);
    s32 max = VM_Read32(vm);
    const u16 *table = (const u16 *)(vm->pc + (s32)VM_Read32(vm));
    s32 index;
    BOOL end = FALSE;

    if (mode == 0) {
        index = GetMoveData(wk, wk->moveId, MOVE_PARAM_EFFECT) * 2;
        if (index / 2 > max) {
            end = TRUE;
        }
    } else {
        end = TRUE;
    }
    if (end == TRUE) {
        return AIEnd(vm, wk);
    }
    VM_Jump(vm, (const u8 *)table + (table[index] | (table[index + 1] << 16)));
    return wk->cmdReturn;
}

// Jumps if overlay 169's check for position effect 3 at the position returns TRUE
BOOL func_ov096_0218a034(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);

    if (func_ov093_021c82f4(wk->serverFlow, func_ov096_0218a160(wk, side), 3)) {
        VM_Jump(vm, vm->pc + offset);
    }
    return wk->cmdReturn;
}

BOOL AIIsAtkLessThanSpAtk(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareOffensiveStats(vm, wk, AI_CMP_LT);
    return wk->cmdReturn;
}

BOOL AIIsAtkGreaterThanSpAtk(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareOffensiveStats(vm, wk, AI_CMP_GT);
    return wk->cmdReturn;
}

BOOL AIIsAtkEqualToSpAtk(VM *vm, void *work) {
    TrAIContext *wk = work;

    CompareOffensiveStats(vm, wk, AI_CMP_EQ);
    return wk->cmdReturn;
}

// The original compares the position with the special attack, and discards the attack
void CompareOffensiveStats(VM *vm, TrAIContext *wk, u32 op) {
    u32 side = VM_Read32(vm);
    s32 offset = VM_Read32(vm);
    u8 pos = func_ov096_0218a160(wk, side);

    GetBattleMonStat(func_ov096_0218a2fc(wk, pos), BATTLEMON_ATTACK);
    AIConditionalJump(vm, op, pos, GetBattleMonStat(func_ov096_0218a2fc(wk, pos), BATTLEMON_SP_ATTACK), offset);
}

BOOL AIConditionalJump(VM *vm, u32 op, s32 value, s32 ref, s32 offset) {
    BOOL jump = FALSE;

    switch (op) {
    case AI_CMP_LT:
        if (value < ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_GT:
        if (value > ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_EQ:
        if (value == ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_NE:
        if (value != ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_AND:
        if (value & ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_NAND:
        if (!(value & ref)) {
            jump = TRUE;
        }
        break;
    case AI_CMP_LE:
        if (value <= ref) {
            jump = TRUE;
        }
        break;
    case AI_CMP_GE:
        if (value >= ref) {
            jump = TRUE;
        }
        break;
    }
    if (jump == TRUE) {
        VM_Jump(vm, vm->pc + offset);
    }
    return jump;
}

// pokebw2's TrAI_GetTargetPos
u8 func_ov096_0218a160(TrAIContext *wk, u32 side) {
    u8 pos;
    u32 slot;

    switch (side) {
    case AI_BATTLER_ATTACKER:
        pos = wk->attackerPos;
        break;
    case AI_BATTLER_DEFENDER:
    default:
        pos = wk->defenderPos;
        break;
    case AI_BATTLER_ATTACKER_PARTNER:
        switch (wk->battleStyle) {
        case BTL_STYLE_SINGLE:
        case BTL_STYLE_ROTATION:
            pos = wk->attackerPos;
            break;
        case BTL_STYLE_DOUBLE:
            pos = wk->attackerPos ^ 2;
            break;
        case BTL_STYLE_TRIPLE:
            slot = wk->attackerPos / 2;
            if (slot != 2) {
                slot++;
            } else {
                slot = 1;
            }
            pos = func_ov093_021b8c38(wk->attackerPos, slot);
            break;
        }
        break;
    case AI_BATTLER_DEFENDER_PARTNER:
        switch (wk->battleStyle) {
        case BTL_STYLE_SINGLE:
        case BTL_STYLE_ROTATION:
            pos = wk->defenderPos;
            break;
        case BTL_STYLE_DOUBLE:
            pos = wk->defenderPos ^ 2;
            break;
        case BTL_STYLE_TRIPLE:
            slot = wk->defenderPos / 2;
            if (slot != 2) {
                slot++;
            } else {
                slot = 1;
            }
            pos = func_ov093_021b8c38(wk->defenderPos, slot);
            break;
        }
        break;
    }
    return pos;
}

// The AI knows the abilities of its own side. For the other side, it knows the abilities that were shown, and the
// trapping abilities, and otherwise guesses one of the species' abilities
u32 GuessAbility(TrAIContext *wk, u32 side, u8 pos) {
    BattleMon *mon = func_ov096_0218a2fc(wk, pos);
    u32 abilities[3];
    s32 count;
    u32 ability;
    u32 species;
    u32 form;
    u32 ability1;
    u32 ability2;
    u32 abilityHidden;

    if (CheckCondition(mon, CONDITION_GASTRO_ACID)) {
        return 0;
    }
    if (side == AI_BATTLER_DEFENDER || side == AI_BATTLER_DEFENDER_PARTNER) {
        wk->knownAbilities[pos] = func_ov094_021f8afc(pos);
        if (wk->knownAbilities[pos] != 0) {
            return wk->knownAbilities[pos];
        }
        ability = GetBattleMonStat(mon, BATTLEMON_ABILITY);
        if (ability == ABILITY_SHADOW_TAG || ability == ABILITY_MAGNET_PULL || ability == ABILITY_ARENA_TRAP) {
            return ability;
        }
        species = GetBattleMonSpecies(mon);
        form = GetBattleMonStat(mon, BATTLEMON_FORM);
        count = 0;
        ability1 = PML_PersonalGetParamSingle(species, form, PERSONAL_ABILITY_1);
        ability2 = PML_PersonalGetParamSingle(species, form, PERSONAL_ABILITY_2);
        abilityHidden = PML_PersonalGetParamSingle(species, form, PERSONAL_ABILITY_HIDDEN);
        if (ability1 != 0) {
            abilities[count++] = ability1;
        }
        if (ability2 != 0) {
            abilities[count++] = ability2;
        }
        if (abilityHidden != 0) {
            abilities[count++] = abilityHidden;
        }
        return abilities[GFL_RandomMTRange(count)];
    }
    return GetBattleMonStat(mon, BATTLEMON_ABILITY);
}

BattleMon *func_ov096_0218a2fc(TrAIContext *wk, u8 pos) {
    return func_ov093_021b98ac(wk->pokeCon, pos);
}


// pokebw2's TrAI_GetPartyMon
BattleMon *func_ov096_0218a308(BattleParty *party, u8 index) {
    return GetBattleMonFromParty(party, index);
}

// pokebw2's TrAI_ResetScores
void func_ov096_0218a310(TrAIContext *wk) {
    s32 i;

    for (i = 0; i < TRAI_MOVE_MAX; i++) {
        wk->scores[i] = wk->initialScores[i];
    }
}

// pokebw2's TrAI_RememberDefenderMove
void func_ov096_0218a324(TrAIContext *wk) {
    BattleMon *defender = func_ov096_0218a2fc(wk, wk->defenderPos);
    s32 i;
    u16 move;

    for (i = 0; i < TRAI_MOVE_MAX; i++) {
        move = wk->knownMoves[wk->defenderPos][i];
        if (move == GetPreviousMoveID(defender)) {
            break;
        }
        if (move == 0) {
            wk->knownMoves[wk->defenderPos][i] = GetPreviousMoveID(defender);
            break;
        }
    }
}

s32 GetMoveData(TrAIContext *wk, u16 move, u32 param) {
    return PML_MoveGetParam(move, param);
}

// pokebw2's GetMoveFlag
BOOL func_ov096_0218a384(TrAIContext *wk, u16 move, u32 flag) {
    return getMoveFlag(move, flag);
}

// pokebw2's TrAI_GetMaxDamage
u32 func_ov096_0218a390(TrAIContext *wk, BattleMon *attacker, BattleMon *defender, u32 damageRoll) {
    u32 max = 0;
    u8 attackerId = GetMonID(attacker);
    u8 defenderId = GetMonID(defender);
    s32 i;
    u32 damage;

    for (i = 0; i < GetBattleMonMoveCount(attacker); i++) {
        damage = AICalcDamage(wk->serverFlow, attackerId, defenderId, MoveGetID(attacker, i), TRUE, damageRoll);
        if (damage > max) {
            max = damage;
        }
    }
    return max;
}

// pokebw2's GetItemData
s32 func_ov096_0218a3f4(TrAIContext *wk, u16 item, u32 param) {
    void *data = PML_ItemArcHandleReadFile(wk->itemArc, item, wk->heapId);
    s32 value = PML_ItemGetParam(data, param);

    GFL_HeapFree(data);
    return value;
}

// pokebw2's TrAI_CreateScriptCache
void func_ov096_0218a41c(HeapID heapId) {
    s32 i;

    if (data_ov096_0218a7c0 == NULL) {
        data_ov096_0218a7c0 = GFL_HeapAllocate(heapId, sizeof(TrAIScriptCache), TRUE, "tr_ai.c", 3999);
        for (i = 0; i < TRAI_SCRIPT_CACHE_SIZE; i++) {
            data_ov096_0218a7c0->fileIds[i] = TRAI_SCRIPT_NONE;
        }
    }
}

// pokebw2's TrAI_FreeScriptCache
void func_ov096_0218a458(void) {
    s32 i;

    if (data_ov096_0218a7c0 != NULL) {
        for (i = 0; i < TRAI_SCRIPT_CACHE_SIZE; i++) {
            if (data_ov096_0218a7c0->scripts[i] != NULL) {
                GFL_HeapFree(data_ov096_0218a7c0->scripts[i]);
                data_ov096_0218a7c0->scripts[i] = NULL;
            }
        }
        GFL_HeapFree(data_ov096_0218a7c0);
        data_ov096_0218a7c0 = NULL;
    }
}

// pokebw2's TrAI_LoadScript
void *func_ov096_0218a494(TrAIContext *wk) {
    s32 i;
    u32 heapId;

    for (i = 0; i < TRAI_SCRIPT_CACHE_SIZE; i++) {
        if (wk->scriptIndex == data_ov096_0218a7c0->fileIds[i]) {
            data_ov096_0218a7c0->refCounts[i]++;
            return data_ov096_0218a7c0->scripts[i];
        }
    }
    for (i = 0; i < TRAI_SCRIPT_CACHE_SIZE; i++) {
        if (data_ov096_0218a7c0->fileIds[i] == TRAI_SCRIPT_NONE) {
            break;
        }
    }
    if (wk->scriptIndex == 2) {
        heapId = HEAPID_TAIL(wk->heapId);
    } else {
        heapId = wk->heapId;
    }
    data_ov096_0218a7c0->fileIds[i] = wk->scriptIndex;
    data_ov096_0218a7c0->refCounts[i]++;
    data_ov096_0218a7c0->scripts[i] = GFL_ArcToolReadHeapNew(wk->scriptArc, wk->scriptIndex, heapId);
    return data_ov096_0218a7c0->scripts[i];
}

// pokebw2's TrAI_ReleaseScript
void func_ov096_0218a528(TrAIContext *wk) {
    s32 i;

    for (i = 0; i < TRAI_SCRIPT_CACHE_SIZE; i++) {
        if (wk->scriptIndex == data_ov096_0218a7c0->fileIds[i]) {
            data_ov096_0218a7c0->refCounts[i]--;
            if (data_ov096_0218a7c0->refCounts[i] == 0) {
                data_ov096_0218a7c0->fileIds[i] = TRAI_SCRIPT_NONE;
                GFL_HeapFree(data_ov096_0218a7c0->scripts[i]);
                data_ov096_0218a7c0->scripts[i] = NULL;
            }
            break;
        }
    }
}
