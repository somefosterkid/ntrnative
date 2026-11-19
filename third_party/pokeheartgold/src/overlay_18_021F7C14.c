#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "system.h"
#include "touchscreen.h"


extern s32 ov18_021FB904;

extern TouchscreenHitbox ov18_021FB934;

u32 ov18_021F7C14(PokedexAppData *pokedexApp, u32 *a1) {
    s32 v0;
    s8 v1;
    u32 v2;

    v2 = ov18_021F7B94(pokedexApp, a1);
    if (v2 == -1U) {
        if (System_GetTouchNew() == 1) {
            v0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov18_021FB934);
            if (v0 == -1) {
                return -1U;
            }
            *a1 = 1;
            return *(s32 *)(&ov18_021FB904 + v0);
        }
        *a1 = 0;
        if (0x40 & gSystem.newAndRepeatedKeys) {
            return 5U;
        }
        if (0x80 & gSystem.newAndRepeatedKeys) {
            return 7U;
        }
        if (1 & gSystem.newKeys) {
            v1 = pokedexApp->unk_18C9;
            if (v1 == 0) {
                return 0xEU;
            }
            if (v1 == 1) {
                return 0xFU;
            }
            return 0xDU;
        }
        if (gSystem.newKeys & 4) {
            if (pokedexApp->unk_18C8 == 0) {
                return 0xCU;
            }
            return 0xBU;
        }
        if (pokedexApp->unk_18C8 == 0) {
            if (gSystem.newKeys & 0x100) {
                return 0xCU;
            }
            goto block_24;
        }
        if (gSystem.newKeys & 0x200) {
            return 0xBU;
        }
block_24:
        if (0x20 & gSystem.newAndRepeatedKeys) {
            return 0x10U;
        }
        if (gSystem.newAndRepeatedKeys & 0x10) {
            if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies) != 0) {
                return 1U;
            }
            return 2U;
        }
        v2 = -1U;
        return v2;
    }
    return v2;
}

u32 ov18_021F7D00(PokedexAppData *pokedexApp, u32 *a1) {
    u32 v0;
    u32 v1;

    v0 = ov18_021F7B94(pokedexApp, a1);
    v1 = -1U;
    if (v0 == -1U) {
        v0 = 0;
        *a1 = 0;
        if (!(0x20 & gSystem.newAndRepeatedKeys)) {
            if (0x10 & gSystem.newAndRepeatedKeys) {
                v1 = 2;
            }
            v0 = v1;
        }
    }
    return v0;
}
