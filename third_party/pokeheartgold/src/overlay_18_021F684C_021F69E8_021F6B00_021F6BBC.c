#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

extern s32 ov18_021FB84C;

extern TouchscreenHitbox ov18_021FB8A4;

int ov18_021F6BBC(PokedexAppData *pokedexApp, int a1) {
    u32 sp4;
    u32 sp0;
    s32 temp_r0;
    u32 temp_r4;
    u32 temp_r0_2;
    u8 temp_r0_3;
    u8 temp_r1;
    u8 temp_r1_2;
    u32 temp_r1_3;
    u8 temp_r2;

    pokedexApp->unk_0864 = 0;
    if (System_GetTouchNewCoords(&sp4, &sp0) == 1) {
        temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov18_021FB8A4);
        if (temp_r0 == -1) {
            return -1;
        }
        temp_r4 = *(s32 *)(((u8 *)&ov18_021FB84C) + (temp_r0 * 4));
        if (temp_r4 == 0) {
            temp_r0_2 = sp4 - 0x1B;
            sp0 -= 4;
            sp4 = temp_r0_2;
            temp_r2 = (temp_r0_2 / 40U) + ((sp0 / 40U) * 5);
            if (temp_r2 == pokedexApp->unk_185A) {
                return 4;
            }
            pokedexApp->unk_185A = temp_r2;
            PlaySE(0x8E9U);
        } else if (temp_r4 == 14) {
            PlaySE(0x8F2U);
        } else if (temp_r4 == 2) {
            PlaySE(0x8E9U);
        } else if (temp_r4 == 5) {
            PlaySE(0x940U);
        }
        return temp_r4;
    } else {
        pokedexApp->unk_0864 = 1;
        if (0x40 & gSystem.newAndRepeatedKeys) {
            temp_r1 = pokedexApp->unk_185A;
            if ((u32)temp_r1 < 5U) {
                return 0xA;
            }
            pokedexApp->unk_185A = temp_r1 - 5;
            PlaySE(0x8E8U);
            return 0;
        }
        if (0x80 & gSystem.newAndRepeatedKeys) {
            temp_r0_3 = pokedexApp->unk_185A;
            if (((u32)temp_r0_3 >= 0xAU) && ((u32)temp_r0_3 < 0xFU)) {
                return 0xC;
            }
            pokedexApp->unk_185A += 5;
            PlaySE(0x8E8U);
            return 0;
        }
        if (gSystem.newAndRepeatedKeys & 0x20) {
            temp_r1_2 = pokedexApp->unk_185A;
            if (temp_r1_2 != 0) {
                pokedexApp->unk_185A = temp_r1_2 - 1;
                PlaySE(0x8E8U);
                return 0;
            }
            if (pokedexApp->unk_1859 != 0) {
                pokedexApp->unk_185A = 0xE;
                return 9;
            }
            return -1;
        }
        if (0x10 & gSystem.newAndRepeatedKeys) {
            temp_r1_3 = pokedexApp->unk_185A + 1;
            if (temp_r1_3 != 0xF) {
                pokedexApp->unk_185A = temp_r1_3;
                PlaySE(0x8E8U);
                return 0;
            }
            if ((u32)(pokedexApp->unk_1859 + 1) <= ov18_021F8950(pokedexApp, a1)) {
                pokedexApp->unk_185A = 0;
                return 0xB;
            }
            return -1;
        }
        if (gSystem.newKeys & 1) {
            return 4;
        }
        if (gSystem.newKeys & 2) {
            PlaySE(0x940U);
            return 6;
        }
        if (0x400 & gSystem.newKeys) {
            return 3;
        }
        if (0x800 & gSystem.newKeys) {
            PlaySE(0x8E9U);
            return 2;
        }
        if (0x200 & gSystem.newAndRepeatedKeys) {
            return 9;
        }
        if (gSystem.newAndRepeatedKeys & 0x100) {
            return 0xB;
        }
        if (4 & gSystem.newKeys) {
            return 8;
        }
        if (gSystem.newKeys & 8) {
            PlaySE(0x8F2U);
            return 1;
        }
        return -1;
    }
}
