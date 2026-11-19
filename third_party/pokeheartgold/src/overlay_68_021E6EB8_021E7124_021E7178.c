#include "global.h"
#include "camera.h"
#include "overlay_68_move_relearner.h"
#include "sprite_system.h"

void ov68_021E6DDC(MoveRelearner *relearner);
void ov68_021E6EB8(MoveRelearner *relearner, NARC *narc);

extern const ManagedSpriteTemplate ov68_021E7E74[MOVE_RELEARNER_NUM_SPRITES];

void ov68_021E7178(MoveRelearner *relearner, NARC *narc);

void ov68_021E7178(MoveRelearner *relearner, NARC *narc) {
    u32 i;

    ov68_021E6DDC(relearner);
    ov68_021E6EB8(relearner, narc);
    for (i = 0; i < MOVE_RELEARNER_NUM_SPRITES; i++) {
        relearner->sprites[i] = SpriteSystem_NewSprite(relearner->spriteSystem, relearner->spriteManager, &ov68_021E7E74[i]);
        ManagedSprite_SetAnimateFlag(relearner->sprites[i], FALSE);
    }
}

void ov68_021E71C4(Camera **camera, enum HeapID heapID);

extern const VecFx32 ov68_021E7BEC;

void ov68_021E71C4(Camera **camera, enum HeapID heapID) {
    VecFx32 pos = ov68_021E7BEC;
    CameraAngle angle = { 0, 0, 0 };

    *camera = Camera_New(heapID);
    Camera_Init_FromPosDistanceAndAngle(&pos, FX32_CONST(16), &angle, 0x5C1, 1, *camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), *camera);
    Camera_ClearFixedTarget(*camera);
    Camera_SetStaticPtr(*camera);
}
