#include "global.h"

#include "overlay_07_battle_anim_system.h"

extern u32 ov07_02234B8C;

u32 ov07_0221FAEC(void *param_1, s32 param_2);

s32 ov07_0221FAE8(BattleAnimSystem *system);
s32 ov07_0221FB04(BattleAnimSystem *system, u32 bg);

u32 ov07_0221FAEC(void *param_1, s32 param_2) {
    return *(u32 *)(&ov07_02234B8C + param_2);
}

extern u32 _02234B80;

u32 ov07_0221FAF8(void *param_1, s32 param_2);

u32 ov07_0221FAF8(void *param_1, s32 param_2) {
    return *(u32 *)(&_02234B80 + param_2);
}

s32 ov07_0221FB04(BattleAnimSystem *system, u32 bg) {
    switch (bg) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 3;
    case 3:
        return ov07_0221FAE8(system);
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return 0; // the DS build falls off the end here and returns the (pointer) arg
#endif
}
