#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

typedef struct BgScaleAnimation BgScaleAnimation;

struct BgScaleAnimation {
    BgConfig *bgConfig;
    volatile BOOL *done;
    fx32 currentScale;
    fx32 targetScale;
    fx32 rate;
    fx32 acceleration;
    int direction;
    int centerIndex;
    SysTask *task;
};

void ov71_02247068(SysTask *task, void *data);

BgScaleAnimation *ov71_02247000(BgConfig *bgConfig, s32 currentScale, s32 targetScale, s32 rate, s32 acceleration, s32 centerIndex, s32 *done);

BgScaleAnimation *ov71_02247000(BgConfig *bgConfig, s32 currentScale, s32 targetScale, s32 rate, s32 acceleration, s32 centerIndex, s32 *done) {
    s32 sp4;
    BgConfig *sp0;
    BgScaleAnimation *var_r4;
    SysTask *temp_r0;
    s32 *var_r5;
    s32 var_r0;
    s32 var_r6;
    s32 var_r7;

    sp0 = bgConfig;
    var_r6 = currentScale;
    var_r7 = targetScale;
    sp4 = rate;
    var_r5 = done;
    var_r4 = Heap_Alloc(HEAP_ID_56, sizeof(BgScaleAnimation));
    var_r0 = 1;
    *var_r5 = 1;
    if (var_r4 != NULL) {
        var_r4->bgConfig = sp0;
        var_r4->currentScale = var_r6;
        var_r4->targetScale = var_r7;
        var_r4->rate = sp4;
        var_r4->acceleration = acceleration;
        if (var_r6 < var_r7) {
            var_r0 = 0;
        }
        var_r4->direction = var_r0;
        var_r4->centerIndex = centerIndex;
        var_r4->done = var_r5;
        *var_r5 = 0;
        temp_r0 = SysTask_CreateOnVBlankQueue(ov71_02247068, var_r4, 0U);
        var_r4->task = temp_r0;
        if (temp_r0 == NULL) {
            *var_r5 = 1;
            Heap_Free(var_r4);
        }
    }
    return var_r4;
}
