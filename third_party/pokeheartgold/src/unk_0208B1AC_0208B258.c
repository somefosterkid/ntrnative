#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_0208B1AC.h"
#include "pokemon_summary_app.h"

void sub_0208B258(PokemonSummary *arg0) {
    SpriteSystem_DestroySpriteManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
}

void sub_0208B278(PokemonSummary *arg0) {
    Sprite_UpdateAnim(arg0->sprites[21], 0x1000);
    Sprite_UpdateAnim(arg0->sprites[22], 0x1000);
    Sprite_UpdateAnim(arg0->sprites[64], 0x1000);
    Sprite_UpdateAnim(arg0->sprites[65], 0x1000);
}
