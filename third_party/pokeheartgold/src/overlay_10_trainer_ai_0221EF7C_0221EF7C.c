#include "global.h"

#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/trainer_ai.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov10_0221F084(BattleSystem *, BattleContext *, u16, u16, u8 *, s32, s32, s32, u8);
#else
s32 ov10_0221F084(BattleSystem *, BattleContext *, u16, u16, u8 *, s32, s32, s32, s32);
#endif

extern const u16 ov10_0222B080[];

extern const u16 ov10_0222B098[];

s32 ov10_0221EF7C(BattleSystem *battleSys, BattleContext *battleCtx, s32 attacker, u16 *moves, s32 *damageVals, u16 heldItem, u8 *ivs, s32 ability, s32 embargoTurns, s32 varyDamage) {
    s32 sp28;
    s32 sp24;
    u16 *sp1C;
    s32 sp18;
    BattleSystem *sp14;
    s32 *var_r2_2;
    s32 *var_r6;
    s32 temp_r0;
    s32 temp_r1;
    s32 var_r0;
    s32 var_r1_2;
    s32 var_r3;
    const u16 *var_r1;
    const u16 *var_r2;
    u16 temp_r2;
    u16 temp_r3;
    u8 var_r0_2;

    sp14 = battleSys;
    var_r6 = damageVals;
    sp18 = attacker;
    sp1C = moves;
    sp24 = 0;
    sp28 = 0;
    do {
        var_r1 = ov10_0222B098;
        var_r0 = 0;
        temp_r2 = *sp1C;
        temp_r3 = battleCtx->trainerAIData.moveData[temp_r2].effect;
loop_2:
        if (*var_r1 != temp_r3) {
            var_r1 += 1;
            var_r0 += 1;
            if (*var_r1 != 0xFFFF) {
                goto loop_2;
            }
        }
        var_r2 = ov10_0222B080;
        var_r1_2 = 0;
loop_5:
        if (*var_r2 != temp_r3) {
            var_r2 += 1;
            var_r1_2 += 1;
            if (*var_r2 != 0xFFFF) {
                goto loop_5;
            }
        }
        if ((ov10_0222B080[var_r1_2] != 0xFFFF) || ((temp_r2 != 0) && (ov10_0222B098[var_r0] == 0xFFFF) && ((u32)battleCtx->trainerAIData.moveData[temp_r2].power > 1U))) {
            if (varyDamage == 1) {
                var_r0_2 = battleCtx->trainerAIData.unk18[sp28];
            } else {
                var_r0_2 = 0x64;
            }
            *var_r6 = ov10_0221F084(sp14, battleCtx, temp_r2, heldItem, ivs, sp18, ability, embargoTurns, (s32)var_r0_2);
        } else {
            *var_r6 = 0;
        }
        var_r6 += 1;
        sp1C += 1;
        temp_r0 = sp28 + 1;
        sp28 = temp_r0;
    } while (temp_r0 < 4);
    var_r2_2 = damageVals;
    var_r3 = 0;
    do {
        temp_r1 = *var_r2_2;
        if (sp24 < temp_r1) {
            sp24 = temp_r1;
        }
        var_r3 += 1;
        var_r2_2 += 1;
    } while (var_r3 < 4);
    return sp24;
}
