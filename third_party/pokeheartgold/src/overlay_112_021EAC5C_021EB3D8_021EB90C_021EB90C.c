#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *arg0);

s32 ov112_021E9C10(Ov112PokewalkerApp *, s32);

void ov112_021EB90C(Ov112PokewalkerApp *arg0, s32 arg1) {
    arg0->unk_4 = 2;
    ov112_021E9C10(arg0, arg1);
    ov112_021E9A78(arg0, 1);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    ov112_021EA570(arg0);
}
