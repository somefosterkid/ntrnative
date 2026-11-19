#include "global.h"

#include "battle/overlay_12_0224E4FC.h"

s32 ov10_0221EEF0(BattleContext *);

s32 ov10_0221EF24(BattleContext *, s32);

u32 ov10_0221F084(BattleSystem *, BattleContext *, u16, u16, s8 *, s32, s32, u32, u8);

extern const u16 ov10_0222B080[];

extern const u16 ov10_0222B098[];

void ov10_0221D7CC(BattleSystem *arg0, BattleContext *arg1);

void ov10_0221D7CC(BattleSystem *arg0, BattleContext *arg1) {
    s8 sp20[6];
    s32 sp1C;
    s32 sp18;
    BattleSystem *sp14;
    s32 temp_r4;
    s32 var_r0;
    s32 var_r1_2;
    s32 var_r4;
    s8 *var_r5;
    const u16 *var_r1;
    const u16 *var_r2;
    u16 temp_r3;
    u16 temp_r3_2;
    u8 temp_r4_2;
    sp14 = arg0;
    ov10_0221EF24(arg1, 1);
    temp_r4 = ov10_0221EEF0(arg1);
    sp1C = ov10_0221EEF0(arg1);
    if (temp_r4 == 1) {
        sp18 = (s32)arg1->trainerAIData.unk18[arg1->trainerAIData.unk1];
    } else {
        sp18 = 0x64;
    }
    var_r1 = ov10_0222B098;
    temp_r3 = arg1->trainerAIData.unk2;
    var_r0 = 0;
    temp_r3_2 = arg1->trainerAIData.moveData[temp_r3].effect;
loop_4:
    if (temp_r3_2 != (*var_r1)) {
        var_r1 += 1;
        var_r0 += 1;
        if ((*var_r1) != 0xFFFF) {
            goto loop_4;
        }
    }

    var_r2 = ov10_0222B080;
    var_r1_2 = 0;
loop_7:
    if (temp_r3_2 != (*var_r2)) {
        var_r2 += 1;
        var_r1_2 += 1;
        if ((*var_r2) != 0xFFFF) {
            goto loop_7;
        }
    }

    if ((ov10_0222B080[var_r1_2] != 0xFFFF) || ((((u32)arg1->trainerAIData.moveData[temp_r3].power) > 1U) && (ov10_0222B098[var_r0] == 0xFFFF))) {
        var_r4 = 0;
        var_r5 = sp20;
        do {
            *var_r5 = (s8)BattleMon_Get(arg1, (s32)arg1->trainerAIData.battlerIdAttacker, var_r4 + 0xA, 0L);
            var_r4 += 1;
            var_r5 += 1;
        } while (var_r4 < 6);
        temp_r4_2 = arg1->trainerAIData.battlerIdAttacker;
        temp_r3_2 = temp_r4_2;
        var_r4 = GetBattlerAbility(arg1, (s32)temp_r3_2);
        if (arg1->battleMons[arg1->trainerAIData.battlerIdTarget].hp <= ov10_0221F084(sp14, arg1, arg1->trainerAIData.unk2, arg1->battleMons[temp_r4_2].item, sp20, temp_r4_2, var_r4, arg1->battleMons[temp_r3_2].unk88.embargoFlag, (u8)sp18)) {
            ov10_0221EF24(arg1, sp1C);
        }
    }
}
