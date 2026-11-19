#include "global.h"

#include "pokedex.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F3CA8(void *arg0, s32 arg1, s8 *arg2, s8 *arg3);

void ov18_021F3CA8(void *arg0, s32 arg1, s8 *arg2, s8 *arg3) {
    u8 *new_var;
    s32 temp_r1;
    s32 temp_r6;
    u8 temp_r0;
    u8 temp_r3;
    temp_r6 = (s32)(arg0 + 0x18A4);
    new_var = (u8 *)arg0;
    temp_r3 = *((u8 *)(temp_r6 + arg1));
    if (temp_r3 & 0x80) {
        if ((((PokedexAppData *)new_var)->curSpecies) == 0xAC) {
            temp_r1 = temp_r3 ^ 0x80;
            switch (temp_r1) {
            case 0:
                *arg2 = 0;
                *arg3 = 0;
                return;

            case 1:
                *arg2 = 0;
                *arg3 = 1;
                return;

            case 2:
                *arg2 = 1;
                *arg3 = 1;
                return;
            }

        } else {
            *arg2 = temp_r3 ^ 0x80;
            *arg3 = (s8)Pokedex_SpeciesGetLastSeenGender(*(*((Pokedex ***)arg0)), ((PokedexAppData *)new_var)->curSpecies, 0U);
        }
    } else {
        *arg2 = 0;
        temp_r0 = *((u8 *)(temp_r6 + arg1));
        switch (temp_r0) {
        case 1:
            *arg3 = 0;
            return;

        case 2:
            *arg3 = 1;
            return;

        case 3:
        default:
            *arg3 = 2;
            return;
        }
    }
}
