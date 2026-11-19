#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov96_021E8FB4(SysTask *task, void *data);

void ov96_021E8B1C(Ov96SpriteResMan *man, s32 count, Ov96SpriteResDesc *descs, Ov96SpriteResLoadParams *params, u32 priority, s32 baseIndex) {
    Ov96SpriteResLoadTask *w;
    s32 i;
    Ov96SpriteResDesc *src = descs;
    Ov96SpriteResLoadParams *s44 = params;

    w = Heap_AllocAtEnd(man->heapID, sizeof(Ov96SpriteResLoadTask));
    w->count = count;
    w->man = man;
    w->params = *s44;
    i = 0;
    w->state = 0;
    w->baseIndex = baseIndex;
    w->pos = 0;
    man->unk_C = 0;
    if (count > 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        Ov96SpriteResDesc *dst = w->descs;
        do {
            *dst = *src;
            i++;
            src++;
            dst++;
        } while (i < count);
#else
        u8 *dst = (u8 *)w;
        do {
            *(Ov96SpriteResDesc *)(dst + 0x54) = *src;
            i++;
            src++;
            dst += 0x10;
        } while (i < count);
#endif
    }
    SysTask_CreateOnMainQueue(ov96_021E8FB4, w, priority);
}
