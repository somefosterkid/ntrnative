#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F1294(PokedexAppData *, s32, s16, s16, u32);

void ov18_021F12C8(PokedexAppData *, u32, s16 *, s16 *, u32);

void ov18_021F41C4(PokedexAppData *, u32 param_2, s16 *param_3, s16 *param_4, u8 *param_5);

void ov18_021F42E4(void *param_1, u32 param_2, s16 *param_3, s16 *param_4, u8 *param_5);

void ov18_021F47C0(u32, s32 *);

void ov18_021F47F8(PokedexAppData *, s32, u32);

s32 ov18_021F4974(PokedexAppData *, u32, s32, s32);

void ov18_021F69C0(PokedexAppData *, s32);

extern u8 ov18_021FA3B0[][2];

void ov18_021F4384(PokedexAppData *pokedexApp) {
    BOOL BVar1;
    u32 uVar2;
    s32 uVar3;
    s32 uVar4;
    s32 a1;
    s32 iStack_2c;
    s32 aiStack_1c;
    s16 asStack_20[2];
    s16 sStack_22;
    s16 sStack_24;
    s16 sStack_26;
    u8 abStack_28[2];

    iStack_2c = 0;
    ov18_021F12C8(pokedexApp, 2, asStack_20, &sStack_22, 2);
    aiStack_1c = 0;
    if (pokedexApp->unk_18CA == 0) {
        uVar4 = 1;
        if (1 < (pokedexApp->unk_18DC).unk_20.nMaps) {
            do {
                BVar1 = ov18_021E8AB0(pokedexApp, uVar4);
                if (BVar1 == 0) {
                    ov18_021F41C4(pokedexApp, (pokedexApp->unk_18DC).unk_20.maps[uVar4], &sStack_24, &sStack_26, abStack_28);
                } else {
                    ov18_021F42E4((u8 *)pokedexApp, (pokedexApp->unk_18DC).unk_20.maps[uVar4], &sStack_24, &sStack_26, abStack_28);
                    uVar2 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[uVar4]);
                    ov18_021F47C0(uVar2, &aiStack_1c);
                }
                a1 = uVar4 + 8;
                ov18_021F1294(pokedexApp, a1, sStack_24, sStack_26, 2);
                ov18_021F118C(pokedexApp, a1, abStack_28[0]);
                ov18_021F11C0(pokedexApp, a1, 1);
                uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][0] >> 1;
                if (asStack_20[0] >= sStack_24 - uVar3 && asStack_20[0] < sStack_24 + uVar3) {
                    uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][1] >> 1;
                    if (sStack_22 >= sStack_26 - uVar3 && sStack_22 < sStack_26 + uVar3) {
                        iStack_2c = 1;
                    }
                }
                uVar4 = uVar4 + 1 & 0xffff;
            } while (uVar4 < (pokedexApp->unk_18DC).unk_20.nMaps);
        }
        ov18_021F47F8(pokedexApp, aiStack_1c, uVar4 + 8);
        if (iStack_2c == 0 && aiStack_1c != 0) {
            iStack_2c = ov18_021F4974(pokedexApp, uVar4 + 8, asStack_20[0], sStack_22);
        }
    } else {
        ov18_021F4620(pokedexApp);
        BVar1 = ov18_021E8AB0(pokedexApp, pokedexApp->unk_18CA);
        if (BVar1 == 0) {
            ov18_021F41C4(pokedexApp, (pokedexApp->unk_18DC).unk_20.maps[pokedexApp->unk_18CA], &sStack_24, &sStack_26, abStack_28);
            ov18_021F1294(pokedexApp, 9, sStack_24, sStack_26, 2);
            ov18_021F118C(pokedexApp, 9, abStack_28[0]);
            ov18_021F11C0(pokedexApp, 9, 1);
            uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][0] >> 1;
            if (asStack_20[0] >= sStack_24 - uVar3 && asStack_20[0] < sStack_24 + uVar3) {
                uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][1] >> 1;
                if (sStack_22 >= sStack_26 - uVar3 && sStack_22 < sStack_26 + uVar3) {
                    iStack_2c = 1;
                }
            }
        } else {
            ov18_021F42E4((u8 *)pokedexApp, (pokedexApp->unk_18DC).unk_20.maps[pokedexApp->unk_18CA], &sStack_24, &sStack_26, abStack_28);
            uVar4 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[pokedexApp->unk_18CA]);
            ov18_021F47C0(uVar4, &aiStack_1c);
            ov18_021F1294(pokedexApp, 9, sStack_24, sStack_26, 2);
            ov18_021F118C(pokedexApp, 9, abStack_28[0]);
            ov18_021F11C0(pokedexApp, 9, 1);
            uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][0] >> 1;
            if (asStack_20[0] >= sStack_24 - uVar3 && asStack_20[0] < sStack_24 + uVar3) {
                uVar3 = (u32)ov18_021FA3B0[abStack_28[0]][1] >> 1;
                if (sStack_22 >= sStack_26 - uVar3 && sStack_22 < sStack_26 + uVar3) {
                    iStack_2c = 1;
                }
            }
            ov18_021F47F8(pokedexApp, aiStack_1c, 10);
            if (iStack_2c == 0 && aiStack_1c != 0) {
                iStack_2c = ov18_021F4974(pokedexApp, 10, asStack_20[0], sStack_22);
            }
        }
    }
    ov18_021F69C0(pokedexApp, iStack_2c);
    return;
}
