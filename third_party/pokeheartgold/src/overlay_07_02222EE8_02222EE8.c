#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_palette_fade.h"
#include "sys_task_api.h"

s32 ov07_02222EE8(PaletteFadeContext *arg0);

s32 ov07_02222EE8(PaletteFadeContext *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return arg0->active;
}

void ov07_02222EF8(PaletteFadeContext *arg0);

void ov07_02222EF8(PaletteFadeContext *ptr) {
    if (ptr == NULL) {
        GF_AssertFail();
    }
    SysTask_Destroy(ptr->task);
    Heap_Free(ptr);
}
