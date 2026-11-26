#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021EBB84(Ov112PokewalkerApp *arg0) {
    ov112_021EA688(arg0, 8);
    ov112_021EA64C(arg0);
    return 1;
}

s32 ov112_021EBB98(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 3);
    return 1;
}

s32 ov112_021EBBB0(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        ov112_021E966C(arg0, 1);
        return 2;
    }
    return 1;
}

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021EBBD8(Ov112PokewalkerApp *arg0) {
    s32 v0;

    v0 = ov112_021E9750(arg0);
    switch (v0) {
    case 0:
        ov112_021EA570(arg0);
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        return 3;
    case 1:
        arg0->unk_4 = 0xB;
        return 0x22;
    default:
        return 2;
    }
}

s32 ov112_021EBC3C(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 4);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    return 4;
}

s32 ov112_021E7CA4(void *, s32, s32);

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);

s32 ov112_021EBC7C(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        ov112_021E9FA4(arg0, 5, arg0->unk_1E458, 4, 0xF0100);
        ov112_021E7CA4(arg0, 2, 0xA);
        ov112_021E9F40(arg0, 1);
        ov112_021EA51C(arg0, 2);
        return 5;
    }
    return 4;
}
