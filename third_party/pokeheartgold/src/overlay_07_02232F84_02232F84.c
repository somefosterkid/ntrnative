#include "global.h"
#include "overlay_07_ball_rotation.h"

s32 ov07_02232F74(void *, s32);

s32 ov07_022335B4(BallRotation *param0);

s32 ov07_02232F84(void *arg0);

s32 ov07_02232F84(void *arg0) {
    if (ov07_022335B4(arg0) == 0) {
        ov07_02232F74(arg0, 2);
    }
    return 1;
}

s32 ov07_02232F9C(void *arg0);

s32 ov07_02232F9C(void *arg0) {
    ov07_02232F74(arg0, 3);
    return 1;
}
