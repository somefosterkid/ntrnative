#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_bg_anim.h"

extern s32 _02237840;

void ov07_0221DD38(BattleBgAnim **bgAnim, BattleBgAnim *anim, int kind) {
    BattleBgAnim *temp_r0;
    u8 var_r2;
    u8 var_r4;
    var_r4 = 0;
    do {
        temp_r0 = bgAnim[var_r4];
        if ((temp_r0 != 0L) && (kind == temp_r0->kind)) {
            GF_AssertFail();
        }
        var_r4 += 1;
    } while (((u32)var_r4) < 4U);
    var_r2 = 0;
loop_6:
    if (bgAnim[var_r2] == 0) {
        bgAnim[var_r2] = anim;
        bgAnim[var_r2]->kind = kind;
        return;
    }
    var_r2 += 1;
    if (((u32)var_r2) >= 4U) {
        void *sym = &_02237840;
        if (sym != 0L) {
            GF_AssertFail();
        }
        return;
    }
    goto loop_6;
}
