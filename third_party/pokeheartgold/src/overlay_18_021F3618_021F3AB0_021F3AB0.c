#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

u32 ov18_021F3AB0(PokedexAppData *pokedexApp, int a1) {
    u32 in_r3 = 0;
    s16 asStack_6;
    s16 sStack_8;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &asStack_6, &sStack_8);
    return ((s32)asStack_6) - 0x34;
    asStack_6 = (s16)(((u32)in_r3) >> 0x10);
}
