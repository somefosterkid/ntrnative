#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "overlay_96_ov96hurdle_work.h"

s32 ov96_02206C90(Ov96HurdleHud *, void *, s32, s32, void *, s32, s32);

s32 ov96_02206D58(void *, s32, s32, void *, s32);

void ov96_02206DEC(Ov96HurdleHud *arg0, Ov96HurdleRunner *arg1, void *arg2, s32 arg3, s32 arg4, void *arg5, s32 arg6, s32 arg7, s32 arg8);

void ov96_02206DEC(Ov96HurdleHud *arg0, Ov96HurdleRunner *arg1, void *arg2, s32 arg3, s32 arg4, void *arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 var_r2;
    s32 var_r4;
    u8 temp_r2;
    u8 temp_r4;

    temp_r4 = 3 & ((s32)(u8)((u32) * (u32 *)((u8 *)arg2 + 0x10) >> 0x17) >> (arg3 * 2));
    if (arg1->unk_9E != temp_r4) {
        arg1->unk_9E = temp_r4;
        arg1->unk_9F = 0;
    }
    temp_r2 = arg1->unk_9E;
    switch (temp_r2) {
    case 1:
        if ((arg6 != 0) && (arg8 != 0)) {
            var_r4 = 1;
        } else {
            var_r4 = 0;
        }
        ov96_02206C90(arg0, arg1, arg3, *(u8 *)&arg4, arg5, var_r4, arg7);
        return;
    case 2:
        if ((arg6 != 0) && (arg8 != 0)) {
            var_r2 = 1;
        } else {
            var_r2 = 0;
        }
        ov96_02206D58(arg1, arg3, *(u8 *)&arg4, arg5, arg7 | var_r2);
    case 0:
        return;
    }
}
