#include "global.h"

#include "bg_window.h"

struct BgConfig;

struct BgScaleAnimation;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

void ov71_022470DC(BgScaleAnimation *bgScaleAnim);

void ov71_022470DC(BgScaleAnimation *bgScaleAnim) {
    static const int sCenterY[] = {
        144,
        56,
    };
    MtxFx22 mtx;

    mtx._00 = bgScaleAnim->currentScale;
    mtx._01 = 1 << FX32_ONE;
    mtx._10 = 0;
    mtx._11 = bgScaleAnim->currentScale;

    SetBgAffine(bgScaleAnim->bgConfig, 2, &mtx, 128, sCenterY[bgScaleAnim->centerIndex]);
    SetBgAffine(bgScaleAnim->bgConfig, 6, &mtx, 128, sCenterY[bgScaleAnim->centerIndex]);
}
