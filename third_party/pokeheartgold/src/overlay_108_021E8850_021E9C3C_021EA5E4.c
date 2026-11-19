#include "global.h"

#include "heap.h"
#include "overlay_108_021E8850.h"
#include "sys_task_api.h"
#include "overlay_108_safari_decoration_app.h"
void ov108_021EA5E4(SysTask *task, void *data) {
    SafariDecorationSelectTask *ptr = data;
    s32 *v2;
    u8 v1;
    SafariDecorationApp *v0;

    v1 = ptr->frame;
    ptr->frame = (u8)(v1 + 1);
    if ((u32)v1 >= 2U) {
        ov108_021EA040(ptr->app);
        ov108_021EA47C(ptr->app);
        v0 = ptr->app;
        ov108_021EA2EC(v0, v0->cursor);
        v2 = ptr->pendingTasks;
        *v2 -= 1;
        Heap_Free(ptr);
        SysTask_Destroy(task);
    }
}

void ov108_021EA624(SafariDecorationApp *arg0) {
    arg0->viewDirty = 1;
    ov108_021E8ED8(&arg0->view, 0);
}
