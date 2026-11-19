#include "global.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F14FC(PokedexAppData *, u32, s32, s32);

void ov18_021F5FFC(PokedexAppData *arg0, s32 arg1, s32 arg2);

void ov18_021F5FFC(PokedexAppData *arg0, s32 arg1, s32 arg2) {
    s32 v1;
    u8 v0;

    v0 = arg0->seenForms[arg2];
    if (v0 & 0x80) {
        v1 = 0x80 ^ v0;
        if (arg0->curSpecies == 0xAC) {
            if (v1 == 2) {
                v1 = 1;
            } else {
                v1 = 0;
            }
        }
    } else {
        v1 = 0;
    }
    ov18_021F14FC(arg0, arg0->curSpecies, v1, arg1);
}
