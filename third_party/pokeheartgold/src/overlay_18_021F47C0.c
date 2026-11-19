#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite.h"
#include "sprite_system.h"

void ov18_021F47C0(s32 arg0, s32 *arg1);

void ov18_021F48AC(PokedexAppData *param_1, u32 param_2, u32 param_3, s32 param_4, s32 param_5);

void ov18_021F47C0(s32 arg0, s32 *arg1) {
    if (arg0 == 0x6A) {
        *arg1 |= 1;
        return;
    }
    if ((arg0 == 0x78) || ((u32)(arg0 - 0xED) <= 2U)) {
        *arg1 |= 2;
        return;
    }
    if ((arg0 == 0x7B) || (arg0 == 0xB0)) {
        *arg1 |= 4;
    }
}

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

void ov18_021F47F8(PokedexAppData *arg0, s32 arg1, s32 arg2);

void ov18_021F47F8(PokedexAppData *pokedexApp, s32 arg1, s32 arg2) {
    s32 a1;

    a1 = arg2;
    if (1 & arg1) {
        ov18_021F1294(pokedexApp, a1, 0x94, 0x4C, 2);
        ov18_021F118C(pokedexApp, a1, 0);
        ov18_021F11C0(pokedexApp, a1, 1);
        a1 += 1;
    }
    if (arg1 & 2) {
        ov18_021F1294(pokedexApp, a1, 0xEC, 0x4C, 2);
        ov18_021F118C(pokedexApp, a1, 0);
        ov18_021F11C0(pokedexApp, a1, 1);
        a1 += 1;
    }
    if (4 & arg1) {
        ov18_021F1294(pokedexApp, a1, 0xE4, 0x5C, 2);
        ov18_021F118C(pokedexApp, a1, 0);
        ov18_021F11C0(pokedexApp, a1, 1);
        ov18_021F1294(pokedexApp, a1 + 1, 0xDC, 0x7C, 2);
        ov18_021F118C(pokedexApp, a1 + 1, 0);
        ov18_021F11C0(pokedexApp, a1 + 1, 1);
    }
}

void ov18_021F48AC(PokedexAppData *param_1, u32 param_2, u32 param_3, s32 param_4, s32 param_5) {
    if ((param_2 & 1) != 0) {
        if ((param_3 & 1) != 0) {
            ov18_021F11C0(param_1, param_4, 0);
        } else {
            ov18_021F11C0(param_1, param_4, 1);
            ManagedSprite_SetPaletteOverride(param_1->unk_0670[param_4], param_5);
        }
        param_4 = param_4 + 1;
    }
    if ((param_2 & 2) != 0) {
        if ((param_3 & 2) != 0) {
            ov18_021F11C0(param_1, param_4, 0);
        } else {
            ov18_021F11C0(param_1, param_4, 1);
            ManagedSprite_SetPaletteOverride(param_1->unk_0670[param_4], param_5);
        }
        param_4 = param_4 + 1;
    }
    if ((param_2 & 4) != 0) {
        if ((param_3 & 4) != 0) {
            ov18_021F11C0(param_1, param_4, 0);
            ov18_021F11C0(param_1, param_4 + 1, 0);
            return;
        }
        ov18_021F11C0(param_1, param_4, 1);
        ov18_021F11C0(param_1, param_4 + 1, 1);
        ManagedSprite_SetPaletteOverride(param_1->unk_0670[param_4], param_5);
        ManagedSprite_SetPaletteOverride(param_1->unk_0670[param_4 + 1], param_5);
    }
    return;
}
