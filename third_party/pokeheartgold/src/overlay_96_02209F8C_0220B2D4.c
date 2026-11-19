#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

static s32 ov96_0220B2D4(void *arg0);

static s32 ov96_0220B2D4(void *arg0) {
    s16 v0;

    *(s16 *)((u8 *)arg0 + 0x10) = (s16)(*(s16 *)((u8 *)arg0 + 0x10) + ((s32) * (s32 *)((u8 *)arg0 + 8) >> 0xC));
    *(s32 *)((u8 *)arg0 + 8) = (s32)((s32)(*(s32 *)((u8 *)arg0 + 8) * 4) / 3);
    *(s16 *)((u8 *)arg0 + 0x12) = (s16)(*(s16 *)((u8 *)arg0 + 0x12) - ((s32) * (s32 *)((u8 *)arg0 + 0xC) >> 0xC));
    *(s32 *)((u8 *)arg0 + 0xC) = (s32)(*(s32 *)((u8 *)arg0 + 0xC) - 4);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)arg0, *(s16 *)((u8 *)arg0 + 0x10), *(s16 *)((u8 *)arg0 + 0x12));
    v0 = *(s16 *)((u8 *)arg0 + 0x10);
    if (((s32)v0 < 0) || ((s32)v0 > 0x100)) {
        return 1;
    }
    return 0;
}

void ov96_0220B324(u32 *param_1);

void ov96_0220B324(u32 *param_1) {
    s32 iVar1;
    s32 iVar2;

    iVar2 = 0;
    do {
        if ((param_1[1] == 1) && (iVar1 = ov96_0220B2D4(param_1), iVar1 != 0)) {
            ManagedSprite_SetDrawFlag((ManagedSprite *)*param_1, 0);
            param_1[1] = 0;
        }
        iVar2 = iVar2 + 1;
        param_1 = param_1 + 5;
    } while (iVar2 < 0x10);
    return;
}

void ov96_0220B354(u32 *param_1);

void ov96_0220B354(u32 *param_1) {
    s32 iVar1;

    iVar1 = 0;
    do {
        if (param_1[1] == 1) {
            ManagedSprite_SetAnimateFlag((ManagedSprite *)*param_1, 0);
        }
        iVar1 = iVar1 + 1;
        param_1 = param_1 + 5;
    } while (iVar1 < 0x10);
    return;
}
