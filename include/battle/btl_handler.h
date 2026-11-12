// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_handler.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_HANDLER_H
#define BWPORT_BATTLE_BTL_HANDLER_H

#include "types.h"
#include "struct_decls.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_action_order.h"
#include "battle/btl_calc.h"
#include "battle/btl_faintrec.h"
#include "battle/btl_handler_work.h"
#include "battle/btl_monset.h"
#include "battle/btl_pospoke.h"
#include "battle/btl_wazarec.h"

BOOL BattleHandler_StrIsEnabled(const BattleHandlerString *string);
void BattleHandler_StrSetup(BattleHandlerString *string, u32 enabled, u16 message);
void BattleHandler_PushRun(BtlServerFlow *flow, u32 command, u32 monId);
u8 HandlerGetAlivePartyCount(BtlServerFlow *handler, u16 code, u8 *monIds);

// Black 2's; twinned code confirms the offsets of command, monId, size, unk23, checkPrevResult and unk26 in Black
// The start of every handler command's parameters
typedef struct {
    u32 command : 8;
    u32 monId : 5;
    // Rounded up to 4 bytes
    u32 size : 10;
    u32 unk23 : 1;
    // Skips the command when the previous one failed
    u32 checkPrevResult : 1;
    // Skips the command when the mon has fainted
    u32 checkFainted : 1;
    u32 unk26 : 1;
    u32 unk27 : 5;
} BattleHandlerHeader;

// Black 2's; twinned code confirms the offsets of message, enabled, count and args in Black
struct BattleHandlerString {
    u16 message;
    union {
        u16 flags;
        struct {
            u16 enabled : 8;
            u16 count : 7;
            u16 hasSound : 1;
        };
    };
    union {
        u32 args[8];
        // Black's AddSoundEffect keeps the sound in the last argument's slot
        struct {
            u32 unused_args[7];
            u32 soundEffect;
        };
    };
};

// Black 2's; twinned code confirms the offsets of monIndex, popup, checkPrevResult and string in Black
struct BattleHandlerMessageParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 checkPrevResult : 1;
    u32 unk25 : 7;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of popup, ability, targetIndex, unk08 and string in Black
struct BattleHandlerAbilityChangeParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u16 ability;
    u8 targetIndex;
    u8 force;
    u8 unk08;
    u8 unk09[3];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, popup, condition, value, showFail, noMessage,
// skipItemReaction, targetIndex, overwrite and string in Black
struct BattleHandlerAddConditionParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u32 condition;
    BattleCondition value;
    u8 showFail;
    // Shows no message of its own when there's no string
    u8 noMessage;
    u8 skipItemReaction;
    u8 targetIndex;
    u8 overwrite;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of popup, monIndex, form and string in Black
struct BattleHandlerChangeFormParam {
    u32 unk00 : 23;
    u32 popup : 1;
    u32 unk18 : 8;
    u8 monIndex;
    u8 form;
    u8 unk06[2];
    BattleHandlerString string;
};

struct BattleHandlerChangeHPParam {
    BattleHandlerHeader header;
    u8 count;
    u8 suppress;
    u8 skipReaction;
    u8 monIds[9];
    u32 hpChanges[6];
};

// Black 2's; twinned code confirms the offsets of type, monIndex and suppressMessage in Black
struct BattleHandlerChangeTypeParam {
    u32 unk00;
    u16 type;
    u8 monIndex;
    u8 suppressMessage;
};

// Black 2's; twinned code confirms the offsets of popup, weather, duration, notifyAirLock and string in Black
struct BattleHandlerChangeWeatherParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u8 weather;
    u8 duration;
    u8 notifyAirLock;
    u8 unk07;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of header, monIndex and reaction in Black
struct BattleHandlerCheckHeldItemParam {
    BattleHandlerHeader header;
    u8 monIndex;
    u8 unk05[3];
    u32 reaction;
};

// Black 2's; twinned code confirms the offsets of popup, checkPrevResult, unk25, condition, monIds, count, useString
// and string in Black
struct BattleHandlerCureConditionParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 checkPrevResult : 1;
    u32 unk25 : 1;
    u32 unk26 : 6;
    u32 condition;
    u8 monIds[12];
    u8 count;
    u8 useString;
    u8 unk16[2];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, popup, checkPrevResult, amount, targetIndex, skipCheck and
// string in Black
struct BattleHandlerRecoverHPParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 checkPrevResult : 1;
    u32 unk25 : 7;
    u16 amount;
    u8 targetIndex;
    // Recovers without the checks for Heal Block and the like
    u8 skipCheck;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of sourceIndex, popup, amount, targetIndex, checkSemi, showViewEffect,
// effect, effectArg1, effectArg2 and string in Black
struct BattleHandlerDamageParam {
    u32 unk00 : 8;
    u32 sourceIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u16 amount;
    u8 targetIndex;
    u8 checkSemi : 1;
    u8 showViewEffect : 1;
    u8 unkFlags : 6;
    u16 effect;
    u8 effectArg1;
    u8 effectArg2;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of flag and monIndex in Black
struct BattleHandlerFlagParam {
    BattleHandlerHeader header;
    u32 flag;
    u8 monIndex;
};

// Black 2's; twinned code confirms the offsets of monIndex and string in Black
struct BattleHandlerIllusionBreakParam {
    u32 unk00;
    u8 monIndex;
    u8 unk05[3];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of popup, item, targetIndex, clearConsumed, clearOtherConsumed,
// otherIndex and string in Black
struct BattleHandlerSetItemParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u16 item;
    u8 targetIndex;
    u8 clearConsumed;
    u8 clearOtherConsumed;
    u8 otherIndex;
    u8 unk0a[2];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of popup, stat, value, change, flag, unk0e, count, monIds and string in
// Black
struct BattleHandlerStatChangeParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u32 stat;
    u32 value;
    s8 change;
    u8 flag;
    u8 unk0e;
    u8 count;
    u8 monIds[8];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of popup, otherIndex and firstString in Black
struct BattleHandlerSwapItemParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u8 otherIndex;
    u8 unk05[3];
    BattleHandlerString firstString;
    BattleHandlerString secondString;
    BattleHandlerString thirdString;
};

// Black 2's; twinned code confirms the offsets of popup, targetIndex and string in Black
struct BattleHandlerTransformParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 10;
    u32 popup : 1;
    u32 unk24 : 8;
    u8 targetIndex;
    u8 unk05[3];
    BattleHandlerString string;
};

u8 func_ov093_021c7d00(BtlServerFlow *flow, u8 monId);

struct BattleHandlerAddSideEffectParam {
    BattleHandlerHeader header;
    u32 effect;
    BattleCondition cont;
    u8 side;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of effects and side in Black
struct BattleHandlerRemoveSideEffectParam {
    BattleHandlerHeader header;
    u8 effects[3];
    u8 side;
};

struct BattleHandlerPosEffectParam {
    BattleHandlerHeader header;
    u32 effect;
    u8 pos;
    u32 args[4];
    u8 argCount;
};

// Black 2's; twinned code confirms the offsets of monIndex, slot, maxPP, updateCurrent and move in Black
struct BattleHandlerUpdateMoveParam {
    BattleHandlerHeader header;
    u8 monIndex;
    u8 slot;
    u8 maxPP;
    u8 updateCurrent;
    u16 move;
};

struct BattleHandlerForceSwitchParam {
    BattleHandlerHeader header;
    u16 effect;
    u8 targetIndex;
    u8 forced : 4;
    u8 ignoreLevel : 4;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of effect, pos1, pos2, reserve, reserved, hideMessageWindow and string
// in Black
struct BattleHandlerEffectAtPosParam {
    BattleHandlerHeader header;
    u16 effect;
    u8 pos1;
    u8 pos2;
    u16 reserve;
    u8 reserved;
    u8 hideMessageWindow;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, attack, defense, spAttack, spDefense, speed, accuracy and
// evasion in Black
struct BattleHandlerSetStatStageParam {
    BattleHandlerHeader header;
    u8 monIndex;
    u8 attack;
    u8 defense;
    u8 spAttack;
    u8 spDefense;
    u8 speed;
    u8 accuracy;
    u8 evasion;
};

// Black 2's; twinned code confirms the offsets of attack, defense, spAttack, spDefense, speed, monIndex, setAttack,
// setDefense, setSpAttack, setSpDefense, setSpeed and string in Black
// The stats it sets, as with Power Split and Guard Split
struct BattleHandlerSetStatusParam {
    BattleHandlerHeader header;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
    u8 monIndex;
    u8 setAttack : 1;
    u8 setDefense : 1;
    u8 setSpAttack : 1;
    u8 setSpDefense : 1;
    u8 setSpeed : 1;
    u8 unk0f_5 : 3;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of effect, value, duration and string in Black
struct BattleHandlerAddFieldEffectParam {
    u32 unk00;
    u32 effect;
    BattleCondition value;
    u8 duration;
    u8 unk0d[3];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, skipDisplay and string in Black
struct BattleHandlerConsumeItemParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 19;
    u32 skipDisplay;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of header, amount, monIndex, moveIndex, currentMoves, allowFainted and
// string in Black
struct BattleHandlerPPParam {
    BattleHandlerHeader header;
    u8 amount;
    u8 monIndex;
    u8 moveIndex;
    // Changes the PP of the moves the mon has now rather than its original ones, as after Transform
    u8 currentMoves : 1;
    u8 allowFainted : 1;
    u8 unk09 : 6;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, force and string in Black
struct BattleHandlerFaintParam {
    u32 unk00;
    u8 monIndex;
    u8 force;
    u8 unk06[2];
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex and flag in Black
struct BattleHandlerFlinchParam {
    u32 unk00;
    u8 monIndex;
    u8 flag;
};

// Black 2's; twinned code confirms the offsets of checkPrevResult, monIndex and item in Black
struct BattleHandlerForceUseItemParam {
    u32 unk00 : 8;
    u32 monIndex2 : 5;
    u32 unk13 : 11;
    u32 checkPrevResult : 1;
    u32 unk25 : 7;
    u8 monIndex;
    u8 unk05;
    u16 item;
};

// Black 2's; twinned code confirms the offsets of monIndex, flag and string in Black
struct BattleHandlerHideTurnParam {
    u32 unk00;
    u8 monIndex;
    u8 unk05[3];
    u32 flag;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monId, moveId and string in Black
struct BattleHandlerInterruptParam {
    u32 unk00;
    union {
        u8 monId;
        u16 moveId;
    };
    u16 unk06;
    BattleHandlerString string;
};

struct BattleHandlerMoveEffectParam {
    BattleHandlerHeader header;
    u8 index;
};

// Black 2's; twinned code confirms the offsets of effect in Black
struct BattleHandlerRemoveFieldEffectParam {
    u32 unk00;
    u32 effect;
};

// Black 2's; twinned code confirms the offsets of count and monIndices in Black
struct BattleHandlerResetStatStageParam {
    u32 unk00;
    u8 count;
    u8 monIndices[6];
};

struct BattleHandlerSetCounterParam {
    BattleHandlerHeader header;
    u8 monIndex;
    u8 counter;
    u8 value;
};

// Black 2's; twinned code confirms the offsets of monIndex, weight and string in Black
struct BattleHandlerSetWeightParam {
    u32 unk00;
    u8 monIndex;
    u8 unk05;
    u16 weight;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of firstMonIndex, secondMonIndex and string in Black
struct BattleHandlerSwapPokeParam {
    u32 unk00;
    u8 firstMonIndex;
    u8 secondMonIndex;
    u8 unk06[2];
    BattleHandlerString string;
};

// Black 2's, not checked against Black
struct BattleHandlerSwitchParam {
    BattleHandlerHeader header;
    BattleHandlerString firstString;
    BattleHandlerString secondString;
    u8 monIndex;
    u8 flag;
};

// Black 2's; twinned code confirms the offsets of monId in Black
struct BattleHandlerPopupParam {
    u32 unk00 : 8;
    u32 monId : 5;
    u32 unk13 : 19;
};

// Black 2's; twinned code confirms the offsets of move, type, targetType and flags in Black
// A move's parameters as the events changed them
struct BtlFlowMoveParam {
    u16 move;
    u16 originalMove;
    PokeTypePair userType;
    u8 type;
    u8 unk07;
    u32 category;
    // How the move picks its targets, the move data's target type
    u32 targetType;
    union {
        u32 raw;
        struct {
            u32 unk0 : 1;
            u32 unk1 : 31;
        };
    } flags;
};

// Black 2's, not checked against Black
// One target's result from the damage calculation of a move
struct BtlFlowDamageEntry {
    u16 damage;
    u16 monId : 5;
    u16 effectiveness : 4;
    u16 unk9 : 4;
    u16 critical : 1;
    u16 fixedDamage : 1;
    u16 substitute : 1;
};

// Black 2's, not checked against Black
struct BtlFlowDamageList {
    u8 count;
    u8 substituteCount;
    u8 total;
    u8 unk03;
    BtlFlowDamageEntry entries[6];
};

// Black 2's; twinned code confirms the offsets of unk00, pos1, pos2, index, enabled and unk05_1 in Black
// Where the move goes, and its effect
struct BattleMoveEffectState {
    u16 unk00;
    u8 pos1;
    u8 pos2;
    u8 index;
    u8 enabled : 1;
    u8 unk05_1 : 1;
    u8 unk05_2 : 6;
};

// Black 2's; twinned code confirms the offsets of count, unk02 and unk05 in Black
// How many times a move hits, and how its hits are checked
struct BtlFlowHitWork {
    u8 count;
    u8 unk01;
    u8 unk02;
    u8 unk03;
    u8 unk04;
    u8 unk05;
};

// Black 2's; twinned code confirms the offsets of hp, attack, defense, speed, spAttack and spDefense in Black
// The experience one party mon gets for a faint
struct BtlFlowExpEntry {
    u32 exp;
    // Set when a traded mon or a Lucky Egg raised it
    u8 boosted;
    // The effort values it gained
    u8 hp;
    u8 attack;
    u8 defense;
    u8 speed;
    u8 spAttack;
    u8 spDefense;
    u8 unk0B;
};

// Black 2's, not checked against Black
struct BtlFlowUnk1B50 {
    u8 unk000[0x220];
    u32 unk220;
};

// Mons that react to a move, with their targets
struct BtlFlowReactionList {
    u8 count;
    u8 monIds[6];
    u8 targets[6];
    u8 unk0D[6];
};

// Black 2's; twinned code confirms the offsets of count and clientIds in Black
// Client IDs the flow collected, such as those that must switch in
struct BtlFlowClientList {
    u8 count;
    u8 clientIds[4];
    u8 unk05;
};

// One level of the flow's work, kept for each level of nested handler calls; the flow's pointers point into the
// current one
struct BtlFlowWorkFrame {
    // 0x48 bytes each on the DS; typed, as their pointers make them bigger on a PC target (as bytes, each set's
    // tail overlapped the next set there, and clearing one emptied the one before: every move failed)
    BtlMonSet monSets[7];
    BattleMoveEffectState moveEffect;
    BtlFlowMoveParam moveParams[2];
    BtlFlowHitWork hitWork;
    BtlFlowReactionList reactionLists[2];
    BtlFlowDamageList damageLists[2];
    u8 unk28C;
    u8 unk28D;
    u8 unk28E;
};

// Black 2's; twinned code confirms the offsets of server, mainModule, pokeCon, queue, unk10, unk14, unk18, unk3E0,
// unk4A4, moveEffect, unk4AC, unk4B0, unk4B4, unk4CE, unk774, unk778, unk77C, unk77D, unk77E, unk77F, actionOrderCount,
// unk783, unk784, unk785, unk786, interruptCount, unk789, unk78A_0, unk78A_3, unk78A_4, unk78A_5, interruptMonIds,
// unk791, unk7A9, unk7C1, unk7D9, actionOrder, tempEntry, unk850, unk854, unk858, unk85C, unk860, unk864, unk868,
// unk86C, unk870, frames, frameDepth, unk1AB0, unk1AB4, unk1ab8 and message in Black; the fields after message are
// laid out from Black's code (expEntries at 0x1b08, actionState at 0x1d74, sizeof 0x212c)
struct BtlServerFlow {
    BtlServer *server;
    BtlMainModule *mainModule;
    BtlPokeCon *pokeCon;
    BtlServerCmdQueue *queue;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    BtlWazaRec wazaRec;
    BtlFaintRec faintRec;
    ArcTool *unk4A4;
    BattleMoveEffectState *moveEffect;
    BtlFlowReactionList *unk4AC;
    BtlFlowReactionList *unk4B0;
    BtlFlowHitWork *unk4B4;
    BtlClientIDList clientIdList;
    BattleMonLevelUp levelUp;
    BtlFlowClientList unk4CE;
    u8 unk4D4[0x2a0];
    u32 unk774;
    u32 unk778;
    u8 unk77C;
    u8 unk77D;
    u8 unk77E;
    u8 unk77F;
    HeapID heapId;
    u8 actionOrderCount;
    u8 unk783;
    u8 unk784;
    u8 unk785;
    u8 unk786;
    u8 unk787;
    // The mons that strike a mon switching out, such as with Pursuit
    u8 interruptCount;
    u8 unk789;
    u8 unk78A_0 : 1;
    u8 unk78A_1 : 1;
    u8 unk78A_2 : 1;
    u8 unk78A_3 : 1;
    u8 unk78A_4 : 1;
    u8 unk78A_5 : 1;
    u8 unk78A_6 : 1;
    u8 unk78A_7 : 1;
    u8 interruptMonIds[6];
    u8 unk791[0x18];
    // Per mon ID, cleared when the mon is revived
    u8 unk7A9[24];
    u8 unk7C1[24];
    u8 unk7D9[4];
    u8 unk7DD[3];
    ActionOrderEntry actionOrder[6];
    // Where an entry is kept while the order is reshuffled
    ActionOrderEntry tempEntry;
    // Two mon sets in overlay 169's format: the move's targets, and a copy
    void *unk850;
    void *unk854;
    void *unk858;
    void *unk85C;
    void *unk860;
    void *unk864;
    void *unk868;
    BtlFlowDamageList *unk86C;
    BtlFlowDamageList *unk870;
    BtlFlowWorkFrame frames[7];
    u32 frameDepth;
    // The mons that came in this turn (0x48 bytes on the DS; typed, as monSets above: on a PC target its clear,
    // sizeof(BtlMonSet), overran into unk1AB0, unk1AB4 and posPoke)
    BtlMonSet unk1A68;
    BtlFlowMoveParam *unk1AB0;
    BtlFlowMoveParam *unk1AB4;
    BtlPosPoke posPoke;
    u8 unk1AD0[0x14];
    BattleHandlerString message;
    BtlFlowExpEntry expEntries[6];
    BtlFlowUnk1B50 unk1B50;
    BtlActionState actionState;
    u8 unk1F6C[8];
    u16 unk1F74;
    u8 unk1F76[2];
    u32 unk1F78;
    u16 unk1F7C;
    u16 unk1F7E;
    // How often the player's mons and their opponents hit each other for no effect, super or not very effectively
    u16 unk1F80[4];
    u32 unk1F88[24];
    u8 unk1FE8[4];
    u8 unk1FEC[0x140];
};

// Black 2's, not checked against Black
struct BattleHandlerDelayMoveDamageParam {
    BattleHandlerHeader header;
    u8 attackerIndex;
    u8 targetIndex;
    u16 move;
};

// Black 2's; twinned code confirms the offsets of sourceMonIndex and targetMonIndex in Black
struct BattleHandlerBatonPassParam {
    u32 unk00;
    u8 sourceMonIndex;
    u8 targetMonIndex;
};

// Black 2's; twinned code confirms the offsets of amount, monIndex, sourceIndex and string in Black
struct BattleHandlerDrainParam {
    BattleHandlerHeader header;
    u16 amount;
    u8 monIndex;
    u8 sourceIndex;
    BattleHandlerString string;
};

// Black 2's, not checked against Black
struct BattleHandlerGravityCheckParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 19;
};

// Black 2's; twinned code confirms the offsets of monIndex in Black
struct BattleHandlerQuitBattleParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 19;
};

// Black 2's; twinned code confirms the offsets of monIndex in Black
struct BattleHandlerRecoverStatStageParam {
    u32 unk00;
    u8 monIndex;
};

// Black 2's; twinned code confirms the offsets of monIndex, amount and string in Black
struct BattleHandlerReviveParam {
    u32 unk00;
    u8 monIndex;
    u8 unk05;
    u16 amount;
    BattleHandlerString string;
};

// Black 2's; twinned code confirms the offsets of monIndex, checkFullHp and allowFainted in Black
struct BattleHandlerUseHeldItemParam {
    u32 unk00 : 8;
    u32 monIndex : 5;
    u32 unk13 : 19;
    u32 checkFullHp : 1;
    u32 allowFainted : 1;
    u32 unk22 : 30;
};

u8 BattleHandler_RecoverHP(BtlServerFlow *handler, const BattleHandlerRecoverHPParam *param, u16 itemId);
u8 BattleHandler_RecoverPP(BtlServerFlow *handler, const BattleHandlerPPParam *param, u16 itemId);
u8 BattleHandler_AddCondition(BtlServerFlow *handler, const BattleHandlerAddConditionParam *param);
u8 BattleHandler_SetStatStage(BtlServerFlow *handler, const BattleHandlerSetStatStageParam *param);
u8 BattleHandler_SetStatus(BtlServerFlow *handler, const BattleHandlerSetStatusParam *param);
u8 BattleHandler_AddSideEffect(BtlServerFlow *handler, const BattleHandlerAddSideEffectParam *param);
u8 BattleHandler_RemoveSideEffectCore(BtlServerFlow *handler, const BattleHandlerRemoveSideEffectParam *param);
u8 BattleHandler_UpdateMove(BtlServerFlow *handler, const BattleHandlerUpdateMoveParam *param);
u8 BattleHandler_ForceSwitch(BtlServerFlow *handler, const BattleHandlerForceSwitchParam *param);
u8 BattleHandler_EffectAtPos(BtlServerFlow *handler, const BattleHandlerEffectAtPosParam *param);
u8 BattleHandler_SetTurnFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param);
u8 BattleHandler_SetContinueFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param);
u8 BattleHandler_InterruptAction(BtlServerFlow *handler, BattleHandlerInterruptParam *param);
u8 BattleHandler_InterruptMove(BtlServerFlow *handler, BattleHandlerInterruptParam *param);
u8 BattleHandler_AbilityPopupRemove(BtlServerFlow *handler, BattleHandlerPopupParam *param);
u8 BattleHandler_HideTurnCancel(BtlServerFlow *handler, BattleHandlerHideTurnParam *param);
u8 BattleHandler_RemoveMessageWindow(BtlServerFlow *handler, BattleHandlerHeader *header);
u8 BattleHandler_ChangeForm(BtlServerFlow *handler, BattleHandlerChangeFormParam *param);
u8 BattleHandler_Flinch(BtlServerFlow *handler, BattleHandlerFlinchParam *param);
u8 BattleHandler_SetWeight(BtlServerFlow *handler, BattleHandlerSetWeightParam *param);
u8 BattleHandler_Revive(BtlServerFlow *handler, BattleHandlerReviveParam *param);
u8 BattleHandler_SetCounter(BtlServerFlow *handler, BattleHandlerSetCounterParam *param);
u8 BattleHandler_CheckHeldItem(BtlServerFlow *handler, BattleHandlerCheckHeldItemParam *param);
u8 BattleHandler_UseHeldItem(BtlServerFlow *handler, BattleHandlerUseHeldItemParam *param);
u8 BattleHandler_ConsumeItem(BtlServerFlow *handler, BattleHandlerConsumeItemParam *param);
u8 BattleHandler_QuitBattle(BtlServerFlow *handler, BattleHandlerQuitBattleParam *param);
u8 BattleHandler_AddFieldEffect(BtlServerFlow *handler, BattleHandlerAddFieldEffectParam *param);
u8 BattleHandler_RemoveFieldEffect(BtlServerFlow *handler, BattleHandlerRemoveFieldEffectParam *param);
u8 BattleHandler_RecoverStatStage(BtlServerFlow *handler, BattleHandlerRecoverStatStageParam *param);
u8 BattleHandler_ResetStatStage(BtlServerFlow *handler, BattleHandlerResetStatStageParam *param);
u8 BattleHandler_Message(BtlServerFlow *handler, BattleHandlerMessageParam *param);
u8 BattleHandler_DecrementPP(BtlServerFlow *handler, BattleHandlerPPParam *param, u16 itemId);
u8 BattleHandler_BatonPass(BtlServerFlow *handler, BattleHandlerBatonPassParam *param);
u8 BattleHandler_IllusionBreak(BtlServerFlow *handler, BattleHandlerIllusionBreakParam *param);
u8 BattleHandler_SwapPoke(BtlServerFlow *handler, BattleHandlerSwapPokeParam *param);
u8 BattleHandler_Drain(BtlServerFlow *handler, BattleHandlerDrainParam *param, u16 itemId);
u8 BattleHandler_Damage(BtlServerFlow *handler, BattleHandlerDamageParam *param);
u8 BattleHandler_ChangeHP(BtlServerFlow *handler, BattleHandlerChangeHPParam *param);
u8 BattleHandler_Faint(BtlServerFlow *handler, BattleHandlerFaintParam *param);
u8 BattleHandler_ChangeType(BtlServerFlow *handler, BattleHandlerChangeTypeParam *param);
u8 BattleHandler_SetMoveEffectIndex(BtlServerFlow *handler, BattleHandlerMoveEffectParam *param);
u8 BattleHandler_SetMoveEffectEnable(BtlServerFlow *handler, BattleHandlerHeader *header);
u8 func_ov093_021c8c6c(BtlServerFlow *handler, BattleHandlerHeader *param);
u8 func_ov093_021c9a24(BtlServerFlow *handler, const BattleHandlerPosEffectParam *param);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void BattleHandler_StrClear(BattleHandlerString *string);
void BattleHandler_AddArg(BattleHandlerString *string, u32 arg);
void BattleHandler_AddSoundEffect(BattleHandlerString *string, u32 soundEffect);
void *BattleHandler_PushWork(BtlServerFlow *flow, u32 command, u32 monId);
void BattleHandler_PopWork(BtlServerFlow *flow, void *work);
BOOL BattleHandler_SetString(BtlServerFlow *handler, const BattleHandlerString *string);
u8 *func_ov093_021c8120(BtlServerFlow *flow, u32 size);
u32 BattleHandler_Result(BtlServerFlow *handler);
void BattleHandler_Execute(BtlServerFlow *flow, void *work);
u8 BattleHandler_AbilityPopupAdd(BtlServerFlow *handler, BattleHandlerHeader *param);
u8 BattleHandler_CureCondition(BtlServerFlow *handler, const struct BattleHandlerCureConditionParam *param, u16 context);
u8 BattleHandler_StatChange(BtlServerFlow *handler, const struct BattleHandlerStatChangeParam *param, u16 context);
u8 BattleHandler_DelayMoveDamage(BtlServerFlow *handler, const BattleHandlerDelayMoveDamageParam *param);
u8 BattleHandler_ResetTurnFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param);
u8 BattleHandler_ResetContinueFlag(BtlServerFlow *handler, BattleHandlerFlagParam *param);
u8 BattleHandler_SendLast(BtlServerFlow *handler, BattleHandlerInterruptParam *param);
u8 BattleHandler_Switch(BtlServerFlow *handler, BattleHandlerSwitchParam *param);
u8 BattleHandler_ForceUseItem(BtlServerFlow *handler, BattleHandlerForceUseItemParam *param);
u8 BattleHandler_ChangeWeather(BtlServerFlow *handler, BattleHandlerChangeWeatherParam *param);
u8 BattleHandler_GravityCheck(BtlServerFlow *handler, BattleHandlerGravityCheckParam *param);
u8 BattleHandler_Transform(BtlServerFlow *handler, BattleHandlerTransformParam *param);
u8 BattleHandler_SetItem(BtlServerFlow *handler, BattleHandlerSetItemParam *param);
u8 BattleHandler_SwapItem(BtlServerFlow *handler, BattleHandlerSwapItemParam *param);
u8 BattleHandler_AbilityChange(BtlServerFlow *handler, BattleHandlerAbilityChangeParam *param);
u32 func_ov093_021c8910(BtlServerFlow *flow);

// Black 2's, not checked against Black
// Clears a set of flags whose first byte is its size in bytes
static inline void BattleHandler_InitFlags(u8 *flags, u32 size) {
    u32 i;

    flags[0] = size;
    for (i = 1; i < size; i++) {
        flags[i] = 0;
    }
}

// Black 2's, not checked against Black
static inline void BattleHandler_SetFlag(u8 *flags, u32 index) {
    u8 byte = (index >> 3) + 1;
    u8 bit = index & 7;

    if (byte < flags[0]) {
        flags[byte] |= 1 << bit;
    }
}

// Black 2's, not checked against Black
// A set of flags whose first byte is its size in bytes
static inline BOOL BattleHandler_IsFlagSet(const u8 *flags, u32 index) {
    u8 byte = (index >> 3) + 1;
    u8 bit = index & 7;

    if (byte < flags[0]) {
        if (flags[byte] & (1 << bit)) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

#endif // BWPORT_BATTLE_BTL_HANDLER_H
