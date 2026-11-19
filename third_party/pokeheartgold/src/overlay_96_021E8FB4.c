#include "global.h"

#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "sys_task_api.h"

void ov96_021E8FB4(SysTask *task, void *data);

void ov96_021E8FB4(SysTask *task, void *data) {
    Ov96SpriteResLoadTask *w = data;

    do {
        if (ov96_021E8FE0(w) != 0) {
            w->man->unk_C = 1;
            Heap_Free(w);
            SysTask_Destroy(task);
            return;
        }
    } while (w->params.runToEnd != 0);
}
