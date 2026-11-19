#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

s32 ov108_021E95AC(SafariDecorationApp *arg0) {
    s32 var_r1;
    unsigned char new_var;
    s32 temp_r0;
    s32 temp_r0_2;
    u32 var_r3;
    unsigned char new_var2;
    u8 temp_r5;
    u8 var_r2;
    var_r1 = 0;
    if (0xCF3 & gSystem.newKeys) {
        arg0->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    if (2 & gSystem.newKeys) {
        return ov108_021E9528(arg0, 6U);
    }
    if (1 & gSystem.newKeys) {
        return ov108_021E9528(arg0, arg0->cursor);
    }
    if (!(0xF0 & gSystem.newKeys)) {
        return 0;
    }
    temp_r5 = arg0->cursor;
    var_r2 = temp_r5 % 2;
    var_r3 = ((u32)(temp_r5 << 0x17)) >> 0x18;
    if (0x10 & gSystem.newKeys) {
        if (var_r3 == 3) {
            return 0;
        }
        if (var_r2 == 1) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            new_var = arg0->page;
#else
            new_var = *(s8 *)&arg0->page; // the DS reads page signed here, then reloads it unsigned
#endif
            if ((new_var < ((s32)((arg0->numPages) - 1))) && (var_r3 < 3U)) {
                arg0->page = (u8)((arg0->page) + 1);
                var_r1 = 2;
                goto block_30;
            }
            return 0;
        }
        var_r2 += 1;
        goto block_30;
    }
    if (0x20 & gSystem.newKeys) {
        if (var_r3 == 3) {
            return 0;
        }
        if (var_r2 == 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
            new_var2 = arg0->page;
#else
            new_var2 = *(s8 *)&arg0->page;
#endif
            if ((new_var2 != 0) && (var_r3 < 3U)) {
                arg0->page = (u8)((arg0->page) - 1);
                var_r1 = 1;
                goto block_30;
            }
            return 0;
        }
        var_r2 -= 1;
        goto block_30;
    }
    if (0x40 & gSystem.newKeys) {
        temp_r0 = var_r3 + 3;
        var_r3 = (u32)((u8)(temp_r0 % 4));
    } else if (0x80 & gSystem.newKeys) {
        temp_r0_2 = var_r3 + 1;
        var_r3 = (u32)((u8)(temp_r0_2 % 4));
    }
block_30:
    arg0->cursor = (u8)(var_r2 + (var_r3 * 2));

    if (var_r1 != 0) {
        ov108_021EA584(arg0, (u8)(var_r1 - 1), &arg0->pendingTasks, 0);
        return 0;
    }
    PlaySE(0x5E5U);
    ov108_021EA2EC(arg0, arg0->cursor);
    ov108_021EA47C(arg0);
    return 0;
}
