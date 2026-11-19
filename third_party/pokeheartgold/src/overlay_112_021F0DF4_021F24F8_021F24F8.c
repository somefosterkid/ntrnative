#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1EFC(Ov112SpriteRec *, u32);

void ov112_021F24F8(Ov112SpriteApp *param_1);

void ov112_021F24F8(Ov112SpriteApp *param_1) {
    if (!param_1->bits14C.active) {
        return;
    }
    switch (param_1->bits14C.state) {
    case 0:
        ManagedSprite_SetDrawFlag(param_1->sprites[5], 1);
        ManagedSprite_SetAnim(param_1->sprites[5], 0x18);
        param_1->bits14C.state++;
        break;
    case 1:
        if (param_1->bits14C.timer >= 25) {
            ov112_021F1EFC(&param_1->spriteRecs[1], 0);
            param_1->bits14C.timer = 0;
        } else {
            param_1->bits14C.timer++;
        }
        break;
    case 2:
        break;
    default:
        GF_AssertFail();
        break;
    }
}
