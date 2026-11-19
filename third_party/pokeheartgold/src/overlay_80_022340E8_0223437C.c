#include "global.h"

#include "frontier/overlay_80_022340E8.h"

s32 ov80_02234390(ArcadeContext *ctx);

s32 ov80_02234424();

void BattleArcadeData_Init(void *a0, u32 a1) {
    if (a1 == 0) {
        ov80_02234390(a0);
        return;
    }
    ov80_02234424();
}
