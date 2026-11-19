#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

s32 ov112_021E9A78(void *, u8);

s32 ov112_021E9C10(void *, s32);

u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);

s32 ov112_021EA688(void *, s32);

s32 ov112_021ECE28(Ov112PokewalkerApp *arg0);

s32 ov112_021ECE28(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    ov112_021E9C10(arg0, 2);
    ov112_021E9C10(arg0, 3);
    ov112_021E9A78(arg0, 1);
    ov112_021E9A78(arg0, 0);
    BufferPokewalkerCourseName(arg0->msgFormat, 0U, (u32)(u8) arg0->unk_1D798);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x15);
    return 0xD;
}

s32 ov112_021ECECC(Ov112PokewalkerApp *arg0);

s32 ov112_021ECECC(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0xE;
    }
    return 0xD;
}

s32 ov112_021E966C(Ov112PokewalkerApp *, s32);

s32 ov112_021ECEEC(void *arg0);

s32 ov112_021ECEEC(void *arg0) {
    ov112_021E966C(arg0, 0);
    return 0xF;
}

s32 ov112_021E9338(struct Ov112PokewalkerApp *);

s32 ov112_021E9750(u8 *data);

s32 ov112_021EA570(void *);

s32 ov112_021ECEF8(void *arg0);

s32 ov112_021ECEF8(void *arg0) {
    s32 v0;

    v0 = ov112_021E9750(arg0);
    switch (v0) {
    case 0:
        ov112_021EA570(arg0);
        if (ov112_021E9338(arg0) == 1) {
            *(s32 *)((u8 *)arg0 + 0xC) = 0x14;
            return 0x12;
        }
        *(s32 *)((u8 *)arg0 + 0xC) = 0x10;
        return 0x12;
    case 1:
        ov112_021EA570(arg0);
        return 7;
    default:
        return 0xF;
    }
}

s32 ov112_021ED2F4(Ov112PokewalkerApp *);

s32 ov112_021ECF38(Ov112PokewalkerApp *arg0);

s32 ov112_021ECF38(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 5U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 2U);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    ov112_021ED2F4(arg0);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 7);
    return 0x11;
}

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021E9F40(void *, s32);

s32 ov112_021ECF88(Ov112PokewalkerApp *arg0);

s32 ov112_021ECF88(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) && (arg0->unk_1F2D6 == 0x1F) && (arg0->unk_1F2E0 == 0x100)) {
        ov112_021E7CA4(arg0, 2, 0xE);
        ov112_021E9F40(arg0, 1);
        return 0x18;
    }
    return 0x11;
}

s32 ov112_021EC460(Ov112PokewalkerApp *, s32, s32, s32);

s32 ov112_021ED0D0(Ov112PokewalkerApp *);

s32 ov112_021ECFD4(Ov112PokewalkerApp *arg0);

s32 ov112_021ECFD4(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 5U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 2U);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    ov112_021ED0D0(arg0);
    ov112_021EC460(arg0, 2, 0x10, arg0->unk_C);
    return 0x40;
}

s32 ov112_021ED020(Ov112PokewalkerApp *arg0);

s32 ov112_021ED020(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 4U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 5U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 2U);
    ov112_021EC460(arg0, 2, 0x10, arg0->unk_C);
    return 0x40;
}

s32 ov112_021ED054(Ov112PokewalkerApp *arg0);

s32 ov112_021ED054(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    ov112_021ED2F4(arg0);
    return 0x15;
}

s32 ov112_021ED090(Ov112PokewalkerApp *arg0);

s32 ov112_021ED090(Ov112PokewalkerApp *arg0) {
    if ((TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) && (arg0->unk_1F2D6 == 0x1F) && (arg0->unk_1F2E0 == 0x100)) {
        return 0x1E;
    }
    return 0x15;
}
