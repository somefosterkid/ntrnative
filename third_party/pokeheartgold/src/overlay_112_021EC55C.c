#include "global.h"

#include "bg_window.h"
#include "screen_fade.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021E7CA4(void *, s32, s32);
s32 ov112_021EC69C(void);
s32 ov112_021EC6C0(void *arg0);
s32 ov112_021EC6D8(void);
s32 ov112_021EC6F8(void);
s32 ov112_021E9C10(Ov112PokewalkerApp *, s32);

s32 ov112_021EC55C(Ov112PokewalkerApp *arg0) {
    s32 v0;

    v0 = arg0->unk_10;
    if (v0 == 1) {
        arg0->unk_C = 1;
        return 0x13;
    } else if (v0 == 2) {
        arg0->unk_C = 0x16;
        return 0x13;
    } else if (v0 == 4) {
        return 0x2B;
    } else {
        if (arg0->unk_1D77C != 0) {
            return 5;
        }
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
        FillBgTilemapRect(arg0->bgConfig, 4U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        FillBgTilemapRect(arg0->bgConfig, 5U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 5U);
        ov112_021EA688(arg0, 3);
        ov112_021EA688(arg0, 4);
        ov112_021EC460(arg0, 2, 0xB, 3);
        return 0x40;
    }
}

s32 ov112_021EC640(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    ov112_021ED2F4(arg0);
    return 2;
}

s32 ov112_021EC67C(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x1A;
    }
    return 2;
}

s32 ov112_021EC69C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 4;
}

s32 ov112_021EC6C0(void *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        *(s32 *)((u8 *)arg0 + 4) = 7;
        return 0x42;
    }
    return 4;
}

s32 ov112_021EC6D8(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_154);
    return 6;
}

s32 ov112_021EC6F8(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 7;
    }
    return 6;
}

s32 ov112_021EC70C(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0xC);
    return 8;
}

s32 ov112_021EC778(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        ov112_021E9C10(arg0, 1);
        ov112_021E9C10(arg0, 0);
        ov112_021E9A78(arg0, 3);
        ov112_021E9A78(arg0, 2);
        return 9;
    }
    return 8;
}

s32 ov112_021EC7B8(Ov112PokewalkerApp *arg0) {
    return ((s32)(arg0->unk_1EC76 - 1) / 5) + 1;
}
