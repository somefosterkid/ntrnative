#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "system.h"
#include "touchscreen.h"

extern s32 ov18_021FB7C0;

extern TouchscreenHitbox ov18_021FB7E0;

extern TouchscreenHitbox ov18_021FB630;

u32 ov18_021F7DCC(PokedexAppData *pokedexApp, u32 *a1) {
    s32 v0;
    u32 v1;

    if (System_GetTouchNew() == 1) {
        v0 = TouchscreenHitbox_FindRectAtTouchNew(&ov18_021FB7E0);
        if (v0 == -1) {
            return -1U;
        }
        *a1 = 1;
        return *(s32 *)(&ov18_021FB7C0 + v0);
    }
    v1 = 0;
    *a1 = 0;
    if (0x40 & gSystem.newAndRepeatedKeys) {
        if (pokedexApp->unk_18C7_0 != 0) {
            return 2U;
        }
        return v1;
    }
    if (0x80 & gSystem.newAndRepeatedKeys) {
        if (pokedexApp->unk_18C7_0 == 0) {
            return 1U;
        }
        return 3U;
    }
    if (0x20 & gSystem.newKeys) {
        return 6U;
    }
    if (gSystem.newKeys & 0x10) {
        return 7U;
    }
    if (0x400 & gSystem.newKeys) {
        return 4U;
    }
    if (gSystem.newKeys & 3) {
        return 5U;
    }
    v1 = -1U;
    return v1;
}

u32 ov18_021F7E70(PokedexAppData *pokedexApp, u32 *a1) {
    u32 v0;

    v0 = ov18_021F7B94(pokedexApp, a1);
    if (v0 == -1U) {
        if (System_GetTouchNew() == 1) {
            if (TouchscreenHitbox_FindHitboxAtTouchNew(&ov18_021FB630) == -1) {
                return -1U;
            }
            *a1 = 1;
            return 4U;
        }
        v0 = 0;
        *a1 = 0;
        if (0x20 & gSystem.newAndRepeatedKeys) {
            return 2U;
        }
        if (!(0x10 & gSystem.newAndRepeatedKeys)) {
            if (gSystem.newKeys & 1) {
                return 3U;
            }
            v0 = -1U;
            return v0;
        }
        return v0;
    }
    return v0;
}
