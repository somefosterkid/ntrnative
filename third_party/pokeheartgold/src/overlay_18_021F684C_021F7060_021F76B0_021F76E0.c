#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern u8 ov18_021FB620[];

void ov18_021F76E0(PokedexAppData *arg0, s32 arg1);

void ov18_021F76E0(PokedexAppData *arg0, s32 arg1) {
    u8 sp0[2];
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->unk_1864, arg1), &sp0[1], &sp0[0]);
    ManagedSprite_SetPositionXY(arg0->unk_0670[0], sp0[1], sp0[0]);
    ov18_021F118C(arg0, 0, ov18_021FB620[arg1]);
}
