#include "global.h"

#include "bg_window.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021EA570(Ov112PokewalkerApp *arg0);
#else
void ov112_021EA570(Ov112PokewalkerApp *arg0);
#endif

s32 ov112_021EBD9C(Ov112PokewalkerApp *arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    ov112_021EA570(arg0);
#else
    ov112_021EA570(arg0);
#endif
    ClearWindowTilemapAndCopyToVram(&arg0->windows[5]);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
    if (arg0->unk_1EC50 == 0) {
        return 0xB;
    }
    arg0->unk_4 = 0xB;
    return 0x22;
}

s32 ov112_021EBDE8(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 9);
    return 8;
}

s32 ov112_021EBE4C(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 9;
    }
    return 8;
}

s32 ov112_021EBE6C(Ov112PokewalkerApp *arg0) {
    ov112_021E96D0(arg0, 1);
    return 0xA;
}

s32 ov112_021EBE78(Ov112PokewalkerApp *arg0) {
    s32 v0;

    v0 = ov112_021E9750(arg0);
    switch (v0) {
    case 0:
        ov112_021EA570(arg0);
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        arg0->unk_1F2E4 = 0;
        return 0xB;
    case 1:
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0x10U, 0x20U, 8U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        ov112_021EA570(arg0);
        arg0->unk_1F2E4 = 1;
        return 0xB;
    default:
        return 0xA;
    }
}

s32 ov112_021EBF18(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 5U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 2U);
    ov112_021EC460(arg0, 2, 0x10, 0xD);
    return 0xC;
}

s32 ov112_021EBF4C(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        v0 = (s32)arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0xC;
}

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021EBF70(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    ov112_021ED2F4(arg0);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    return 0xE;
}

s32 ov112_021EBFC8(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) && (arg0->unk_1F2E0 == 0x100)) {
        ov112_021EA5A4(arg0, 2);
        return 0xF;
    }
    return 0xE;
}
