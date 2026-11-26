#include "global.h"

#include "sys_task.h"
#include "overlay_71_ball_path_state.h"

s32 ov71_022476C4(void *, void *);

s32 ov71_022476EC(void *, void *);

s32 ov71_02248FC8(void *, s32, s32, s32);

s32 ov71_02249004(void *, s32, s32, s32);

s32 ov71_0224903C(void *);

void ov71_02249208(SysTask *task, void *data);

void ov71_02249184(SysTask *arg0, void *arg1);

void ov71_02249184(SysTask *arg0, void *arg1) {
    s32 v0;

    if (*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) != 0) {
        v0 = *((s32 *)&((BallPathState *)arg1)->currentY) + *((s32 *)&((BallPathState *)arg1)->yStep);
        *((s32 *)&((BallPathState *)arg1)->currentY) = v0;
        *((s32 *)&((BallPathState *)arg1)->currentPos.y) = v0;
        *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) = (s32)(*((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) - 1);
        if ((s32) *((s32 *)&((BallPathState *)arg1)->entryFramesRemaining) <= 0) {
            *((s32 *)&((BallPathState *)arg1)->yAccel) = 0x200;
            *((s32 *)&((BallPathState *)arg1)->yStep) = 0;
            ov71_02248FC8(arg1, 0x4000, 0x4000, 0x3C);
            ov71_02249004(arg1, 0, 0xFFFFF800, 0x1E);
            *((s32 *)&((BallPathState *)arg1)->midpointReached) = 1;
            SysTask_SetFunc(arg0, ov71_02249208);
        }
    }
    ov71_0224903C(arg1);
    ov71_022476C4(*(void **)arg1, &((BallPathState *)arg1)->currentPos);
    ov71_022476EC(*(void **)arg1, &((BallPathState *)arg1)->rotation);
}
