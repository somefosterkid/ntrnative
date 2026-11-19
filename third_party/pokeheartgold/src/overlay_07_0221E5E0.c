#include "global.h"
#include "heap.h"
#include "bg_window.h"
#include "overlay_07_battle_bg_anim.h"

void ov07_0221DEC0(void *);

u32 ov07_0221E5E0(u32 param_1, void *param_2);

struct BgConfig;
struct SysTask;
struct SysTaskQueue;
struct WaveAnimContext;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void SysTask_Destroy(SysTask *task);
void ov07_0221E600(SysTask *task, void *param);

u32 ov07_0221E5E0(u32 param_1, void *param_2) {
    ov07_0221DEC0(param_2);
    return 0;
}

void ov07_0221E664(void *);

u32 ov07_0221E5EC(u32 param_1, void *param_2);

u32 ov07_0221E5EC(u32 param_1, void *param_2) {
    ov07_0221DEC0(param_2);
    ov07_0221E664(param_2);
    return 0;
}

void ov07_0221E600(SysTask *task, void *param) {
    BattleBgAnim *bgAnim = param;

    if (bgAnim->cancel == 1) {
        Heap_Free(bgAnim);
        SysTask_Destroy(task);
        return;
    }

    bgAnim->offsetX += bgAnim->stepX;
    bgAnim->offsetY += bgAnim->stepY;

    if (bgAnim->stepX != 0) {
        BgSetPosTextAndCommit(bgAnim->bgConfig, bgAnim->bg, BG_POS_OP_SET_X, bgAnim->offsetX);
    }

    if (bgAnim->stepY != 0) {
        BgSetPosTextAndCommit(bgAnim->bgConfig, bgAnim->bg, BG_POS_OP_SET_Y, bgAnim->offsetY);
    }
}
