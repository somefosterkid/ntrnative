#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"

s32 ov96_021EAC5C(u32 *, s32);

void ov96_0220B8F0(void *arg0);

void ov96_0220B8F0(void *arg0) {
    s32 sp0;
    SysTask *temp_r0_2;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r7;
    void *var_r5;
    void *var_r6;

    var_r6 = arg0;
    sp0 = 0;
    var_r6 += 0x14;
    var_r7 = 0;
    do {
        ov96_021EAC5C(*(void **)((u8 *)(var_r6 + (((u32)(*(s32 *)((u8 *)var_r6 + 0x38) << 7) >> 0x1E) * 4)) + 4), 0xC);
        temp_r0_2 = *(SysTask **)((u8 *)var_r6 + 0x34);
        if (temp_r0_2 != NULL) {
            SysTask_Destroy(temp_r0_2);
            *(SysTask **)((u8 *)var_r6 + 0x34) = NULL;
        }
        var_r4 = 0;
        var_r5 = var_r6;
loop_4:
        ManagedSprite_SetAnimateFlag(*(ManagedSprite **)((u8 *)var_r5 + 0x1C), 0);
        var_r4 += 1;
        var_r5 += 4;
        if (var_r4 < 6) {
            goto loop_4;
        }
        var_r6 += 0x3C;
        temp_r0 = sp0 + 1;
        sp0 = temp_r0;
    } while (temp_r0 < 3);
}
