#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

extern u8 ov18_021FA3B0;

u32 ov18_021F4974(PokedexAppData *pokedexApp, u32 param_2, s32 param_3, s32 param_4);

u32 ov18_021F4974(PokedexAppData *pokedexApp, u32 param_2, s32 param_3, s32 param_4) {
    u32 uVar1;
    BOOL BVar2;
    unsigned int new_var;
    s16 sStack_1a;
    s16 sStack_1c;
    uVar1 = param_2 + 4;
    if (param_2 < uVar1) {
        do {
            BVar2 = ManagedSprite_GetDrawFlag(pokedexApp->unk_0670[param_2]);
            if (BVar2 != 0) {
                ov18_021F12C8(pokedexApp, param_2, &sStack_1a, &sStack_1c, 2);
                if ((param_3 >= ((s32)(((s32)sStack_1a) - ((u32)ov18_021FA3B0 >> 1)))) && (param_3 < ((s32)(((s32)sStack_1a) + ((u32)ov18_021FA3B0 >> 1))))) {
                    new_var = ((u8 *)(&ov18_021FA3B0))[1];
                    if ((param_4 >= ((s32)(((s32)sStack_1c) - ((u32)(new_var >> 1))))) && (param_4 < ((s32)(((s32)sStack_1c) + ((u32)(new_var >> 1)))))) {
                        return 1;
                    }
                }
            }
            param_2 = param_2 + 1;
        } while (param_2 < uVar1);
    }
    return 0;
}
