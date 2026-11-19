#include "global.h"

#include "bg_window.h"
#include "sys_task.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E7AD8(MoveRelearner *arg0);

s32 ov68_021E6C14(MoveRelearner *, s32);
s32 ov68_021E7B6C(MoveRelearner *arg0);

s32 ov68_021E7B94(MoveRelearner *arg0);
s32 ov68_021E7B8C(MoveRelearner *arg0);

s32 ov68_021E7AD8(MoveRelearner *arg0) {
    u16 v0;
    MoveRelearnerButtonFlash *v1;

    v1 = &arg0->buttonFlash;
    v0 = v1->state;
    switch (v0) {
    case 0:
        BgTilemapRectChangePalette(arg0->bgConfig, 7U, v1->x, v1->y, v1->width, v1->height, 7U);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 7U);
        v1->state = (u16)(v1->state + 1);
    default:
        break;
    case 1:
        v1->timer = (u16)(v1->timer + 1);
        if (v1->timer == 4) {
            BgTilemapRectChangePalette(arg0->bgConfig, 7U, v1->x, v1->y, v1->width, v1->height, 0U);
            ScheduleBgTilemapBufferTransfer(arg0->bgConfig, 7U);
            v1->timer = 0U;
            v1->state = (u16)(v1->state + 1);
        }
        break;
    case 2:
        v1->timer = (u16)(v1->timer + 1);
        if (v1->timer == 2) {
            return v1->nextState;
        }
        break;
    }
    return 0xD;
}

s32 ov68_021E7B6C(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 2);
    arg0->question = 1;
    arg0->nextState = 3;
    return 2;
}

s32 ov68_021E7B8C(MoveRelearner *arg0) {
    return ov68_021E7B94(arg0);
}
