#include "global.h"

#include "pokedex.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F8584(void *arg0, s32 arg1, void *arg2, u32 *arg3, u16 *arg4, u32 arg5, Pokedex *arg6);

void ov18_021F8584(void *arg0, s32 arg1, void *arg2, u32 *arg3, u16 *arg4, u32 arg5, Pokedex *pokedex) {
    u32 v3;
    u32 v2;
    void *v1;
    void *v0;
    int new_var;
    u16 *v8;
    u16 *v9;
    u16 species;
    unsigned int new_var2;
    u16 v7;
    u32 v4;
    u32 v6;
    u8 v5;
    new_var2 = 0U;
    v0 = arg0;
    v1 = arg2;
    if (8 & arg1) {
        memcpy(arg2, arg4, arg5 * 2);
        *arg3 = arg5;
        return;
    }
    if (arg1 == 6) {
        v3 = 0;
        new_var = 2;
        if (arg5 > new_var2) {
            v8 = arg4;
            do {
                species = *v8;
                v5 = *((u8 *)((((PokedexAppData *)v0)->unk_1854) + species));
                if (((arg1 & v5) && (!(v5 & 1))) && (Pokedex_CheckMonSeenFlag(pokedex, species) != 0)) {
                    *((u16 *)(v1 + ((*arg3) * new_var))) = *v8;
                    *arg3 += 1;
                }
                v8 += 1;
                v4 = (v3 = v3 + 1);
            } while (v4 < arg5);
        }
    } else {
        v2 = 0;
        if (arg5 > new_var2) {
            v9 = arg4;
            do {
                v7 = *v9;
                if (((*((u8 *)((((PokedexAppData *)v0)->unk_1854) + v7))) & arg1) && (Pokedex_CheckMonSeenFlag(pokedex, v7) != 0)) {
                    *((u16 *)(v1 + ((*arg3) * 2))) = *v9;
                    *arg3 += 1;
                }
                v9 += 1;
                v6 = v2 + 1;
                v2 = v6;
            } while (v6 < arg5);
        }
    }
}
