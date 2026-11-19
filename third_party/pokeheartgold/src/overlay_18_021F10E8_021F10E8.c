#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

void ov18_021F10E8(PokedexAppData *pokedexApp, u32 index);

void ov18_021F10E8(PokedexAppData *pokedexApp, u32 index) {
    if (pokedexApp->unk_0670[index] != NULL) {
        Sprite_DeleteAndFreeResources(pokedexApp->unk_0670[index]);
        pokedexApp->unk_0670[index] = NULL;
    }
}
