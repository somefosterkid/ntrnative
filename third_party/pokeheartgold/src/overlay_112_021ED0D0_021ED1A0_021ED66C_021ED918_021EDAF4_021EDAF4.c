#include "global.h"

#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EDAF4(Ov112PokewalkerApp *arg0, s32 arg1) {
    if (arg1 == 0) {
        ManagedSprite_SetAnimationFrame(arg0->sprites[8], 0);
    } else if (arg1 > 30) {
        ManagedSprite_SetAnimationFrame(arg0->sprites[8], (arg1 - 30) / 4);
    }
}
