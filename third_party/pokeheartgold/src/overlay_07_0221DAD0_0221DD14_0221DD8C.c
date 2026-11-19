#include "global.h"

#include "overlay_07_battle_bg_anim.h"

BattleBgAnim *ov07_0221DD8C(BattleBgAnim **bgAnim, int kind);

BattleBgAnim *ov07_0221DD8C(BattleBgAnim **bgAnim, int kind) {
    u8 i;
    BattleBgAnim *anim;

    i = 0;
loop_1:
    anim = bgAnim[i];
    if (kind == anim->kind) {
        return anim;
    }
    i += 1;
    if ((u32)i >= 4U) {
        return NULL;
    }
    goto loop_1;
}
