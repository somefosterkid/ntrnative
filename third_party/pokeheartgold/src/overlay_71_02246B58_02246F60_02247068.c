#include "global.h"

#include "bg_window.h"
#include "sys_task_api.h"

typedef struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
} BgScaleAnimation;

void ov71_02247424(void *param);

void ov71_022470DC(BgScaleAnimation *bgScaleAnim);

void ov71_02247068(SysTask *task, void *param);

void ov71_02247068(SysTask *task, void *param) {
    BgScaleAnimation *bgScaleAnim = param;
    fx32 step;

    step = ((bgScaleAnim->currentScale * bgScaleAnim->rate) >> FX32_SHIFT) / 32;

    switch (bgScaleAnim->direction) {
    case 0:
        if ((bgScaleAnim->currentScale + step) < bgScaleAnim->targetScale) {
            bgScaleAnim->currentScale += step;
        } else {
            bgScaleAnim->currentScale = bgScaleAnim->targetScale;
            *(bgScaleAnim->done) = 1;
        }
        break;
    case 1:
        if ((bgScaleAnim->currentScale - step) > bgScaleAnim->targetScale) {
            bgScaleAnim->currentScale -= step;
        } else {
            bgScaleAnim->currentScale = bgScaleAnim->targetScale;
            *(bgScaleAnim->done) = 1;
        }
        break;
    }

    ov71_022470DC(bgScaleAnim);

    if (*(bgScaleAnim->done)) {
        ov71_02247424(bgScaleAnim);
        SysTask_Destroy(task);
    } else {
        bgScaleAnim->rate += bgScaleAnim->acceleration;
    }
}
