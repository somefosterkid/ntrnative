#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);

s32 ov112_021EA570(void *);

s32 ov112_021EC1A8(Ov112PokewalkerApp *arg0);

s32 ov112_021EC1A8(Ov112PokewalkerApp *arg0) {
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x21);
    ov112_021EA570(arg0);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    return 0x13;
}

s32 ov112_021EC1E4(Ov112PokewalkerApp *arg0);

s32 ov112_021EC1E4(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        arg0->unk_4 = 0xB;
        return 0x22;
    }
    return 0x13;
}

s32 ov112_021EC208(Ov112PokewalkerApp *arg0);

s32 ov112_021EC208(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 5U, 0U, 0U, 0U, 0x40U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 5U);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x1F);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    return 0x15;
}

s32 ov112_021EC284(Ov112PokewalkerApp *arg0);

s32 ov112_021EC284(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        arg0->unk_4 = 0;
        return 0x16;
    }
    return 0x15;
}

s32 ov112_021EC2A8(Ov112PokewalkerApp *arg0);

s32 ov112_021EC2A8(Ov112PokewalkerApp *arg0) {
    if ((s32) arg0->unk_1F370 >= 0) {
        BufferBoxMonNickname(arg0->msgFormat, 1U, (void *)&arg0->unk_1F2E8[0]);
        arg0->unk_1E524 = ov112_021EA08C(arg0, 2, 0x1A);
        return 0x17;
    }
    return 0x1D;
}

s32 ov112_021EC2E4(Ov112PokewalkerApp *arg0);

s32 ov112_021EC2E4(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        return 0x18;
    }
    return 0x17;
}

s32 ov112_021E9290(void *, void *, void *, s32);

s32 ov112_021EA688(void *, s32);

s32 ov112_021EC304(Ov112PokewalkerApp *arg0);

s32 ov112_021EC304(Ov112PokewalkerApp *arg0) {
    arg0->unk_1F2D0 = 0;
    ov112_021EA688(arg0, 8);
    ov112_021E9290(arg0->unk_1F2E8, arg0->unk_1D7AC, arg0->unk_1D79C, 0);
    return 0x19;
}
