#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA570(Ov112PokewalkerApp *arg0);

void ov112_021EA570(Ov112PokewalkerApp *arg0) {
    ManagedSprite_SetDrawFlag(arg0->sprites[0], 0);
}

void ov112_021EA584(Ov112PokewalkerApp *arg0) {
    ManagedSprite_SetAnim(arg0->sprites[0], ManagedSprite_GetActiveAnim(arg0->sprites[0]) + 3);
}

void ov112_021EA5A4(Ov112PokewalkerApp *param_1, s32 param_2) {
    ManagedSprite_SetAnim(param_1->sprites[1], 0);
    ManagedSprite_SetDrawFlag(param_1->sprites[1], 1);
    if (param_2 == 0) {
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 0);
        return;
    }
    if (0 < param_2) {
        ManagedSprite_SetDrawPriority(param_1->sprites[1], 2);
        ManagedSprite_SetDrawPriority(param_1->sprites[2], 1);
        ManagedSprite_SetDrawFlag(param_1->sprites[2], 1);
        ManagedSprite_SetAnim(param_1->sprites[2], param_2);
        ManagedSprite_SetAnimateFlag(param_1->sprites[2], 1);
    }
    return;
}
