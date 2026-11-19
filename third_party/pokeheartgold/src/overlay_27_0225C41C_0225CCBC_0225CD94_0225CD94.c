#include "global.h"

#include "overlay_27.h"
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern TouchscreenHitbox ov27_0225D120;

s32 ov27_0225CD94(Ov27TaskData *data) {
    s32 temp_r0;
    Ov27TaskData *var_r4;

    var_r4 = data;
    temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov27_0225D120);
    if (temp_r0 != -1) {
        if (temp_r0 == 0) {
            var_r4->selection = 0;
            ov27_0225CCE0(var_r4, var_r4->selection);
            Sprite_SetAnimCtrlSeq(var_r4->cursor, 3);
            PlaySE(0x5DCU);
            var_r4->state = 5;
        } else if (temp_r0 == 1) {
            var_r4->selection = 1;
            ov27_0225CCE0(var_r4, var_r4->selection);
            Sprite_SetAnimCtrlSeq(var_r4->cursor, 3);
            PlaySE(0x5DCU);
            var_r4->state = 5;
        }
    } else {
        if (0x40 & gSystem.newKeys) {
            if (var_r4->selection == 1) {
                PlaySE(0x5DCU);
            }
            var_r4->selection = 0;
            ov27_0225CCE0(var_r4, var_r4->selection);
        } else if (0x80 & gSystem.newKeys) {
            if (var_r4->selection == 0) {
                PlaySE(0x5DCU);
            }
            var_r4->selection = 1;
            ov27_0225CCE0(var_r4, var_r4->selection);
        } else if (gSystem.newKeys & 1) {
            Sprite_SetAnimCtrlSeq(var_r4->cursor, 3);
            var_r4->state = 5;
            PlaySE(0x5DCU);
        } else if (gSystem.newKeys & 2) {
            var_r4->selection = 1;
            ov27_0225CCE0(var_r4, var_r4->selection);
            Sprite_SetAnimCtrlSeq(var_r4->cursor, 3);
            PlaySE(0x5DCU);
            var_r4->state = 5;
        }
    }
    return 0;
}
