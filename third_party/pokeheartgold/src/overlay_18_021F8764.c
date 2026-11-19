#include "global.h"

#include "pokedex.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F8764(void *arg0, s32 *arg1, u16 *arg2, u32 arg3, u16 *arg4, u32 arg5, s32 arg6, Pokedex *arg7);

void ov18_021F8764(void *arg0, s32 *arg1, u16 *arg2, u32 arg3, u16 *arg4, u32 arg5, s32 arg6, Pokedex *pokedex) {
    unsigned char new_var;
    u32 v3;
    u16 *v2;
    s32 *v1;
    void *v0;
    u16 *v7;
    u16 *v9;
    u16 *v12;
    u16 species;
    u16 v5;
    u32 v4;
    u32 v6;
    u32 v8;
    u32 v11;
    new_var = 0U;
    v0 = arg0;
    v1 = arg1;
    v2 = arg2;
    *v1 = 0;
    v3 = 0;
    if (arg6 == 1) {
        if (arg5 > new_var) {
            v7 = arg4;
            do {
                v8 = 0;
                if (arg3 > new_var) {
                    u16 *v10;
                    v5 = *v7;
                    v10 = v2;
loop_5:
                    if (v5 == (*v10)) {
                        *((u16 *)(v0 + ((*v1) * 2))) = v5;
                        *v1 = (*v1) + 1;
                    } else {
                        v8 += 1;
                        v10 += 1;
                        if (v8 < arg3) {
                            goto loop_5;
                        }
                    }
                }
                v7 += 1;
                v3 = (v6 = v3 + 1);
            } while (v6 < arg5);
        }
    } else if (arg5 > new_var) {
        v12 = arg4;
        do {
            v11 = 0;
            if (arg3 > new_var) {
                v9 = v2;
loop_14:
                species = *v12;

                if ((species == (*v9)) && (Pokedex_CheckMonCaughtFlag(pokedex, species) != 0)) {
                    *((u16 *)(v0 + ((*v1) * 2))) = *v12;
                    *v1 = (*v1) + 1;
                } else {
                    v9 += 1;
                    v11 += 1;
                    if (v11 < arg3) {
                        goto loop_14;
                    }
                }
            }
            v12 += 1;
            v4 = v3 + 1;
            v3 = v4;
        } while (v4 < arg5);
    }
}

u32 ov18_021F8824(PokedexAppData *pokedexApp) {
    return pokedexApp->unk_185A + (0xF * pokedexApp->unk_1859);
}

u16 ov18_021F8838(PokedexAppData *pokedexApp) {
    return pokedexApp->unk_1030[ov18_021F8824(pokedexApp)].unk_0;
}

u32 ov18_021F8850(PokedexAppData_UnkSub0878 *a0, u16 a1) {
    u16 *puVar1;
    u32 uVar2;
    u32 uVar3;

    uVar3 = 0;
    puVar1 = &a0->unk_7B4;
    uVar2 = 0;
    while (uVar2 < *puVar1) {
        if (uVar3 == 0) {
            uVar3 = (u32)a0->unk_000[0][0];
        }
        if ((u32)a1 == (u32)a0->unk_000[0][0]) {
            return (u32)a1;
        }
        uVar2 = uVar2 + 1;
        a0 = (PokedexAppData_UnkSub0878 *)(a0->unk_000 + 1);
    }
    return uVar3;
}
