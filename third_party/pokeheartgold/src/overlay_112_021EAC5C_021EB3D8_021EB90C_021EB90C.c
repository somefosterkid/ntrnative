#include "global.h"

#include "bg_window.h"

void ov112_021EA570(void *arg0);

s32 ov112_021E9A78(void *, u8);

s32 ov112_021E9C10(void *, s32);

void ov112_021EB90C(void *arg0, s32 arg1);

void ov112_021EB90C(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 4) = 2;
    ov112_021E9C10(arg0, arg1);
    ov112_021E9A78(arg0, 1);
    FillBgTilemapRect(*(BgConfig **)((u8 *)arg0 + 0x18), 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(*(BgConfig **)((u8 *)arg0 + 0x18), 1U);
    ov112_021EA570(arg0);
}
