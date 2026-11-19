#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1EFC(Ov112SpriteRec *arg0, s32 flag);

void ov112_021F1CC8(Ov112SpriteApp *param_1, s32 param_2);

void ov112_021F1CC8(Ov112SpriteApp *param_1, s32 param_2) {
    ov112_021F1EFC(&param_1->spriteRecs[0], param_2);
    ov112_021F1EFC(&param_1->spriteRecs[1], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[4], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[5], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[6], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[9], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[7], param_2);
    ManagedSprite_SetDrawFlag(param_1->sprites[8], param_2);
    return;
}
