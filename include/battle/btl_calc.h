// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_calc.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_CALC_H
#define BWPORT_BATTLE_BTL_CALC_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/math.h"
#include "struct_decls.h"
#include "battle/btl_pokeparam.h"
#include "pml/waza.h"

u32 BattleRandom(u32 max);
u16 GetBoostFromStatStage(u16 value, u8 stage);
u32 GetTypeEffectiveness(u8 attackType, u8 defenseType);
u32 GetTypeEffectivenessMultiplier(u32 effectiveness1, u32 effectiveness2);
u32 CalcBaseDamage(u32 power, u32 attack, u32 level, u32 defense);
u8 GetTypeWeaknesses(u8 attackType, u8 *defenseTypes);
BOOL RollEffectChance(u32 chance);
u32 fixed_round(u32 value, u32 ratio);
u32 GetRatioOverZero(u32 value, u32 ratio);
u32 MultiplyValueByRatio(u32 value, u32 ratio);
u32 DivideMaxHp(BattleMon *mon, u32 divisor);
u32 DivideMaxHPZeroCheck(BattleMon *mon, u32 divisor);
BOOL IsBasicStatus(s32 condition);
u32 ItemGetParam(u16 item, u16 param);
u8 DecideMoveTargetAutoForClient(BtlMainModule *mainModule, BtlPokeCon *pokeCon, BattleMon *mon, u16 move,
                                 MATHRandContext32 *saved);

BattleCondition func_ov093_021d7d18(u32 status);

// Up to four client IDs without repeats, the unused ones 4
struct BtlClientIDList {
    u32 count;
    u8 clientIds[4];
};

void func_ov093_021d7840(const MATHRandContext32 *rand, HeapID heapId);
u32 func_ov093_021d7f88(BtlSetup *setup);
void func_ov093_021d8230(BtlClientIDList *list);
void func_ov093_021d7894(void);
u32 func_ov093_021d7ff8(u32 index, BattleParty *party);
BOOL func_ov093_021d7f60(u16 trainerClass);
u32 func_ov093_021d8270(BtlClientIDList *list, u8 clientId, u32 value);
void func_ov093_021d787c(const MATHRandContext32 *rand);

u32 func_ov093_021d7ad4(s32 value);
void func_ov093_021d7dc0(s32 condition, BattleMon *mon, BattleCondition *out);
s32 func_ov093_021d799c(u8 attackType, PokeTypePair defenseTypes);
s32 func_ov093_021d7b08(s32 value, s32 minimum);

u32 func_ov093_021d80a8(BattleMon *mon);
BattleCondition func_ov093_021d7d78(u32 turns);
u16 func_ov093_021d7e44(const u16 *excluded, u16 count);

u8 GetNumMonsOnField(u32 battleStyle, u8 count);
BOOL func_ov093_021d7ef0(u32 value);
void func_ov093_021d8090(u32 packed, u8 *values);

u32 TypeEffectivenessPowerMod(u32 damage, u32 effectiveness);
fx32 WeatherPowerMod(u32 weather, u32 moveType);
void func_ov093_021d8244(BtlClientIDList *list, u8 clientId);
u8 func_ov093_021d80bc(BtlMainModule *mainModule, BtlPokeCon *pokeCon, BattleMon *mon, u16 move);
u8 func_ov093_021d7908(u32 value, u32 stage);
void func_ov093_021d7c70(MoveConditionParam param, BattleMon *mon, BattleCondition *out);
u32 func_ov093_021d806c(const u8 *values);
u32 func_ov093_021d7bd4(BattleMon *mon, u32 weather);
u32 func_ov093_021d7e10(BattleMon *mon);
BOOL func_ov093_021d7930(u8 critStage);
u8 func_ov093_021d7ba4(u8 hits);

u32 func_ov093_021d7f00(u32 value);

u32 func_ov093_021d7b8c(u32 min, u32 max);
BOOL func_ov093_021d7e90(const u16 *moves, u32 count, u16 move);
u32 func_ov093_021d7fbc(BtlSetupTrainer *trainer, PokeParty *party);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 func_ov093_021d826c(BtlClientIDList *list);
BattleCondition func_ov093_021d7d9c(u32 turns);
BOOL func_ov093_021d7f4c(u32 trainerClass);
void func_ov093_021d804c(u16 terminator);
void func_ov093_021d8058(void);

#endif // BWPORT_BATTLE_BTL_CALC_H
