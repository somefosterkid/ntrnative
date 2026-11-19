#include "global.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F3CA8(PokedexAppData *, s32, void *, s32 *);

s32 ov18_021F69E8(PokedexAppData *, u16, u8, u8, u8);

s32 ov18_021F6AB0(PokedexAppData *, s32, s32);

void ov18_021F4DDC(PokedexAppData *arg0);

void ov18_021F4DDC(PokedexAppData *arg0) {
    u8 sp4[4];
    s32 temp_r3;
    PokedexAppData_UnkSub18CC *temp_r4;

    ov18_021F3CA8(arg0, 0, &sp4[1], (s32 *)sp4);
    temp_r4 = &arg0->unk_18CC;
    ov18_021F69E8(arg0, arg0->curSpecies, sp4[1], sp4[0], 2);
    temp_r3 = arg0->curSpecies;
    ov18_021F6AB0(arg0, temp_r4->mon_ypos[temp_r3], temp_r4->mon_scale[temp_r3]);
}
