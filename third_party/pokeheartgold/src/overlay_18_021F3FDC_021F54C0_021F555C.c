#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);
void ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

void ov18_021F555C(PokedexAppData_UnkSub0868_State77_Sub4 *param_1) {
    s16 asStack_12;
    s16 sStack_14;

    ov18_021F12C8(param_1->pokedexApp, param_1->index, &asStack_12, &sStack_14, 1);
    sStack_14 = sStack_14 + param_1->unk_18;
    ov18_021F1294(param_1->pokedexApp, param_1->index, (s32)asStack_12, (s32)sStack_14, 1);
    ov18_021F12C8(param_1->pokedexApp, param_1->index + 1, &asStack_12, &sStack_14, 2);
    sStack_14 = sStack_14 + param_1->unk_18;
    ov18_021F1294(param_1->pokedexApp, param_1->index + 1, (s32)asStack_12, (s32)sStack_14, 2);
    param_1->unk_18 = param_1->unk_18 + param_1->unk_1A;
    return;
}
