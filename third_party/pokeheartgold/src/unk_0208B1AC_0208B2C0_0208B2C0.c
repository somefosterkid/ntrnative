#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"

void sub_0208B55C(PokemonSummary *arg0);

void sub_0208B714(PokemonSummary *arg0);

void sub_0208B74C(PokemonSummary *);

extern const UnmanagedSpriteTemplate _02103A70[];

void sub_0208B2C0(PokemonSummary *param_1);

void sub_0208B2C0(PokemonSummary *param_1) {
    Sprite *pSVar1;
    u32 uVar2;

    uVar2 = 0;
    do {
        pSVar1 = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem,
            param_1->spriteManager,
            &_02103A70[uVar2]);
        param_1->sprites[uVar2] = pSVar1;
        uVar2 = uVar2 + 1 & 0xffff;
    } while (uVar2 < 0x73);
    sub_0208B55C(param_1);
    Sprite_SetDrawFlag(param_1->sprites[10], 0);
    Sprite_SetDrawFlag(param_1->sprites[41], 0);
    Sprite_SetDrawFlag(param_1->sprites[42], 0);
    Sprite_SetDrawFlag(param_1->sprites[43], 0);
    Sprite_SetDrawFlag(param_1->sprites[44], 0);
    Sprite_SetDrawFlag(param_1->sprites[45], 0);
    Sprite_SetDrawFlag(param_1->sprites[46], 0);
    Sprite_SetDrawFlag(param_1->sprites[47], 0);
    Sprite_SetDrawFlag(param_1->sprites[48], 0);
    Sprite_SetDrawFlag(param_1->sprites[49], 0);
    Sprite_SetDrawFlag(param_1->sprites[50], 0);
    Sprite_SetDrawFlag(param_1->sprites[51], 0);
    Sprite_SetDrawFlag(param_1->sprites[67], 0);
    Sprite_SetDrawFlag(param_1->sprites[68], 0);
    Sprite_SetDrawFlag(param_1->sprites[69], 0);
    Sprite_SetDrawFlag(param_1->sprites[70], 0);
    Sprite_SetDrawFlag(param_1->sprites[71], 0);
    sub_0208B714(param_1);
    sub_0208B74C(param_1);
    Sprite_SetDrawFlag(param_1->sprites[72], 0);
    return;
}
