#include "global.h"

#include "application/pokedex/pokedex_internal.h"

extern u32 ov18_021FA398;

s32 ov18_021F2C74(PokedexAppData *param_1, BOOL param_2);

s32 ov18_021F2C74(PokedexAppData *param_1, BOOL param_2) {
    u16 uVar1;
    u32 uVar2;
    u16 *puVar3;

    uVar1 = ov18_021F891C(param_1, param_2);
    puVar3 = (u16 *)&ov18_021FA398;
    uVar2 = 0;
    do {
        if (uVar1 <= *puVar3) {
            break;
        }
        uVar2 = uVar2 + 1;
        puVar3 = puVar3 + 1;
    } while (uVar2 < 0xc);
    return uVar2 + 0xb;
}
