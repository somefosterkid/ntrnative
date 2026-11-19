#include "global.h"

#include "dex_mon_measures.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "pokedex.h"

void *ov18_021F8168(u32 arg0, u32 *arg1);

void ov18_021F8198(u8 *param_1, s32 *param_2, Pokedex *param_3, u16 *param_4, u32 param_5);
void ov18_021F81D8(u16 *param_1, Pokedex *param_2, u16 *param_3, u32 param_4);

void *ov18_021F8168(u32 arg0, u32 *arg1) {
    u32 sizeOut;
#if defined(SDK_PORT) || defined(NONMATCHING)
    void *ret;
#endif
    if (arg0 >= 0x52U) {
        GF_AssertFail();
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    ret = GfGfxLoader_LoadFromNarc_GetSizeOut(GetPokedexDataNarcID(), arg0 + 0xB, 0, HEAP_ID_POKEDEX_APP, 0, &sizeOut);
#else
    GfGfxLoader_LoadFromNarc_GetSizeOut(GetPokedexDataNarcID(), arg0 + 0xB, 0, HEAP_ID_POKEDEX_APP, 0, &sizeOut);
#endif
    *arg1 = sizeOut >> 1;
#if defined(SDK_PORT) || defined(NONMATCHING)
    return ret; /* r0 on DS = result of GfGfxLoader_LoadFromNarc_GetSizeOut() */
#endif
}

void ov18_021F8198(u8 *param_1, s32 *param_2, Pokedex *param_3, u16 *param_4, u32 param_5) {
    BOOL BVar1;
    u32 uVar2;

    uVar2 = 0;
    *param_2 = 0;
    while (uVar2 < param_5) {
        BVar1 = Pokedex_CheckMonSeenFlag(param_3, *param_4);
        if (BVar1 != 0) {
            *(u16 *)(param_1 + *param_2 * 2) = *param_4;
            *param_2 = *param_2 + 1;
        }
        uVar2 = uVar2 + 1;
        param_4 = param_4 + 1;
    }
    return;
}

void ov18_021F81D8(u16 *param_1, Pokedex *param_2, u16 *param_3, u32 param_4) {
    BOOL BVar1;
    u16 *puVar2;
    u32 uVar3;

    param_1[0x3da] = (u16)param_4;
    uVar3 = 0;
    param_1[0x3db] = 0;
    if (param_4 != 0) {
        puVar2 = param_1 + 0x3db;
        do {
            *param_1 = *param_3;
            BVar1 = Pokedex_CheckMonCaughtFlag(param_2, *param_3);
            if (BVar1 != 0) {
                param_1[1] = 2;
                *puVar2 = *puVar2 + 1;
            } else {
                param_1[1] = 1;
            }
            param_3 = param_3 + 1;
            param_1 = param_1 + 2;
            uVar3 = uVar3 + 1;
        } while (uVar3 < param_4);
    }
    return;
}
