#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);
s32 ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

s32 ov18_021F55D8(PokedexAppData_UnkSub0868_State77_Sub4 *arg0) {
    s16 sp8[2];
    s16 sp4[2];
    s32 temp_r3;

    ov18_021F555C(arg0);
    ov18_021F12C8(arg0->pokedexApp, arg0->index, &sp8[1], sp8, 1);
    ov18_021F12C8(arg0->pokedexApp, arg0->unk_1E, &sp4[1], sp4, 1);
    temp_r3 = sp4[0] - 0x10;
    if (sp8[0] >= temp_r3) {
        ov18_021F1294(arg0->pokedexApp, arg0->index, sp4[1], (s16)temp_r3, 1);
        return 0;
    }
    return 1;
}
