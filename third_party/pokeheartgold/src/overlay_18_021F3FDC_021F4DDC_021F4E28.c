#include "global.h"

#include "application/pokedex/pokedex_internal.h"
#include "sprite.h"
#include "sprite_system.h"

void ov18_021F1294(PokedexAppData *, u32, s16, s16, u32);

void ov18_021F12C8(PokedexAppData *, u32, s16 *, s16 *, u32);

void ov18_021F4E28(PokedexAppData *param_1);

void ov18_021F4E28(PokedexAppData *param_1) {
    PokedexAppData *p = param_1;
    PokedexAppData_UnkSub18CC *q = &p->unk_18CC;
    f32 x;
    s16 sp[2];
    x = (f32)FX_Div(0x100000, q->player_scale[p->curSpecies] << 0xc) / 4096.0f;
    ManagedSprite_SetAffineOverwriteMode(p->unk_0670[1], 2);
    ManagedSprite_SetAffineScale(p->unk_0670[1], x, x);
    ov18_021F12C8(param_1, 1, &sp[1], &sp[0], 2);
    ov18_021F1294(param_1, 1, sp[1], (s16)(sp[0] + q->player_ypos[p->curSpecies]), 2);
}
