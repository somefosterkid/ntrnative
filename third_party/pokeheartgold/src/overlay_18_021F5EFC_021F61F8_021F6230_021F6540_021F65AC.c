#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F65AC(PokedexAppData *pokedexApp) {
    s32 iVar1;
    s16 asStack_e;
    s16 sStack_10;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[5], &asStack_e, &sStack_10);
    iVar1 = ov18_021F64A4(pokedexApp, (s32)pokedexApp->unk_18C5);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[5], asStack_e, (s16)iVar1);
    return;
}

void ov18_021F65EC(PokedexAppData *pokedexApp) {
    s32 iVar1;
    s16 asStack_e;
    s16 sStack_10;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[5], &asStack_e, &sStack_10);
    iVar1 = ov18_021F64F4(pokedexApp, (s32)pokedexApp->unk_18CA);
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[5], asStack_e, (s16)iVar1);
    return;
}
