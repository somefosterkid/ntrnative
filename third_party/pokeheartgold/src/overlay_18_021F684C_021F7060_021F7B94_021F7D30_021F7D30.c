#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "system.h"
#include "touchscreen.h"


extern s32 ov18_021FB760;

extern TouchscreenHitbox ov18_021FB804;

u32 ov18_021F7D30(PokedexAppData *pokedexApp, u32 *a1) {
    s32 temp_r0;
    u32 var_r0;

    var_r0 = ov18_021F7B94(pokedexApp, a1);
    if (var_r0 == -1U) {
        if (System_GetTouchNew() == 1) {
            temp_r0 = TouchscreenHitbox_FindRectAtTouchNew(&ov18_021FB804);
            if (temp_r0 == -1) {
                return -1U;
            }
            *a1 = 1;
            return *(s32 *)(&ov18_021FB760 + temp_r0);
        }
        *a1 = 0;
        if (0x40 & gSystem.newAndRepeatedKeys) {
            return 5U;
        }
        if (0x80 & gSystem.newAndRepeatedKeys) {
            return 7U;
        }
        if (0x20 & gSystem.newAndRepeatedKeys) {
            if (Pokedex_CheckMonCaughtFlag(pokedexApp->args->pokedex, pokedexApp->curSpecies) != 0) {
                return 1U;
            }
            return 0U;
        }
        if (gSystem.newAndRepeatedKeys & 0x10) {
            return 0xCU;
        }
        if (gSystem.newKeys & 1) {
            return 0xBU;
        }
        var_r0 = -1U;
        return var_r0;
    }
    return var_r0;
}
