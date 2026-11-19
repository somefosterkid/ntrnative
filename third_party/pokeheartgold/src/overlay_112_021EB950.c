#include "global.h"

#include "bg_window.h"
#include "pokewalker.h"
#include "system.h"
#include "screen_fade.h"

void ov112_021E9A78(u8 *, u8);

void ov112_021E9C10(u8 *, u32);

void ov112_021EA570(u8 *);

u32 ov112_021EB950(u8 *param_1, u32 param_2);

s32 ov112_021EB9C0(void);
s32 ov112_021EB9E4(void);

s32 ov112_021EBA08(void);

u32 ov112_021EB950(u8 *param_1, u32 param_2) {
    u16 uStack_10[2];

    sub_02032688(*(POKEWALKER **)(param_1 + 0x1e440), &uStack_10[1], &uStack_10[0]);
    if (uStack_10[1] != 2) {
        return 0;
    }
    *(u32 *)(param_1 + 4) = 3;
    *(u32 *)(param_1 + 0x10) = 4;
    ov112_021E9C10(param_1, param_2);
    ov112_021E9A78(param_1, 1);
    FillBgTilemapRect(*(BgConfig **)(param_1 + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(BgConfig **)(param_1 + 0x18), 1);
    ov112_021EA570(param_1);
    return 1;
}

s32 ov112_021EB9C0(void) {
    if ((gSystem.heldKeys & 0x80) && (0x400 & gSystem.heldKeys) && (0x200 & gSystem.heldKeys)) {
        return 1;
    }
    return 0;
}

s32 ov112_021EB9E4(void) {
    if ((0x40 & gSystem.heldKeys) && (gSystem.heldKeys & 4) && (0x100 & gSystem.heldKeys)) {
        return 1;
    }
    return 0;
}

s32 ov112_021EBA08(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_154);
    return 2;
}
