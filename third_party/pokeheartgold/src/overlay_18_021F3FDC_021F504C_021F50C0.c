#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F1294(PokedexAppData *, u32, s16, s16, u32);
#else
void ov18_021F1294(PokedexAppData *, u32, s32, s32, u32);
#endif

void ov18_021F12C8(PokedexAppData *, u32, s16 *, s16 *, u32);

BOOL ov18_021F50C0(PokedexAppData_UnkSub0868_State77_Sub4 *a0) {
    s32 iVar1;
    s16 sStack_14[4];
    ov18_021F12C8(a0->pokedexApp, 2, &sStack_14[3], &sStack_14[2], 1);
    ov18_021F12C8(a0->pokedexApp, 4, &sStack_14[1], &sStack_14[0], 1);
    iVar1 = a0->unk_04 + 0x400;
    a0->unk_04 = iVar1;
    sStack_14[2] = sStack_14[2] + (iVar1 >> 0xc);
    sStack_14[0] = sStack_14[0] + (a0->unk_04 >> 0xc);
    if ((0x57 + 1) <= sStack_14[2]) {
        ov18_021F1294(a0->pokedexApp, 2, (s32)sStack_14[3], 0x58, 1);
        ov18_021F1294(a0->pokedexApp, 4, (s32)sStack_14[1], 0x58, 1);
        return 0;
    }
    ov18_021F1294(a0->pokedexApp, 2, (s32)sStack_14[3], (s32)sStack_14[2], 1);
    ov18_021F1294(a0->pokedexApp, 4, (s32)sStack_14[1], (s32)sStack_14[0], 1);
    return 1;
}
