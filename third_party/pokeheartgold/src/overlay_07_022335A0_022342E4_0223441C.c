#include "global.h"
struct BallRotation;

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_ball_rotation.h"

void ov07_0221C69C(void);

s32 ov07_022341A4(void *, ManagedSpriteTemplate *, s16 *);

void ov07_0223441C(struct BallRotation *arg0);

void ov07_0223441C(BallRotation *arg0) {
    ManagedSpriteTemplate sp0;
    ManagedSprite *temp_r0;
    ManagedSpriteTemplate *var_r3;
    s32 var_r0;

    ov07_022341A4(arg0, &sp0, &sp0.y);
    var_r0 = 0;
    sp0.z = 0;
    sp0.animation = 0;
    sp0.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    sp0.drawPriority = 0;
    sp0.pal = 0;
    sp0.bgPriority = arg0->unk_90.bgPrio;
    sp0.vramTransfer = 0;
    var_r3 = &sp0;
    do {
        var_r0 += 1;
        var_r3->resIdList[0] = arg0->unk_90.target + 0x1770;
        var_r3 = (void *)((u8 *)var_r3 + (4));
    } while (var_r0 < 6);
    temp_r0 = SpriteSystem_NewSprite(arg0->unk_90.spriteSystem, arg0->unk_2C, &sp0);
    arg0->unk_30 = temp_r0;
    ManagedSprite_SetDrawFlag(temp_r0, 1);
    ManagedSprite_SetAffineOverwriteMode(arg0->unk_30, 2U);
    ManagedSprite_SetAnimationFrame(arg0->unk_30, 0U);
    ManagedSprite_SetAnim(arg0->unk_30, 0);
    ManagedSprite_TickFrame(arg0->unk_30);
    ov07_0221C69C();
}
