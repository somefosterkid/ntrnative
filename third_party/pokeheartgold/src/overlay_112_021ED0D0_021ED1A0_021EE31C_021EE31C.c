#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
#include "overlay_112_hitbox.h"

s32 ov112_021EA51C(void *, s32);

s32 ov112_021EC440(void *, s32, s32, s32);

s32 ov112_021EE31C(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2);

s32 ov112_021EE31C(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2) {
    int temp_r0;
    Ov112PokewalkerApp *new_var;
    temp_r0 = ov112_021E9888(1);
    if (temp_r0 != (-1)) {
        ov112_021EC440(arg0, temp_r0, temp_r0 + 2, 0x33);
        PlaySE(0x5DDU);
        return 0x41;
    }
    new_var = arg0;
    if (0x40 & gSystem.newKeys) {
        ov112_021EA51C(new_var, 2);
        if (new_var->unk_1EC50 != 0) {
            PlaySE(0x5DCU);
        }
        new_var->unk_1EC50 = 0;
    } else if (0x80 & gSystem.newKeys) {
        ov112_021EA51C(arg0, 3);
        if (new_var->unk_1EC50 != 1) {
            PlaySE(0x5DCU);
        }
        arg0->unk_1EC50 = 1;
    } else if (gSystem.newKeys & 1) {
        s32 temp_r1 = arg0->unk_1EC50;
        ov112_021EC440(arg0, temp_r1, temp_r1 + 2, 0x33);
        PlaySE(0x5DDU);
        return 0x41;
    } else if (gSystem.newKeys & 2) {
        ov112_021EC440(arg0, 1, 3, 0x33);
        PlaySE(0x5DCU);
        return 0x41;
    }
    return 0x32;
}
