#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02091278.h"
#include "wild_encounter.h"

void ov18_021F24E0(PokedexAppData *pokedexApp, u32 species, int a2) {
    BOOL BVar1;
    s32 a2_00;

    if (species == 0 || !(BVar1 = Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex), BVar1 != 0)) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a2], 0);
        return;
    }
    a2_00 = LanguageToDexFlag((u32)pokedexApp->unk_185C);
    ov18_021F118C(pokedexApp, a2, a2_00);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a2], 1);
    return;
}
