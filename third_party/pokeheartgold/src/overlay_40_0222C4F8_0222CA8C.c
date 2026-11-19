#include "global.h"

#include "overlay_40.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov40_0222CA8C(void *arg0) {
    s32 v0;
    s32 v1;
    s32 v2;
    void *v3;
    void *v4;

    v4 = arg0;
    v0 = 0;
    do {
        v2 = 0;
        v3 = v4;
loop_2:
        Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)v3 + 0x6F8));
        v2 += 1;
        v3 += 4;
        if (v2 < 2) {
            goto loop_2;
        }
        v4 += 0x30;
        v1 = v0 + 1;
        v0 = v1;
    } while (v1 < 6);
}

void ov40_0222CABC(void *arg0) {
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        SysTask_Destroy(*(SysTask **)((u8 *)v1 + 0x700));
        v0 += 1;
        v1 += 0x30;
    } while (v0 < 6);
}
