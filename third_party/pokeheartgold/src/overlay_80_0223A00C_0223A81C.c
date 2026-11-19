#include "global.h"

#include "sprite.h"
#include "unk_02009D48.h"

void Sprite_DeleteAndFreeResources(Sprite *gfxElem);
typedef struct LinearInterpolationTaskFX32 {
    fx32 currentValue;
    fx32 startValue;
    fx32 delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskFX32;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    Sprite *unk_04[4];
    LinearInterpolationTaskFX32 unk_14[4];
} UnkStruct_ov5_021E5128;

void ov80_0223A81C(UnkStruct_ov5_021E5128 *param0);

typedef struct FrontierBrainVsSpriteAnimation {
    s16 taskStartDelay;
    s16 tasksStarted;
    ManagedSprite *vsSprites[4];
    LinearInterpolationTaskFX32 tasks[4];
} FrontierBrainVsSpriteAnimation;
BOOL ov80_0223A8D4(LinearInterpolationTaskFX32 *task);
VecFx32 ov80_0223A91C(fx32 x, fx32 y, fx32 z);
BOOL ov80_0223A834(FrontierBrainVsSpriteAnimation *vsSpriteAnimation);

void ov80_0223A81C(UnkStruct_ov5_021E5128 *param0) {
    int v0;

    for (v0 = 0; v0 < 4; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_04[v0]);
    }
}

BOOL ov80_0223A834(FrontierBrainVsSpriteAnimation *vsSpriteAnimation) {
    int i;
    BOOL allTasksFinished = 1;
    VecFx32 scale;

    if (vsSpriteAnimation->tasksStarted < 4) {
        allTasksFinished = 0;
        vsSpriteAnimation->taskStartDelay--;

        if (vsSpriteAnimation->taskStartDelay <= 0) {
            vsSpriteAnimation->taskStartDelay = 3;
            vsSpriteAnimation->tasksStarted++;
        }
    }

    for (i = 0; i < vsSpriteAnimation->tasksStarted; i++) {
        BOOL isFinished = ov80_0223A8D4(&vsSpriteAnimation->tasks[i]);
        scale = ov80_0223A91C(vsSpriteAnimation->tasks[i].currentValue, vsSpriteAnimation->tasks[i].currentValue, vsSpriteAnimation->tasks[i].currentValue);

        Sprite_SetAffineScale(vsSpriteAnimation->vsSprites[i]->sprite, &scale);
        Sprite_SetDrawFlag(vsSpriteAnimation->vsSprites[i]->sprite, 1);

        if (!isFinished) {
            allTasksFinished = 0;
        }
    }

    return allTasksFinished;
}
