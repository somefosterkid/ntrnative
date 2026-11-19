#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_fade_bg_context.h"
#include "palette.h"
#include "sys_task.h"

void ov07_02223E94(SysTask *arg0, void *ptr);

void ov07_02223E94(SysTask *arg0, void *ptr) {
    FadeBgContext *ctx = ptr;

    if (PaletteData_GetSelectedBuffersBitmask(ctx->paletteData) == 0) {
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, arg0);
    }
}
