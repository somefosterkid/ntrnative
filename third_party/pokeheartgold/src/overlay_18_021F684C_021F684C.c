#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokemon.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F1294(PokedexAppData *, s32, s16, s16, u32);
#else
void ov18_021F1294(PokedexAppData *, s32, u32, u32, u32);
#endif

void ov18_021F3CA8(PokedexAppData *, u32, u8 *, u8 *);

void ov18_021F684C(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4);

void ov18_021F684C(PokedexAppData *param_1, s32 param_2, u32 param_3, u32 param_4) {
    u8 bVar1;
    u32 uVar2;
    u8 uVar3;
    u8 uStack_20;
    u32 unaff_r7;
    u8 uStack_1c;
    u8 auStack_1b[3];

    ov18_021F3CA8(param_1, param_3, &uStack_1c, auStack_1b);
    if (param_1->unk_18C7_7 == 0) {
        uVar3 = 2;
        uVar2 = 0;
    } else {
        uVar3 = 0;
        bVar1 = GetMonPicHeightBySpeciesGenderForm(param_1->curSpecies, auStack_1b[0], 0, uStack_1c, 0);
        uVar2 = (u32)bVar1;
    }
    if (param_2 == 1) {
        uStack_20 = 0x40;
        unaff_r7 = uVar2 + 0x78 & 0xff;
        if ((u32)(((u8 *)param_1)[0x18c7] << 26) >> 31 == 1) {
            param_2 = 3;
            ov18_021F11C0(param_1, 1, 0);
            ov18_021F11C0(param_1, 3, 1);
        } else {
            ov18_021F11C0(param_1, 1, 1);
            ov18_021F11C0(param_1, 3, 0);
        }
        {
            u8 *fl = &((u8 *)param_1)[0x18c7];
            *fl = *fl & ~0x20 | ((((u32)(u8)(1 ^ ((u32)(*fl << 26) >> 31))) << 31) >> 26);
        }
    } else if (param_2 == 2) {
        uStack_20 = 0xc0;
        unaff_r7 = uVar2 + 0x78 & 0xff;
        if ((u32)(((u8 *)param_1)[0x18c7] << 25) >> 31 == 1) {
            param_2 = 4;
            ov18_021F11C0(param_1, 2, 0);
            ov18_021F11C0(param_1, 4, 1);
        } else {
            ov18_021F11C0(param_1, 2, 1);
            ov18_021F11C0(param_1, 4, 0);
        }
        {
            u8 *fl = &((u8 *)param_1)[0x18c7];
            *fl = *fl & ~0x40 | ((((u32)(u8)(1 ^ ((u32)(*fl << 25) >> 31))) << 31) >> 25);
        }
    }
    ov18_021F1A7C(param_1, param_1->curSpecies, uStack_1c, auStack_1b[0], uVar3, param_2, param_4);
    ov18_021F1294(param_1, param_2, uStack_20, unaff_r7, 1);
    return;
}
