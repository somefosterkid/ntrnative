// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_server_flow.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_SERVER_FLOW_H
#define BWPORT_BATTLE_BTL_SERVER_FLOW_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "battle/btl_pokeparam.h"

// The stat that a move's stat change affects, as PML_MoveGetStatChangeStage returns it: one stat from 1, or
// MOVE_STAT_CHANGE_ALL for the five from attack to speed at once. An enum, which MWCC loads again where it is
// passed on after tests of it that share one load, as func_ov093_021c2e64 does
typedef enum {
    MOVE_STAT_CHANGE_NONE = 0,
    MOVE_STAT_CHANGE_ALL = 10,
} MoveStatChangeStat;

// The damage of a move, with the type effectiveness if withEffectiveness is set. damageRoll is USE_MIN_DAMAGE for the
// lowest random roll, or ROLL_FOR_DAMAGE for a random one
u32 AICalcDamage(BtlServerFlow *serverFlow, u8 attackerId, u8 defenderId, u16 move, BOOL withEffectiveness,
                 u32 damageRoll);
u16 GetTurnCounter(BtlServerFlow *serverFlow);
BOOL CheckEvolution(BtlServerFlow *flow, u8 monId);
BOOL IsMonSwitchingOut(BtlServerFlow *flow);
void AddSwitchOutInterrupt(BtlServerFlow *flow, u8 monId);
BOOL ServerControl_HideTurnCancel(BtlServerFlow *handler, BattleMon *mon, u32 flag);
void ServerControl_SwitchInFillSlot(BtlServerFlow *handler, u8 target, u8 slot, u8 slotAgain, BOOL flag);
void ServerControl_SetMonCounter(BtlServerFlow *handler, BattleMon *mon, u32 counter, u8 value);
BOOL ServerControl_EscapeSub(BtlServerFlow *handler, BattleMon *mon, u32 flag);
void SetMoveEffectIndex(BtlServerFlow *flow, u8 index);
BOOL ServerControl_SwitchOut(BtlServerFlow *handler, BattleMon *mon, u8 flag);
BOOL ServerControl_FieldEffectCore(BtlServerFlow *handler, u32 effect, BattleCondition value, u8 dependPoke);
void ServerControl_FieldEffectEnd(BtlServerFlow *handler, u32 effect);
BOOL ServerControl_DecrementPP(BtlServerFlow *handler, BattleMon *mon, u8 moveIndex, u8 amount);
void ServerControl_MoveCore(BtlServerFlow *handler, u8 clientId, u8 firstSlot, u8 secondSlot, u32 flag);
void ServerControl_AfterMove(BtlServerFlow *handler, u8 clientId, u8 firstSlot, u8 secondSlot);
BOOL ServerControl_ChangeWeatherCheck(BtlServerFlow *handler, u8 weather, u8 duration);
void ServerControl_ChangeWeatherCore(BtlServerFlow *handler, u8 weather, u8 duration);
void ServerControl_CureCondition(BtlServerFlow *handler, BattleMon *mon, s32 condition, BattleCondition *prev);
void ServerControl_UnnerveAction(BtlServerFlow *handler, BattleMon *mon);
void ServerControl_ViewEffect(BtlServerFlow *handler, u16 effect, u8 pos1, u8 pos2, BOOL reserved, u32 reserve);
BOOL ServerControl_SimpleDamageCore(BtlServerFlow *handler, BattleMon *mon, u32 damage, BattleHandlerString *string);
void ServerControl_SwitchInCore(BtlServerFlow *flow, u8 clientId, u8 pos, u8 slot);
BOOL ServerControl_Escape(BtlServerFlow *flow, BattleMon *mon);
void ServerControl_ClearMonDependentEffects(BtlServerFlow *flow, BattleMon *mon, BOOL flag);
u32 GetEnemyMaxLevel(BtlServerFlow *flow);
BOOL ActionOrder_InterruptProc(BtlServerFlow *flow, u8 monId, u8 targetId);
BOOL ServerControl_CheckFainted(BtlServerFlow *flow, BattleMon *mon);
void ServerDisplay_AddEffectAtPosition(BtlServerFlow *flow, BattleMon *mon, u32 effect);
BOOL ServerControl_RecoverHPCheckFail(BtlServerFlow *flow, BattleMon *mon);
BOOL ServerControl_RecoverHP(BtlServerFlow *flow, BattleMon *mon, u16 amount, BOOL flag);
void ServerDisplay_FaintPokemon(BtlServerFlow *flow, BattleMon *mon, u32 flag);
BOOL ServerControl_RecoverHPCheckFailSpecial(BtlServerFlow *flow, BattleMon *mon, BOOL showMessage);
void ServerControl_RecoverHPCore(BtlServerFlow *flow, BattleMon *mon, u16 amount);
BOOL ServerEvent_AddCondition(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                              BattleCondition value, BOOL flag, BOOL defaultMessage);
BOOL ServerControl_MoveConditionCore(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move,
                                     u32 condition, BattleCondition value, BOOL flag);
void ServerEvent_DamageAddEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target);
void ServerControl_DamageAddCondition(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *attacker,
                                      BattleMon *target);
void ServerControl_DamageDrain(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                               u32 damage);
void ServerControl_SkyDropCheckRelease(BtlServerFlow *flow, BattleMon *mon, BOOL flag);
void ServerDisplay_SkyDropTargetAppear(BtlServerFlow *flow, BattleMon *mon, u16 effect);

BOOL func_ov093_021c80ac(BtlServerFlow *flow, u8 monId);
void func_ov093_021c8434(BtlServerFlow *flow, u8 monId, u8 targetId);

BOOL func_ov093_021c804c(BtlServerFlow *flow, u8 monId, BattleAction *action);
u8 func_ov093_021c8010(BtlServerFlow *serverFlow, u8 monId);
u8 func_ov093_021c7d54(BtlServerFlow *flow, u8 monId, u8 *monIds);
BOOL func_ov093_021c8234(BtlServerFlow *flow, u8 monId);

u8 func_ov093_021c7ed8(BtlServerFlow *flow, u8 monId);
u8 func_ov093_021c8030(BtlServerFlow *flow, u8 monId, u16 move);
BOOL func_ov093_021c83e8(BtlServerFlow *flow, u32 money, u8 monId);
s32 func_ov093_021c83a0(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c824c(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c82c4(BtlServerFlow *flow, u8 side, u32 sideEffect);
BattleParty *func_ov093_021c7fcc(BtlServerFlow *flow, u8 monId);
BattleParty *func_ov093_021c7fe0(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c7f24(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c7f4c(BtlServerFlow *flow);
BOOL func_ov093_021c8374(BtlServerFlow *handler, u8 monIndex);
// pokebw2's BtlServerFlow_GetWazaRec
BtlWazaRec *func_ov093_021c812c(BtlServerFlow *flow);
// pokebw2's BtlServerFlow_GetFaintRec
BtlFaintRec *func_ov093_021c8130(BtlServerFlow *flow);
u8 func_ov093_021c7d34(BtlServerFlow *flow, u8 monId);

// Black 2's, not checked against Black
// The move that a move calls instead of itself, such as Metronome's, and its target
struct BtlFlowCalledMove {
    u16 move;
    u8 target;
};

// Black 2's, not checked against Black
// What a move's use reports back
struct BtlFlowFightWork {
    u8 result;
    BtlFlowCalledMove called;
};

// Walks the mons in battle of every client
struct BtlFlowMonIter {
    u8 clientId;
    u8 index;
    u8 done;
    // Set in rotation battles, to walk all three slots
    u8 rotation;
};

// Black 2's; twinned code confirms the offsets of multipleTargets and unk1 in Black
struct BtlFlowDamageFlags {
    u32 multipleTargets : 1;
    u32 unk1 : 1;
    u32 unk2 : 30;
};

u32 GetBattleTerrain(BtlServerFlow *flow);
void ServerControl_FaintPokemon(BtlServerFlow *handler, BattleMon *mon);
BOOL ServerControl_ChangeWeather(BtlServerFlow *flow, u8 weather, u8 turns);
void StoreBattleMonsSpeedOrder(BtlServerFlow *flow, void *monSet);
void ServerControl_SideEffectEndMessage(u8 side, u32 effect, BtlServerFlow *flow);
BOOL IsGuaranteedHit(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender);
void ServerControl_SimpleEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void ServerControl_SimpleCondition(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets);
void ServerControl_ForceSwitch(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets);
void ServerDisplay_AddCondition(BtlServerFlow *flow, BattleMon *mon, s32 condition, BattleCondition value);
u32 AddConditionCheckFailOverwrite(BtlServerFlow *flow, BattleMon *mon, s32 condition, BattleCondition value,
                                   u8 overwrite);
void AddConditionCheckFailStandard(BtlServerFlow *flow, BattleMon *mon, u32 cause, u32 condition);
void ServerControl_OHKOSuccess(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical);
void ServerControl_CalcRecoil(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 damage);
void func_ov093_021bbb48(BtlServerFlow *serverFlow);
void func_ov093_021bbc34(BtlServerFlow *serverFlow);
void func_ov093_021bbcd4(BtlServerFlow *flow);
void func_ov093_021bbd20(BtlServerFlow *serverFlow);
u32 func_ov093_021bbd2c(BtlServerFlow *serverFlow, BtlClientActions *clientActions);
u32 func_ov093_021bbd8c(BtlServerFlow *serverFlow, BtlClientActions *clientActions);
BOOL func_ov093_021bc220(BtlServerFlow *flow, BtlClientActions *clientActions);
BOOL func_ov093_021bc310(BtlServerFlow *flow);
u32 func_ov093_021bc360(BtlServerFlow *serverFlow);
BOOL func_ov093_021bc390(BtlServerFlow *serverFlow);
void func_ov093_021bc3b0(BtlServerFlow *flow);
void func_ov093_021bc418(BtlServerFlow *flow, u8 clientId, u8 slot);
BOOL func_ov093_021bc4dc(BtlServerFlow *flow, BtlFlowClientList *list);
BtlClientIDList *func_ov093_021bc550(BtlServerFlow *serverFlow);
u8 func_ov093_021bc55c(BtlServerFlow *serverFlow);
void func_ov093_021bc874(ActionOrderEntry *order, u32 count);
ActionOrderEntry *func_ov093_021bca50(BtlServerFlow *flow, ActionOrderEntry *after, u8 monId);
ActionOrderEntry *func_ov093_021bcb14(BtlServerFlow *flow, u16 move, u8 monId, u8 target);
u8 func_ov093_021bcb68(BtlServerFlow *flow, ActionOrderEntry *entry);
void func_ov093_021bd1e0(BattleMoveEffectState *targets);
void func_ov093_021bd200(BattleMoveEffectState *targets, BtlServerFlow *flow, BattleMon *mon, void *monSet);
void func_ov093_021bd2b4(BtlFlowMonIter *iter, BtlServerFlow *flow);
void func_ov093_021bd338(BtlFlowMonIter *iter, BtlServerFlow *flow);
BOOL func_ov093_021bd34c(BtlFlowMonIter *iter, BtlServerFlow *flow, BattleMon **mon);
void func_ov093_021bd3e8(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bdc78(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bdc98(BtlServerFlow *flow, BattleMon *mon, u8 slot);
u16 func_ov093_021bde48(BattleMon *mon, BattleAction *action);
BOOL func_ov093_021be3a8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param);
void func_ov093_021be518(BtlFlowReactionList *list);
void func_ov093_021be52c(BtlFlowReactionList *list, u8 monId, u8 arg2, u8 target);
BOOL func_ov093_021be6ec(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target);
BOOL func_ov093_021beab4(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets, BtlFlowReactionList *list);
void func_ov093_021bebd8(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target);
BOOL func_ov093_021bec08(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bec2c(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021bf04c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void func_ov093_021bf0e4(BtlServerFlow *flow, BattleMon *mon, void *targets, BtlFlowMoveParam *param, BOOL isDamage);
void func_ov093_021bf1b4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, BOOL flag);
void func_ov093_021bf4ac(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data);
void func_ov093_021bf83c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void func_ov093_021bf8d4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
BOOL func_ov093_021bfa00(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move);
void func_ov093_021bfbd0(BtlServerFlow *flow, u16 move, BattleMoveEffectState *effect, u32 reserved);
void func_ov093_021bfe60(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021bfeac(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BOOL *failed);
BOOL func_ov093_021c024c(BtlServerFlow *flow, BattleMon *mon, u16 move);
BOOL func_ov093_021c03ac(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c03d8(BtlServerFlow *flow, BattleMon *mon, u16 move);
BOOL func_ov093_021c0404(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021c0520(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 cause);
void func_ov093_021c07ac(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 moveSlot, void *targets);
void func_ov093_021c08cc(BtlServerFlow *flow, BattleMon *mon, u8 moveIndex, u8 amount);
u32 func_ov093_021c0d90(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data,
                        u32 *reserved, u32 arg6);
u32 func_ov093_021c0f24(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data);
BOOL func_ov093_021c1184(BtlFlowHitWork *hitWork);
void func_ov093_021c1198(BtlFlowHitWork *hitWork, u32 value);
u32 func_ov093_021c11a4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, void *monSet,
                        BtlFlowDamageList *list, BtlFlowHitWork *hitWork, u32 ratio, BtlFlowDamageFlags flags);
void func_ov093_021c15ac(BtlServerFlow *flow, s32 effectiveness);
u32 func_ov093_021c15d4(BattleMon *mon, u32 damage);
void func_ov093_021c1828(BtlServerFlow *flow, u8 attackerPos, BattleMon *attacker, BattleMon *target,
                         BtlFlowMoveParam *param, u16 damage);
void func_ov093_021c1880(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                         u16 damage, BOOL flag);
void func_ov093_021c1ccc(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target);
void func_ov093_021c1ce4(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *monSet);
u32 func_ov093_021c2e4c(BtlServerFlow *flow);
void func_ov093_021c3184(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void func_ov093_021c3274(BtlServerFlow *flow, u16 move, BattleMon *mon, void *targets);
void func_ov093_021c36e8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical, u32 cause,
                         u16 damage);
u16 func_ov093_021c3720(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 critical);
u32 func_ov093_021c39cc(BtlServerFlow *flow);
s32 func_ov093_021c39f4(BtlServerFlow *flow, BtlServerClient *client);
BOOL func_ov093_021c4034(BtlServerFlow *flow, BattleMon *mon);
u16 func_ov093_021c4104(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 damage, u32 effectiveness,
                        u8 critical, BtlFlowMoveParam *param);
void func_ov093_021c41c0(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021c446c(BtlServerFlow *flow);
void func_ov093_021c4614(BtlServerFlow *flow);
void func_ov093_021c4744(BtlServerFlow *flow);
void func_ov093_021c4bac(BtlServerFlow *flow, u32 effect, u32 side);
void func_ov093_021c4c24(BtlServerFlow *flow);
void func_ov093_021c4c40(u32 effect, BtlServerFlow *flow);
void func_ov093_021c4ea4(BtlServerFlow *flow, BattleMon *mon, u32 weather, s32 damage);
void func_ov093_021c5474(BtlFlowUnk1B50 *work);
void func_ov093_021c54bc(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c54c8(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c54fc(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c5540(BtlServerFlow *flow, BattleMon *mon, u32 damage);
void func_ov093_021c557c(BtlServerFlow *flow, BattleMon *mon, u32 status, BOOL flag);
void func_ov093_021c5604(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021c5718(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021c572c(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021c5750(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c5774(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c5798(BtlServerFlow *flow, BtlFlowMoveParam *param, u32 count, u32 *effectiveness, BattleMon **mons,
                         u16 *damages, u8 *critical, BOOL multipleTargets);
void func_ov093_021c5840(BtlServerFlow *flow, u32 count, u32 *effectiveness, BattleMon **mons, BOOL multipleTargets);
void func_ov093_021c59c4(BtlServerFlow *flow, u32 count, BattleMon **mons, u8 *critical, BOOL multipleTargets);
void func_ov093_021c5a4c(BtlServerFlow *flow, BattleMon *mon, u32 stat, s32 change);
void func_ov093_021c5a8c(BtlServerFlow *flow, BattleMon *mon, u32 stat, s32 change, u16 context, BOOL flag);
void func_ov093_021c604c(BtlServerFlow *flow, BattleMon *mon, u32 damage);
void func_ov093_021c6158(BtlServerFlow *flow, BtlFlowClientList *list);
u16 func_ov093_021c63d0(BtlServerFlow *flow, BattleMon *mon);
u32 func_ov093_021c6e3c(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param, BOOL flag);
BOOL func_ov093_021c6f0c(BtlServerFlow *flow, BattleMon *mon, BOOL flag);
u16 func_ov093_021c7cd4(BtlServerFlow *flow);
u16 func_ov093_021c7cdc(BtlServerFlow *flow);
u16 func_ov093_021c7ce8(BtlServerFlow *flow);
// pokebw2's CalcMoveEffectiveness
u32 func_ov093_021c7dac(BtlServerFlow *serverFlow, u8 attackerId, u8 defenderId, u16 move);
BOOL func_ov093_021c7ec4(BtlServerFlow *flow);
u8 func_ov093_021c7eec(BtlServerFlow *flow, u8 monId);
u8 func_ov093_021c7f04(BtlServerFlow *flow, u8 monId);
u8 func_ov093_021c8140(BtlServerFlow *flow, u32 arg1);
u32 func_ov093_021c81d0(BtlServerFlow *serverFlow, BattleMon *mon, BOOL a2);
u32 func_ov093_021c82d0(BtlServerFlow *serverFlow, u8 pos, u32 sideEffect);
BOOL func_ov093_021c82f4(BtlServerFlow *serverFlow, u8 pos, u32 a2);
BOOL func_ov093_021c8348(BtlServerFlow *flow, u8 monId);
void func_ov093_021c8408(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c84ac(BtlServerFlow *flow, BattleMon *mon, u8 *flag);
void func_ov093_021c84e8(BtlServerFlow *flow);
void func_ov093_021c84f4(BtlServerFlow *flow, u8 monId);
BOOL func_ov093_021c8534(BtlServerFlow *flow);
void func_ov093_021c8588(BtlServerFlow *flow);
void func_ov093_021c859c(BtlServerFlow *flow);
void func_ov093_021c85b8(BtlServerFlow *flow);
void func_ov093_021c85d4(BtlServerFlow *flow, u32 depth, BOOL clear);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BattleMon *GetBattleMon(BtlServerFlow *serverFlow, u8 monId);
BOOL ServerControl_FlinchCore(BtlServerFlow *handler, BattleMon *mon, u32 chance);
BOOL ServerControl_AfterSwitchIn(BtlServerFlow *handler);
void ServerControl_CheckItemReaction(BtlServerFlow *handler, BattleMon *mon, u32 reaction);
void ServerControl_ChangeHeldItem(BtlServerFlow *handler, BattleMon *mon, u16 item, BOOL consume);
BOOL ServerControl_UseHeldItem(BtlServerFlow *handler, BattleMon *mon);
BOOL ServerControl_CheckMatchup(BtlServerFlow *handler);
BOOL ServerEvent_DecrementPP(BtlServerFlow *handler, BattleMon *mon, u8 moveIndex);
BOOL ServerEvent_CheckFloating(BtlServerFlow *handler, BattleMon *mon, BOOL flag);
BOOL ServerControl_DrainCore(BtlServerFlow *handler, BattleMon *mon, BattleMon *source, u16 amount);
BOOL ServerControl_CheckSimpleDamageEnabled(BtlServerFlow *handler, BattleMon *mon, u32 damage);
void ServerEvent_BeforeFaint(BtlServerFlow *flow, BattleMon *mon);
void ServerControl_AfterMoveCore(BtlServerFlow *flow, BattleMon *mon);
u32 ServerEvent_InterruptSwitch(BtlServerFlow *flow, BattleMon *mon);
void ServerControl_SwitchOutCore(BtlServerFlow *flow, BattleMon *mon, u32 effect);
void ServerControl_AddCondition(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                BattleCondition value, BOOL showMessage, BOOL skipItemReaction,
                                const BattleHandlerString *string);
BOOL ServerEvent_CalcDamage(BtlServerFlow *flow, BattleMon *attacker, BattleMon *defender,
                            const BtlFlowMoveParam *param, u32 effectiveness, u32 ratio, BOOL critical, BOOL fixedRoll,
                            u16 *damage);
u32 ServerEvent_GetWeather(BtlServerFlow *flow);
s32 ServerEvent_CalcRecoil(BtlServerFlow *flow, BattleMon *mon, u16 move, s32 damage, BOOL *forced);
u32 ServerEvent_DecideSpecialMoveCondition(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target,
                                           BattleHandlerString *string);
void ServerEvent_AddMoveConditionString(BtlServerFlow *flow, u32 condition, BattleMon *attacker, BattleMon *target,
                                        BattleHandlerString *string);
void ServerEvent_MoveConditionContinue(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 condition,
                                       BattleCondition *value);
BOOL ServerControl_AddConditionCheckFail(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                         BattleCondition value, u8 overwrite, BOOL showFail);
fx32 ServerEvent_GetWeightRatio(BtlServerFlow *flow, BattleMon *mon);
BOOL ServerEvent_RollStatDropEffectChance(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker,
                                          BattleMon *target);
BOOL ServerControl_ForceSwitchCore(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BOOL forced,
                                   BOOL *failed, u16 effect, BOOL ignoreLevel, const BattleHandlerString *string);
void ServerControl_ChangeWeatherAfter(BtlServerFlow *flow, u8 weather);
u32 ServerEvent_CheckMoveAddCondition(BtlServerFlow *flow, u16 move, BattleMon *attacker, BattleMon *target,
                                      BattleCondition *value);
void ServerEvent_GetMoveParam(BtlServerFlow *flow, u16 move, BattleMon *mon, BtlFlowMoveParam *param);
u32 func_ov093_021c8168(BtlServerFlow *flow);
BOOL func_ov093_021c83d4(BtlServerFlow *flow);
u32 func_ov093_021c815c(BtlServerFlow *flow);
u8 func_ov093_021c8020(BtlServerFlow *flow, u8 pos);
u8 func_ov093_021c7d44(BtlServerFlow *flow, u8 pos);
u16 func_ov093_021c81c8(BtlServerFlow *serverFlow, BattleMon *mon, BOOL a2);
u32 func_ov093_021c83cc(BtlServerFlow *flow);
u32 func_ov093_021c8180(BtlServerFlow *flow);
BOOL func_ov093_021c814c(BtlServerFlow *flow, u8 monId);
u16 func_ov093_021c8114(BtlServerFlow *flow);
u8 func_ov093_021c8300(BtlServerFlow *flow, u8 clientId);
BOOL func_ov093_021c846c(BtlServerFlow *flow, u8 attackerId, u8 targetId, BOOL *failed);
void func_ov093_021c8494(BtlServerFlow *flow, u8 monId);
void ServerEvent_EquipTempItem(BtlServerFlow *handler, BattleMon *mon, u8 monIndex);
void ServerEvent_GastroAcidConfirmed(BtlServerFlow *handler, BattleMon *mon);
u16 ServerEvent_CalculateSpeed(BtlServerFlow *flow, BattleMon *mon, BOOL flag);
void ServerControl_SwitchOutConfirm(BtlServerFlow *flow, BattleMon *mon);
void ServerControl_OHKO(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void ServerControl_FieldEffect(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon);
void ServerEvent_CheckItemReaction(BtlServerFlow *flow, BattleMon *mon, u32 reaction);
BOOL ServerEvent_CheckHeldItemFail(BtlServerFlow *flow, BattleMon *mon, u16 item);
void ServerEvent_EquipItem(BtlServerFlow *flow, BattleMon *mon);
BOOL ServerEvent_MoveConditionCheckFail(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 condition);
void ServerEvent_AddConditionFailed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition);
void ServerEvent_ConditionConfirmed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition,
                                    BattleCondition value);
void ServerEvent_MoveStatusConfirmed(BtlServerFlow *flow, BattleMon *target, BattleMon *attacker, u32 condition);
void ServerEvent_GetMoveStatChangeValue(BtlServerFlow *flow, u16 move, u32 index, BattleMon *attacker,
                                        BattleMon *target, MoveStatChangeStat *stat, s32 *change);
s32 ServerEvent_CheckSubstituteInteraction(BtlServerFlow *flow, BattleMon *mon, u32 stat, u8 attackerId, u16 context,
                                           s32 change);
u8 ServerEvent_IncreaseMoveWeatherTurns(BtlServerFlow *flow, u8 weather, BattleMon *mon);
BtlServerFlow *func_ov093_021bb968(BtlServer *server, BtlMainModule *mainModule, BtlPokeCon *pokeCon,
                                   BtlServerCmdQueue *queue, u32 a4, HeapID heapId);
void func_ov093_021bb9d0(BtlServerFlow *serverFlow);
// Not decompiled yet
void func_ov093_021bb9d8(BtlServerFlow *flow);
BOOL func_ov093_021bbb60(BtlServerFlow *serverFlow);
u32 func_ov093_021bbc44(BtlServerFlow *serverFlow, BtlClientActions *clientActions);
void func_ov093_021bbd80(BtlServerFlow *serverFlow);
u32 func_ov093_021bbfa8(BtlServerFlow *flow, u32 i);
void func_ov093_021bc0e8(BtlServerFlow *flow, ActionOrderEntry *order, u32 count);
void func_ov093_021bc568(BtlServerFlow *flow);
u8 func_ov093_021bc610(BtlServerFlow *flow, BtlClientActions *clientActions, const ActionOrderEntry *order, u8 max);
u8 func_ov093_021bc8ec(BtlServerFlow *flow, u16 move, BattleMon *mon);
u32 func_ov093_021bcce0(BtlServerFlow *flow, ActionOrderEntry *entry);
void func_ov093_021bcefc(BtlServerFlow *flow, BattleMon *mon, u32 action);
void func_ov093_021bcf34(BtlServerFlow *flow, BattleMon *mon, u32 action);
void func_ov093_021bcf70(BtlServerFlow *flow, BattleMon *mon, u32 action);
u32 func_ov093_021bcfac(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021bd080(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bd554(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021bd6b8(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021bd708(BtlServerFlow *flow, BattleMon *mon, BOOL arg2, BOOL arg3);
BOOL func_ov093_021bd850(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021bd8ac(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bdb88(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bdbb8(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021bdbec(BtlServerFlow *flow);
void func_ov093_021bdc0c(BtlServerFlow *flow);
void func_ov093_021bdc2c(BtlServerFlow *flow);
void func_ov093_021bdc58(BtlServerFlow *flow);
void func_ov093_021bde98(BtlServerFlow *flow, BattleMon *mon, BattleAction *action, u32 key);
void func_ov093_021be400(BtlServerFlow *flow, u16 move);
void func_ov093_021be550(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets);
void func_ov093_021be620(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param);
void func_ov093_021be66c(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 event);
void func_ov093_021be6a8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, u32 event);
BOOL func_ov093_021be7e4(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target, BtlFlowFightWork *work);
BOOL func_ov093_021be878(BtlServerFlow *flow, BattleMon *mon, u8 target);
void func_ov093_021be8c4(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021be8f4(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021be924(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021be95c(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021be994(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 result);
void func_ov093_021be9d0(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 result);
void func_ov093_021bea14(BtlServerFlow *flow, BattleMon *mon, BattleMon *attacker, u16 move);
void func_ov093_021bea60(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move);
BOOL func_ov093_021beb14(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets, u8 *monId, u8 *target);
BOOL func_ov093_021bec58(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets);
BOOL func_ov093_021bf168(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon);
void func_ov093_021bf244(BtlServerFlow *flow, u8 monId, u16 move);
void func_ov093_021bf27c(BtlServerFlow *flow, u8 monId, u16 move);
void func_ov093_021bf2b4(BtlServerFlow *flow, u8 monId, u16 move);
void func_ov093_021bf2ec(BtlServerFlow *flow, u8 monId, u16 move, u32 event);
void func_ov093_021bf3d8(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data);
BOOL func_ov093_021bf6e8(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, BattleMon *target, void *data,
                         u32 event);
BOOL func_ov093_021bf78c(BtlServerFlow *flow, BtlFlowMoveParam *param, u32 event, BattleMon *mon, BattleMon *target,
                         void *data, BattleHandlerString *string, BOOL *silent);
BOOL func_ov093_021bf9a4(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param);
BOOL func_ov093_021bfa60(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param);
u32 func_ov093_021bfc08(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move);
BOOL func_ov093_021bfd24(BtlServerFlow *flow, BattleMon *mon, u8 pos, void *targets, u16 move);
BOOL func_ov093_021c001c(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 flag);
BOOL func_ov093_021c0274(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 status);
u32 func_ov093_021c02cc(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 event);
BOOL func_ov093_021c031c(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c0450(BtlServerFlow *flow, BattleMon *mon, u16 move, s32 cause);
u32 func_ov093_021c07d4(BtlServerFlow *flow, BattleMon *mon, u8 moveSlot, u16 move, void *targets);
void func_ov093_021c091c(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, void *data,
                         u32 arg5);
void func_ov093_021c0a4c(BtlServerFlow *flow, BattleMon *attacker, void *targets, const BtlFlowMoveParam *param,
                         void *effectiveness, u32 arg5, BtlFlowDamageList *list);
u32 func_ov093_021c0c34(BtlFlowDamageList *list);
u32 func_ov093_021c0c38(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons, u16 *damages,
                        u32 *effectiveness, u8 *critical);
u32 func_ov093_021c0cb4(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons);
u32 func_ov093_021c0ce8(BtlServerFlow *flow, const BtlFlowDamageList *list, BattleMon **mons, u16 *damages,
                        u32 *effectiveness, u8 *critical, u8 *unk9);
u32 func_ov093_021c0d70(const BtlFlowDamageList *list);
u8 func_ov093_021c1194(BtlFlowHitWork *hitWork);
u32 func_ov093_021c11f0(BtlServerFlow *flow, BattleMon *attacker, void *monSet, BtlFlowDamageList *list,
                        BtlFlowMoveParam *param, BtlFlowHitWork *hitWork, u32 ratio, BtlFlowDamageFlags flags);
void func_ov093_021c14e0(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param, void *targets);
void func_ov093_021c15e8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param);
void func_ov093_021c1624(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param);
u32 func_ov093_021c1678(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u32 arg3, u16 *damage);
void func_ov093_021c16f8(BtlServerFlow *flow, BattleMon *mon, u32 cause);
void func_ov093_021c1758(BtlServerFlow *flow, BattleMon *mon, u32 cause);
void func_ov093_021c1788(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, u8 count, BattleMon **mons);
void func_ov093_021c17cc(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *attacker, u8 count, BattleMon **mons);
void func_ov093_021c18d4(BtlServerFlow *flow, BattleMon *attacker, BtlFlowMoveParam *param, u32 damage);
void func_ov093_021c1910(BtlServerFlow *flow, BattleMon *attacker, BtlFlowMoveParam *param, u32 damage);
void func_ov093_021c19d8(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param);
void func_ov093_021c1a10(BtlServerFlow *flow, BattleMon *mon, BtlFlowMoveParam *param);
void func_ov093_021c1a54(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *monSet, u32 damage,
                         u32 arg5);
void func_ov093_021c1b5c(BtlServerFlow *flow, BattleMon *mon, void *monSet, BtlFlowMoveParam *param, u32 arg4,
                         BOOL flag, u32 event);
void func_ov093_021c1c88(BtlServerFlow *flow, BattleMon *mon, void *monSet, BtlFlowMoveParam *param, u32 arg4);
BOOL func_ov093_021c2e64(BtlServerFlow *flow, const BtlFlowMoveParam *param, BattleMon *attacker, BattleMon *target,
                         BOOL showFail);
BOOL func_ov093_021c3008(BtlServerFlow *flow, u8 monId, BattleMon *mon, MoveStatChangeStat stat, s32 change, u8 attackerId,
                         u16 context, u32 value, BOOL showFail, BOOL flag);
BOOL func_ov093_021c3a4c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target);
void func_ov093_021c3ccc(BtlServerFlow *flow, BattleMon *mon, u16 move);
void func_ov093_021c3d08(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets);
void func_ov093_021c3e0c(BtlServerFlow *flow, BattleMon *mon, void *targets);
void func_ov093_021c4218(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param,
                         u32 effectiveness, u32 damage, u32 critical, BOOL flag);
void func_ov093_021c4268(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param,
                         u32 effectiveness, u32 damage, u32 critical, BOOL flag);
BOOL func_ov093_021c4304(BtlServerFlow *flow, BtlFlowMoveParam *param, BattleMon *mon, void *targets, BOOL *showFail);
BOOL func_ov093_021c4838(BtlServerFlow *flow, void *monSet, u32 event);
void func_ov093_021c4910(BtlServerFlow *flow, u8 monId, u32 event);
BOOL func_ov093_021c493c(BtlServerFlow *flow, void *monSet);
void func_ov093_021c4a64(BtlServerFlow *flow, BattleMon *mon, u32 condition, u32 damage);
u32 func_ov093_021c4b3c(BtlServerFlow *flow, BattleMon *mon, u32 condition, u32 damage);
void func_ov093_021c4b8c(BtlServerFlow *flow);
BOOL func_ov093_021c4d1c(BtlServerFlow *flow, void *monSet);
s32 func_ov093_021c4e38(BtlServerFlow *flow, BattleMon *mon, u32 weather, s32 damage);
BOOL func_ov093_021c5200(BtlServerFlow *flow);
BOOL func_ov093_021c5308(BtlServerFlow *flow, BattleMon *mon);
BOOL func_ov093_021c5350(BtlServerFlow *flow, BattleParty *party, const BtlFlowExpEntry *entries);
BOOL func_ov093_021c62d8(BtlServerFlow *flow, BattleMon *mon, u16 move, u8 target, BtlFlowCalledMove *called);
u8 func_ov093_021c6350(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c6394(BtlServerFlow *flow, BattleMon *mon, u16 move);
BOOL func_ov093_021c6458(BtlServerFlow *flow, BattleMon *mon, u16 move, u16 actualMove, BattleHandlerString *string);
void func_ov093_021c6564(BtlServerFlow *flow, BattleMon *mon, u16 move, u32 cause);
u32 func_ov093_021c65a8(BtlServerFlow *flow, BattleMon *mon, u16 move, void *targets);
BOOL func_ov093_021c6668(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move);
BOOL func_ov093_021c66ac(BtlServerFlow *flow, BattleMon *mon, void *targets);
BOOL func_ov093_021c6720(BtlServerFlow *flow, BattleMon *mon, u16 move);
BOOL func_ov093_021c676c(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move, u8 *monId, BOOL *failed);
void func_ov093_021c6848(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c6878(BtlServerFlow *flow, BattleMon *mon, u16 move);
u32 func_ov093_021c68a8(BtlServerFlow *flow, BattleMon *mon, void *targets, u16 move);
BOOL func_ov093_021c6948(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move);
BOOL func_ov093_021c69b8(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, u16 move);
u32 func_ov093_021c6a04(BtlServerFlow *flow, BattleMon *mon, BattleMon *target, BtlFlowMoveParam *param);
BOOL func_ov093_021c6a9c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move);
BOOL func_ov093_021c6b5c(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, BtlFlowMoveParam *param);
void func_ov093_021c6bb8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target);
BOOL func_ov093_021c6bf8(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u16 move);
void func_ov093_021c6fc0(BtlServerFlow *flow, BattleMon *mon);
void func_ov093_021c7008(BtlServerFlow *flow, BattleMon *mon);
u32 func_ov093_021c7038(BtlServerFlow *flow, BattleMon *attacker, BattleMon *target, u8 moveType, u8 defenseType);
BOOL func_ov093_021c8338(BtlServerFlow *flow, u8 monId);
void func_ov093_021c84d0(BtlServerFlow *flow, BattleMon *mon);
u32 func_ov093_021c84d8(BtlServerFlow *serverFlow);
void func_ov093_021c84e0(BtlServerFlow *flow, u8 monId);
void ServerEvent_CheckSideEffectParam(BtlServerFlow *flow, u8 monId, u32 effect, u8 side, BattleCondition *cont);

// Black 2's, not checked against Black
// The ID of the mon that a mon carries off with Sky Drop, 0x1f for none
static inline u8 BtlFlow_GetSkyDropTarget(BattleMon *mon) {
    u8 targetId;
    u8 counter = GetConditionCount(mon, 4);

    if (counter == 0 || (targetId = counter - 1) >= 0x18) {
        targetId = 0x1f;
    }
    return targetId;
}

// Black 2's, not checked against Black
// A mon that is there and hasn't fainted
static inline BOOL BtlFlow_IsMonAlive(BattleMon *mon) {
    if (mon != NULL) {
        if (!IsFainted(mon)) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// ov095 code the server flow thunks tail-call (signatures not yet known)
BOOL data_ov095_0689c465(u32 condition, BattleCondition value, BattleMon *mon, u16 context, BattleHandlerString *string);
BOOL IsAbilityMultitype(u16 ability);
BOOL IsArceus(u16 species);

#endif // BWPORT_BATTLE_BTL_SERVER_FLOW_H
