#include "global.h"
struct UnkStruct_ov112_021EA230;

#include "sprite_system.h"
#include "overlay_112_unk_struct_ov112_021_ea230.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA4F4(Ov112PokewalkerApp *arg0);

void ov112_021EA4F4(Ov112PokewalkerApp *arg0) {
    ManagedSprite_SetAnimateFlag(*((ManagedSprite * *)arg0->sprites), 1);
    ManagedSprite_SetAnim(*((ManagedSprite * *)arg0->sprites), 8);
    ManagedSprite_SetDrawFlag(*((ManagedSprite * *)arg0->sprites), 0);
}
