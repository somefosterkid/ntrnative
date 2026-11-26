#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
#include "overlay_112_hitbox.h"

s32 ov112_021EB9C0(void);

s32 ov112_021EB9E4(void);

extern s32 ov112_021FF090[];

s32 ov112_021EB44C(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 temp_r0;
    s32 temp_r1;
    temp_r0 = ov112_021E9888(2);
    if (temp_r0 != (-1)) {
        ov112_021EC440(arg0, temp_r0, ov112_021FF090[temp_r0], 0xE);
        PlaySE(0x5DDU);
        return 0x19;
    }
    if (0x40 & gSystem.newKeys) {
        if ((arg0->unk_1EC50) != 0) {
            PlaySE(0x5DCU);
            arg0->unk_1EC50 -= 1;
        }
        ov112_021EA51C(arg0, ov112_021FF090[arg0->unk_1EC50]);
    } else if (0x80 & gSystem.newKeys) {
        if (((s32)(arg0->unk_1EC50)) < 2) {
            PlaySE(0x5DCU);
            arg0->unk_1EC50 += 1;
        }
        ov112_021EA51C(arg0, ov112_021FF090[arg0->unk_1EC50]);
    } else {
        if (1 & gSystem.newKeys) {
            PlaySE(0x5DDU);
            temp_r1 = arg0->unk_1EC50;
            ov112_021EC440(arg0, temp_r1, ov112_021FF090[temp_r1], 0xE);
            return 0x19;
        }
        if (gSystem.newKeys & 2) {
            PlaySE(0x5DCU);
            ov112_021EC440(arg0, 2, 3, 0xE);
            return 0x19;
        }
        if (ov112_021EB9C0() != 0) {
            PlaySE(0x5DCU);
            ov112_021EB90C(arg0, 3);
            return 0x1A;
        }
        if ((ov112_021EB9E4() != 0) && (ov112_021EB950(arg0, 3) != 0)) {
            PlaySE(0x5DCU);
            return 0x1A;
        }
    }
    return 0xD;
}
