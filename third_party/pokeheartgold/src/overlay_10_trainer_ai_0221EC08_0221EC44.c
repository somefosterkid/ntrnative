#include "global.h"
#include "battle/battle.h"

s32 ov10_0221EE60(BattleSystem *, void *);

s32 ov10_0221EF24(void *, s32);

void ov10_0221EC44(void *arg0, BattleContext *arg1);

void ov10_0221EC44(void *arg0, BattleContext *arg1) {
    ov10_0221EF24(arg1, 1);
    if (ov10_0221EE60(arg0, arg1) != 1) {
        arg1->trainerAIData.unk10 = (u8)(1 | arg1->trainerAIData.unk10);
    }
}
