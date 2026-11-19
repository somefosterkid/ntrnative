#include "global.h"

#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/trainer_ai.h"

extern const u16 ov10_0222B080[];

extern const u16 ov10_0222B098[];

void ov10_0221E848(BattleSystem *battleSys, BattleContext *battleCtx);

void ov10_0221E848(BattleSystem *battleSys, BattleContext *battleCtx) {
    s32 sp1C[2];
    s32 sp24;
    u8 sp28[8];
    s32 sp20;
    s32 sp30[4];
    BattleSystem *sp18;
    s32 *var_r2_2;
    s32 temp_r0;
    s32 var_r0;
    s32 var_r1_2;
    s32 var_r3;
    s32 var_r4;
    const u16 *var_r1;
    const u16 *var_r2;
    u16 temp_r3;
    u16 temp_r3_2;
    u8 *var_r5;
    int var_r7;

    sp18 = battleSys;
    ov10_0221EF24(battleCtx, 1);
    sp20 = ov10_0221EEF0(battleCtx);
    temp_r3 = battleCtx->trainerAIData.unk2;
    var_r1 = ov10_0222B098;
    var_r0 = 0;
    temp_r3_2 = battleCtx->trainerAIData.moveData[temp_r3].effect;
loop_1:
    if (temp_r3_2 != *var_r1) {
        var_r1 += 1;
        var_r0 += 1;
        if (*var_r1 != 0xFFFF) {
            goto loop_1;
        }
    }
    var_r2 = ov10_0222B080;
    var_r1_2 = 0;
loop_4:
    if (temp_r3_2 != *var_r2) {
        var_r2 += 1;
        var_r1_2 += 1;
        if (*var_r2 != 0xFFFF) {
            goto loop_4;
        }
    }
    if ((ov10_0222B080[var_r1_2] != 0xFFFF) || (((u32)battleCtx->trainerAIData.moveData[temp_r3].power > 1U) && (ov10_0222B098[var_r0] == 0xFFFF))) {
        var_r7 = battleCtx->trainerAIData.battlerIdAttacker;
        sp1C[0] = 0;
loop_10:
        var_r4 = 0;
        var_r5 = sp28;
        do {
            *var_r5 = (u8)BattleMon_Get(battleCtx, (s32)var_r7, var_r4 + 0xA, NULL);
            var_r4 += 1;
            var_r5 += 1;
        } while (var_r4 < 6);
        ov10_0221EF7C(sp18, battleCtx, (s32)var_r7, battleCtx->battleMons[var_r7].moves, sp30, (u16)(s32)battleCtx->battleMons[var_r7].item, sp28, (s32)GetBattlerAbility(battleCtx, (s32)var_r7), (s32)battleCtx->battleMons[var_r7].unk88.embargoFlag, sp20);
        var_r7 = BattleSystem_GetBattlerIdPartner(sp18, (s32)battleCtx->trainerAIData.battlerIdAttacker);
        if (sp1C[0] == 0) {
            sp24 = (sp30)[battleCtx->trainerAIData.unk1];
        }
        var_r3 = 0;
        var_r2_2 = sp30;
loop_15:
        if ((s32)*var_r2_2 <= sp24) {
            var_r3 += 1;
            var_r2_2 += 1;
            if (var_r3 < 4) {
                goto loop_15;
            }
        }
        if (var_r3 == 4) {
            battleCtx->trainerAIData.unk8 = 2;
        } else {
            battleCtx->trainerAIData.unk8 = 1;
            return;
        }
        temp_r0 = sp1C[0] + 1;
        sp1C[0] = temp_r0;
        if (temp_r0 >= 2) {
            return;
        }
        goto loop_10;
    }
    battleCtx->trainerAIData.unk8 = 0;
}
