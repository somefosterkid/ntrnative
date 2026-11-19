#include "global.h"

#include "palette.h"
#include "overlay_07_battle_anim_funcs.h"

s32 ov07_0221BFC0(BattleAnimSystem *system);


s32 ov07_02222F7C(u16 *, u16);

void ov07_02222FC4(BattleAnimSystem *arg0);

void ov07_02222FC4(BattleAnimSystem *arg0) {
    u16 *v0;

    v0 = PaletteData_GetFadedBuf(ov07_0221FA78(arg0), PLTTBUF_MAIN_BG);
    if (ov07_0221BFC0(arg0) == 1) {
        ov07_02222F7C(v0, 0x30);
        return;
    }
    ov07_02222F7C(v0, 0x80);
}

void ov07_02222FF4(BattleAnimSystem *arg0);

void ov07_02222FF4(BattleAnimSystem *arg0) {
    PaletteData *data;

    data = ov07_0221FA78(arg0);
    if (ov07_0221BFC0(arg0) == 1) {
        PaletteData_CopyPalette(data, 0U, 0U, 0U, 0U, 0x60U);
        return;
    }
    PaletteData_CopyPalette(data, 0U, 0U, 0U, 0U, 0x100U);
}
