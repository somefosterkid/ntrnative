#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "system.h"
#include "touchscreen.h"

extern s32 ov18_021FB6F0;

extern TouchscreenHitbox ov18_021FB72C;

int ov18_021F6B00(PokedexAppData *pokedexApp) {
    s32 temp_r0;
    s32 var_r0;

    pokedexApp->unk_0864 = 0;
    if (System_GetTouchNew() == 1) {
        temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov18_021FB72C);
        if (temp_r0 == -1) {
            return -1;
        }
        var_r0 = *(s32 *)(&ov18_021FB6F0 + temp_r0);
        if (var_r0 == 6) {
            if (pokedexApp->unk_1860 == 0) {
                return 5;
            }
            return var_r0;
        }
        return var_r0;
    }
    pokedexApp->unk_0864 = 1;
    if (pokedexApp->unk_1860 == 1) {
        if (gSystem.newKeys & 0x20) {
            if (pokedexApp->unk_1858 == 0) {
                return 3;
            }
            return -1;
        }
        if (gSystem.newKeys & 0x10) {
            if (pokedexApp->unk_1858 == 1) {
                return 4;
            }
            return -1;
        }
    }
    if ((gSystem.newKeys & 1) || (gSystem.newKeys & 8)) {
        return 0;
    }
    if (gSystem.newKeys & 2) {
        return 2;
    }
    if (gSystem.newKeys & 0x800) {
        return 1;
    }
    return -1;
}
