#include "global.h"
#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"
struct Ov112PokewalkerApp;

void ov112_021EC460(Ov112PokewalkerApp *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 uVar1;

    uVar1 = ov112_021EA08C(param_1, param_2, param_3);
    param_1->unk_1E524 = uVar1;
    param_1->unk_C = param_4;
    return;
}

s32 ov112_021EC478(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 4U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 5U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    ov112_021EA688(arg0, 7);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 5U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    arg0->unk_1F2E0 = 0;
    return 2;
}
