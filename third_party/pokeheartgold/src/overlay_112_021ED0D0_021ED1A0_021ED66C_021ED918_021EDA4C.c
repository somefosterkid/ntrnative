#include "global.h"

#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7594(void *, void *, void *, void *);

void ov112_021E7CC8(Ov112PokewalkerApp *);

s32 ov112_021E9290(BoxPokemon *, void *, void *, s32);

u32 ov112_021EDA4C(Ov112PokewalkerApp *param_1) {
    ov112_021E7DA4(param_1);
    ov112_021E7594(param_1->unk_C910, &param_1->unk_F1D0, param_1->unk_9D70, &param_1->unk_AABC);
    ov112_021E7CC8(param_1);
    if (param_1->unk_1E42C != NULL) {
        ov112_021E9290(&(param_1->unk_1E42C)->box, param_1->unk_1D7AC, param_1->unk_1D79C, 0);
    } else {
        ov112_021E9290(param_1->unk_1E430, param_1->unk_1D7AC, param_1->unk_1D79C, 0);
    }
    ManagedSprite_SetOamMode(param_1->sprites[8], GX_OAM_MODE_XLU);
    ov112_021EA670(param_1, 8);
    ManagedSprite_SetAnim(param_1->sprites[8], 0x19);
    ManagedSprite_SetAnimationFrame(param_1->sprites[8], 0);
    param_1->unk_1F2D4 = 0;
    ov112_021EA5A4(param_1, 2);
    ov112_021ED25C(param_1);
    return 0x1f;
}
