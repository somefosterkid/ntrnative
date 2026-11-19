#include "global.h"

#include "battle/battle.h"
#include "battle/battle_system.h"

#include "overlay_10_trainer_ai.h"
#include "pokemon.h"
#include "pokemon_types_def.h"

s32 ov10_0221EEF0(BattleContext *);

s32 ov10_0221EF24(BattleContext *, s32);

u8 ov10_0221EF34(BattleContext *, u8);

void ov10_0221E018(BattleSystem *battleSys, BattleContext *battleCtx) {
    s32 v0;
    u8 *v1;
    s32 battlerId;
    u8 *new_var;
    BattleContext *battleContext;
    BattleSystem *battleSystem;
    s32 v2;
    s32 v4;
    s32 index;
    u32 v3;
    battleSystem = battleSys;
    battleContext = battleCtx;
    ov10_0221EF24(battleCtx, 1);
    v2 = ov10_0221EEF0(battleContext);
    v0 = ov10_0221EEF0(battleContext);
    battlerId = ov10_0221EF34(battleContext, (u8)v2);
    index = 0;
    if (BattleSystem_GetPartySize(battleSys, battlerId) > 0) {
        Pokemon *mon;
#if defined(SDK_PORT) || defined(NONMATCHING)
        v1 = &battleContext->selectedMonIndex[battlerId];
#else
        v1 = &battleContext->unk_0[battlerId];
#endif
        new_var = (u8 *)v1;
loop_2:
        mon = BattleSystem_GetPartyMon(battleSystem, battlerId, index);

#if defined(SDK_PORT) || defined(NONMATCHING)
        if (index != *new_var) {
#else
        if (index != (*((u8 *)(new_var + 0x219C)))) {
#endif
            v4 = 0;
loop_4:
            v3 = GetMonData(mon, v4 + 0x3A, 0L);

            if (v3 != GetMonData(mon, v4 + 0x42, 0L)) {
                ov10_0221EF24(battleContext, v0);
            } else {
                v4 += 1;
                if (v4 < 4) {
                    goto loop_4;
                }
            }
            if (v4 == 4) {
                goto block_8;
            }
        } else {
block_8:
            index += 1;

            if (index < BattleSystem_GetPartySize(battleSystem, battlerId)) {
                goto loop_2;
            }
        }
    }
}
