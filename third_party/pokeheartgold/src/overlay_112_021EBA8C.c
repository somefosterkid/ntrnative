#include "global.h"

#include "system.h"
#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EBA8C(void);

int ov112_021EBAB0(void);

s32 ov112_021EBA8C(void) {
    if ((1 & gSystem.newKeys) || (gSystem.touchNew != 0)) {
        return 3;
    }
    return 2;
}

int ov112_021EBAB0(void) {
    return sizeof(u32);
}

s32 ov112_021EBAB4(Ov112PokewalkerApp *arg0) {
    FillBgTilemapRect(arg0->bgConfig, 4U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 5U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    ov112_021EA64C(arg0);
    ov112_021EA688(arg0, 7);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 4U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 5U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    return 2;
}
