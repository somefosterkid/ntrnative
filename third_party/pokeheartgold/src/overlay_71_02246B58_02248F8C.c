#include "global.h"
#include "overlay_71_ball_path_state.h"
struct BallPathState;

void ov71_02248F8C(struct BallPathState *arg0, s16 arg1, s32 arg2);

void ov71_02248F8C(BallPathState *arg0, s16 arg1, s32 arg2) {
    if (arg2 != 0) {
        arg0->rotationSpeedStep = (s16)((s32)(arg1 - arg0->rotationSpeed) / arg2);
        arg0->rotationSpeed = (s16)(arg0->rotationSpeed + arg0->rotationSpeedStep);
    } else {
        arg0->rotationSpeed = arg1;
    }
    arg0->targetRotationSpeed = arg1;
    *((s32 *)&arg0->rotSpeedTransitionFrames) = arg2;
}

void ov71_02248FC8(struct BallPathState *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov71_02248FC8(BallPathState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg3 != 0) {
        *((s32 *)&arg0->orbitRadiusXStep) = (s32)((s32)(arg1 - *((s32 *)&arg0->orbitRadiusX)) / arg3);
        *((s32 *)&arg0->orbitRadiusZStep) = (s32)((s32)(arg2 - *((s32 *)&arg0->orbitRadiusZ)) / arg3);
        *((s32 *)&arg0->orbitRadiusX) = (s32)(*((s32 *)&arg0->orbitRadiusX) + *((s32 *)&arg0->orbitRadiusXStep));
        *((s32 *)&arg0->orbitRadiusZ) = (s32)(*((s32 *)&arg0->orbitRadiusZ) + *((s32 *)&arg0->orbitRadiusZStep));
        *((s32 *)&arg0->targetOrbitRadiusX) = arg1;
        *((s32 *)&arg0->targetOrbitRadiusZ) = arg2;
    }
    *((s32 *)&arg0->orbitTransitionFrames) = arg3;
}
