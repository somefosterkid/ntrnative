#include "global.h"

#include "overlay_10_trainer_ai.h"
#include "battle/battle.h"

s32 ov10_0221E74C(BattleContext *, s32, s32 *, s32 *, s32);

int ov10_0221EEF0(BattleContext *);

s32 ov10_0221EF24(BattleContext *, int);

u8 ov10_0221EF34(BattleContext *, u8);

void ov10_0221E650(s32 arg0, BattleContext *arg1) {
    s32 v1;
    s32 v0;
    int v2;
    int v3;
    int v4;

    ov10_0221EF24(arg1, 1U);
    v2 = ov10_0221EEF0(arg1);
    v3 = ov10_0221EEF0(arg1);
    v4 = ov10_0221EEF0(arg1);
    ov10_0221E74C(arg1, ov10_0221EF34(arg1, v2), &v1, &v0, v3);
    if (v1 < v0) {
        ov10_0221EF24(arg1, v4);
    }
}

void ov10_0221E6A4(s32 arg0, BattleContext *arg1) {
    s32 v1;
    s32 v0;
    int v2;
    int v3;
    int v4;

    ov10_0221EF24(arg1, 1U);
    v2 = ov10_0221EEF0(arg1);
    v3 = ov10_0221EEF0(arg1);
    v4 = ov10_0221EEF0(arg1);
    ov10_0221E74C(arg1, ov10_0221EF34(arg1, v2), &v1, &v0, v3);
    if (v1 > v0) {
        ov10_0221EF24(arg1, v4);
    }
}

void ov10_0221E6F8(s32 arg0, BattleContext *arg1) {
    s32 v1;
    s32 v0;
    int v2;
    int v3;
    int v4;

    ov10_0221EF24(arg1, 1U);
    v2 = ov10_0221EEF0(arg1);
    v3 = ov10_0221EEF0(arg1);
    v4 = ov10_0221EEF0(arg1);
    ov10_0221E74C(arg1, ov10_0221EF34(arg1, v2), &v1, &v0, v3);
    if (v1 == v0) {
        ov10_0221EF24(arg1, v4);
    }
}
