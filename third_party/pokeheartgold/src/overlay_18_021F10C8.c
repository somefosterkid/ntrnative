#include "global.h"
struct PokedexAppData;

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F10C8(struct PokedexAppData *arg0);

void ov18_021F10C8(PokedexAppData *arg0) {
    SpriteSystem_FreeResourcesAndManager(arg0->unk_0668, arg0->unk_066C);
    SpriteSystem_Free(arg0->unk_0668);
}
