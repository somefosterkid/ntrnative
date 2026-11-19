#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

extern u8 ov18_021FA310[];

u8 ov18_021F2C98(PokedexAppData *pokedexApp, s32 param_2);

u8 ov18_021F2C98(PokedexAppData *pokedexApp, s32 param_2) {
    u8 *new_var2;
    u16 uVar1;
    ManagedSprite **new_var;
    new_var = &pokedexApp->unk_0670[param_2];
    uVar1 = ManagedSprite_GetActiveAnim(*new_var);
    new_var2 = (u8 *)(ov18_021FA310 + 0x1);
    new_var2 = uVar1 + new_var2;
    return *((u8 *)new_var2);
}
