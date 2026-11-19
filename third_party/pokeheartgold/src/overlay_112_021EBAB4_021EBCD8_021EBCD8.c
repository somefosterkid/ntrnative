#include "global.h"

#include "system.h"
#include "unk_02005D10.h"
#include "overlay_112_pokewalker_app.h"
#include "overlay_112_hitbox.h"

s32 ov112_021EA51C(void *, s32);

s32 ov112_021EC440(void *, s32, s32, s32);

s32 ov112_021EBCD8(Ov112PokewalkerApp *arg0, s32 arg1);

s32 ov112_021EBCD8(Ov112PokewalkerApp *arg0, s32 arg1) {
    s32 temp_r1;
    s32 temp_r0;

    temp_r0 = ov112_021E9888(1);
    if (temp_r0 != -1) {
        ov112_021EC440(arg0, temp_r0, temp_r0 + 2, 6);
        PlaySE(0x5DDU);
        return 0x21;
    }
    if (0x40 & gSystem.newKeys) {
        if (arg0->unk_1EC50 != 0) {
            PlaySE(0x5DCU);
        }
        ov112_021EA51C(arg0, 2);
#if defined(SDK_PORT) || defined(NONMATCHING)
        *&arg0->unk_1EC50 = 0;
#else
        *(s32 *)((u32)&arg0->unk_1EC50) = 0;
#endif
    } else if (0x80 & gSystem.newKeys) {
        if (arg0->unk_1EC50 != 1) {
            PlaySE(0x5DCU);
        }
        ov112_021EA51C(arg0, 3);
#if defined(SDK_PORT) || defined(NONMATCHING)
        *&arg0->unk_1EC50 = 1;
#else
        *(s32 *)((u32)&arg0->unk_1EC50) = 1;
#endif
    } else if (1 & gSystem.newKeys) {
        PlaySE(0x5DDU);
        temp_r1 = arg0->unk_1EC50;
        ov112_021EC440(arg0, temp_r1, temp_r1 + 2, 6);
        return 0x21;
    } else if (gSystem.newKeys & 2) {
        PlaySE(0x5DCU);
        ov112_021EC440(arg0, 1, 3, 6);
        return 0x21;
    }
    return 5;
}
