#include "global.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *);

s32 ov112_021EE3E0(Ov112PokewalkerApp *arg0) {
    ov112_021EA570(arg0);
    if (arg0->unk_1EC50 == 0) {
        FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[3]);
        ClearWindowTilemapAndCopyToVram(&arg0->windows[4]);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
        arg0->unk_1EC7C = 0;
        return 0x34;
    }
    arg0->unk_4 = 0xB;
    return 0x42;
}
