#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021EE0F4(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x14);
    return 0x28;
}

s32 ov112_021EE10C(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x3E;
    }
    return 0x28;
}

s32 ov112_021E7464(void);

s32 ov112_021EE12C(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x1D);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    ov112_021E7464();
    PlayFanfare(0x4A1U);
    return 0x2A;
}

s32 ov112_021EE178(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) && (IsFanfarePlaying() == 0)) {
        arg0->unk_4 = 6;
        return 0x42;
    }
    return 0x2A;
}

s32 ov112_021EE1A4(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x12);
    return 0x2C;
}

s32 ov112_021EE1BC(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x2D;
    }
    return 0x2C;
}

s32 ov112_021EE1DC(Ov112PokewalkerApp *arg0) {
    ov112_021E966C(arg0, 1);
    return 0x2E;
}

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021EE1E8(Ov112PokewalkerApp *arg0) {
    s32 v0;

    v0 = ov112_021E9750(arg0);
    switch (v0) {
    case 0:
        ov112_021EA570(arg0);
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
        return 0x2F;
    case 1:
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        ov112_021EA570(arg0);
        arg0->unk_4 = 0;
        return 0x42;
    default:
        return 0x2E;
    }
}

s32 ov112_021EE29C(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x13);
    return 0x30;
}

s32 ov112_021EE2B4(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x31;
    }
    return 0x30;
}

s32 ov112_021E7CA4(void *, s32, s32);

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);

s32 ov112_021EE2D4(Ov112PokewalkerApp *arg0) {
    ov112_021E7CA4(arg0, 2, 0xA);
    ov112_021E9FA4(arg0, 5, arg0->unk_1E468, 4, 0xF0100);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1EC50 = 0;
    ov112_021EA51C(arg0, 2);
    return 0x32;
}
