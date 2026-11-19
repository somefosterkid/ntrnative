#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F41C4(PokedexAppData *pokedexApp, u32 param_2, s16 *param_3, s16 *param_4, u8 *param_5);

void ov18_021F41C4(PokedexAppData *pokedexApp, u32 param_2, s16 *param_3, s16 *param_4, u8 *param_5) {
    s32 uVar1;
    BOOL BVar2;
    u32 uVar3;
    u16 uVar4;
    u8 uVar5;
    u8 uVar6;

    BVar2 = ov18_021E8B24(param_2);
    if (BVar2 == 1) {
        uVar1 = 0x20;
        uVar4 = 4;
        uVar5 = 1;
        uVar6 = 3;
    } else {
        uVar3 = ov18_021E8B0C(param_2);
        if (uVar3 == 0x12) {
            uVar6 = 2;
            uVar1 = pokedexApp->unk_190C[param_2].unk_00;
            uVar4 = pokedexApp->unk_190C[param_2].unk_01;
            uVar5 = 1;
        } else {
            BVar2 = ov18_021E8B5C(param_2);
            if (BVar2 == 1) {
                uVar1 = 0x23;
                uVar4 = 8;
                uVar5 = 2;
                uVar6 = 1;
            } else {
                uVar1 = pokedexApp->unk_190C[param_2].unk_00;
                uVar4 = pokedexApp->unk_190C[param_2].unk_01;
                uVar5 = pokedexApp->unk_190C[param_2].unk_02;
                uVar6 = pokedexApp->unk_190C[param_2].unk_03;
            }
        }
    }
    if (uVar5 == 1) {
        if (uVar6 == 1) {
            *param_5 = 0;
        } else if (uVar6 == 2) {
            *param_5 = 6;
        } else if (uVar6 == 3) {
            *param_5 = 7;
        } else if (uVar6 == 4) {
            *param_5 = 8;
        } else if (uVar6 == 5) {
            *param_5 = 9;
        }
    } else if (uVar5 == 2) {
        if (uVar6 == 1) {
            *param_5 = 1;
        } else {
            *param_5 = 10;
        }
    } else if (uVar5 == 3) {
        if (uVar6 == 1) {
            *param_5 = 2;
        } else {
            *param_5 = 0xb;
        }
    } else if (uVar5 == 4) {
        *param_5 = 3;
    } else if (uVar5 == 5) {
        *param_5 = 4;
    } else if (uVar5 == 6) {
        *param_5 = 5;
    } else {
        *param_5 = 0;
    }
    *param_3 = ((uVar5 * 8) / 2) + (uVar1 - pokedexApp->unk_18C8 * 0x16) * 8 + 0x40;
    *param_4 = uVar4 * 8 + ((uVar6 * 8) / 2) + 0x28;
    return;
}
