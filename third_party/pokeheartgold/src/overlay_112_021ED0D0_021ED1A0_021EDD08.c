#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021EDCB0(s32);

s32 ov112_021EDD08(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    arg0->unk_14 = ov112_021EDCB0(arg0->unk_14);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, arg0->unk_14);
    return 0x22;
}

s32 ov112_021EDD30(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    arg0->unk_14 = ov112_021EDCB0(arg0->unk_14);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, arg0->unk_14);
    arg0->unk_C = 0x42;
    return 0x22;
}

s32 ov112_021EDD5C(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 7);
    return 0x11;
}

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021EDD80(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    return 0x15;
}

s32 ov112_021EDDB4(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    return 2;
}

s32 ov112_021EDDE8(Ov112PokewalkerApp *arg0) {
    BgClearTilemapBufferAndCommit(arg0->bgConfig, 1U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 5);
    ov112_021E7CA4(arg0, 2, 0xE);
    ov112_021E9F40(arg0, 1);
    return 0x17;
}

s32 ov112_021EDE1C(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (TextPrinterCheckActive((u8) arg0->unk_1E524) == 0) {
        arg0->unk_4 = 0;
        FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x22;
}

s32 ov112_021E7464(void);

s32 ov112_021EDE68(Ov112PokewalkerApp *arg0) {
    Pokemon *mon;

    ov112_021E7464();
    if (ov112_021ED640(arg0) == 0) {
        MIi_CpuClearFast(0U, &arg0->unk_AD00, 0x2FCU);
        arg0->unk_AABC = (u8)(arg0->unk_AABC & ~0x20);
    }
    mon = arg0->unk_1E42C;
    if (mon != NULL) {
        BufferBoxMonNickname(arg0->msgFormat, 1U, &mon->box);
    } else {
        BufferBoxMonNickname(arg0->msgFormat, 1U, arg0->unk_1E430);
    }
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x18);
    PlayFanfare(0x4A2U);
    return 0x24;
}
