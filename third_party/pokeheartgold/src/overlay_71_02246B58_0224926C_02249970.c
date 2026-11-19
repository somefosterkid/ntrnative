#include "global.h"

#include "sprite.h"
#include "sys_task_api.h"

s32 ov71_022472FC(void *);

void ov71_02249970(void *arg0);

void ov71_02249970(void *arg0) {
    Sprite *sprite;

    sprite = *(Sprite **)((u8 *)arg0 + 0x20);
    if (sprite != NULL) {
        Sprite_Delete(sprite);
    }
    ov71_022472FC(arg0 + 0x10);
}

void ov71_02249988(void *arg0);

void ov71_02249988(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 0;
}

void ov71_02249990(void *arg0);

void ov71_02249990(void *arg0) {
    SysTask *task;

    task = *(SysTask **)((u8 *)arg0 + 4);
    if (task != NULL) {
        SysTask_Destroy(task);
        *(SysTask **)((u8 *)arg0 + 4) = NULL;
    }
}
