#include "global.h"

#include "sys_task.h"
#include "sys_task_api.h"

#include "overlay_96_ov96_arena_work.h"

void ov96_021F2CD0(SysTask *task, void *param);

void ov96_021F2CD0(SysTask *task, void *param) {
    Ov96ArenaFx *env = param;
    s32 v;
    switch (env->unk_02) {
    case 0:
        env->unk_1A++;
        v = (env->unk_1A << 7) / 5;
        reg_G2_BG2OFS = ((0x80 - v) << 16) & 0x1ff0000;
        if (env->unk_1A >= 5) {
            env->unk_1A = 0;
            env->unk_02++;
        }
        break;
    case 1:
        env->unk_1A++;
        if (env->unk_1A >= 20) {
            env->unk_1A = 0;
            env->unk_02++;
        }
        break;
    case 2:
        env->unk_1A++;
        v = -(env->unk_1A << 7) / 5;
        reg_G2_BG2OFS = (v << 16) & 0x1ff0000;
        if (env->unk_1A >= 5) {
            env->unk_1A = 0;
            env->unk_08 = NULL;
            env->unk_02 = 0;
            SysTask_Destroy(task);
        }
        break;
    }
}
