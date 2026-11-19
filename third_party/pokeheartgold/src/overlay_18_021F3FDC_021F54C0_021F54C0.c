#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);
s32 ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

void ov18_021F54C0(PokedexAppData_UnkSub0868_State77_Sub4 *arg0, s16 arg1, s16 arg2) {
    s16 sp[4];

    arg0->unk_18 = arg1;
    arg0->unk_1A = arg2;
    ov18_021F12C8(arg0->pokedexApp, 2, &sp[3], &sp[2], 1);
    ov18_021F12C8(arg0->pokedexApp, 4, &sp[1], &sp[0], 1);
    if (sp[2] < sp[0]) {
        arg0->index = 2;
        arg0->unk_1E = 8;
        ov18_021F11C0(arg0->pokedexApp, 3, 1);
        ov18_021F1294(arg0->pokedexApp, 3, sp[3], (s16)(sp[2] + 0xC0), 2);
        return;
    }
    arg0->index = 4;
    arg0->unk_1E = 9;
    ov18_021F11C0(arg0->pokedexApp, 5, 1);
    ov18_021F1294(arg0->pokedexApp, 5, sp[1], (s16)(sp[0] + 0xC0), 2);
}
