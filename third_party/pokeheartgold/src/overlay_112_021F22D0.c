#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F22D0(Ov112SpriteApp *param_1, s32 param_2, s32 param_3, s32 param_4);

s32 ov112_021F235C(void *arg0);
s32 ov112_021F24D8(void *);
void ov112_021F2328(Ov112SpriteApp *arg0);
s32 ov112_021F238C(Ov112SpriteApp *ctx);
s32 ov112_021F24F8(Ov112SpriteApp *);
void ov112_021F2338(Ov112SpriteApp *arg0);

void ov112_021F22D0(Ov112SpriteApp *param_1, s32 param_2, s32 param_3, s32 param_4) {
    s16 sStack_1a;
    s16 sStack_1c;

    ManagedSprite_GetPositionXY(param_1->sprites[param_3], &sStack_1a, &sStack_1c);
    if (param_1->spriteRecs[param_2].unk_0 != 0) {
        sStack_1c = 0x18;
    } else {
        sStack_1c = 0x28;
    }
    ManagedSprite_SetPositionXY(param_1->sprites[param_3], sStack_1a, sStack_1c);
    ManagedSprite_SetAnim(param_1->sprites[param_3], param_4);
    ManagedSprite_SetDrawFlag(param_1->sprites[param_3], 1);
    return;
}

void ov112_021F2328(Ov112SpriteApp *arg0) {
    ov112_021F235C(arg0);
    ov112_021F24D8(arg0);
}

void ov112_021F2338(Ov112SpriteApp *arg0) {
    ov112_021F238C(arg0);
    ov112_021F24F8(arg0);
}
