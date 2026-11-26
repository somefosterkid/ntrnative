#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021EB7D8(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    ov112_021EC460(arg0, 2, 0xF, 0x16);
    return 0x18;
}

s32 ov112_021EB838(Ov112PokewalkerApp *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        arg0->unk_4 = 0xB;
        return 0x1A;
    }
    return 0x15;
}

s32 ov112_021EB85C(void *arg0);

s32 ov112_021EB85C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 0xB;
    return 0x1A;
}

s32 ov112_021EB864(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    ov112_021EC460(arg0, 2, 0x16, 0x16);
    return 0x18;
}

s32 ov112_021EB8C4(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x18;
}

s32 ov112_021EB8E8(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (ManagedSprite_IsAnimated(arg0->sprites[0]) == 0) {
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x19;
}

s32 ov112_021EB908(void);

s32 ov112_021EB908(void) {
    return 0x1B;
}
