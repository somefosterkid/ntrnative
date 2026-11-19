#include "global.h"

#include "math_util.h"
#include "overlay_71_ball_path_state.h"
#include "overlay_71_trade3dmodel.h"

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

void ov71_02249004(BallPathState *bpState, int axis, fx32 targetValue, int duration);

void ov71_02249004(BallPathState *bpState, int axis, fx32 targetValue, int duration) {
    if (duration) {
        bpState->axisRotAnimators[axis].step = (targetValue - bpState->axisRotAnimators[axis].currentValue) / duration;
    }

    bpState->axisRotAnimators[axis].targetValue = targetValue;
    bpState->axisRotAnimators[axis].framesRemaining = duration;
}

void ov71_0224903C(BallPathState *bpState);

void ov71_0224903C(BallPathState *bpState) {
    int angleDegrees;

    if (bpState->rotSpeedTransitionFrames) {
        bpState->rotSpeedTransitionFrames--;

        if (bpState->rotSpeedTransitionFrames) {
            bpState->rotationSpeed += bpState->rotationSpeedStep;
        } else {
            bpState->rotationSpeed = bpState->targetRotationSpeed;
        }
    }

    bpState->angleAccum += bpState->rotationSpeed;
    angleDegrees = (u16)(bpState->angleAccum) / (65536 / 360);

    if (angleDegrees >= 360) {
        angleDegrees -= 360;
    }

    if (bpState->orbitTransitionFrames) {
        bpState->orbitRadiusX += bpState->orbitRadiusXStep;
        bpState->orbitRadiusZ += bpState->orbitRadiusZStep;
        bpState->orbitTransitionFrames--;

        if (bpState->orbitTransitionFrames == 0) {
            bpState->orbitRadiusX = bpState->targetOrbitRadiusX;
            bpState->orbitRadiusZ = bpState->targetOrbitRadiusZ;
        }
    }

    bpState->currentPos.x = bpState->orbitCenter.x + FX_Mul(GF_CosDegNoWrap(angleDegrees), bpState->orbitRadiusX);
    bpState->currentPos.z = bpState->orbitCenter.z + FX_Mul(GF_SinDegNoWrap(angleDegrees), bpState->orbitRadiusZ);
    bpState->rotation.y += bpState->rotationSpeed;

    for (int i = 0; i < 3; i++) {
        if (bpState->axisRotAnimators[i].framesRemaining) {
            bpState->axisRotAnimators[i].framesRemaining--;

            if (bpState->axisRotAnimators[i].framesRemaining == 0) {
                bpState->axisRotAnimators[i].currentValue = bpState->axisRotAnimators[i].targetValue;
            } else {
                bpState->axisRotAnimators[i].currentValue += bpState->axisRotAnimators[i].step;
            }
        }
    }

    bpState->rotation.x += bpState->axisRotAnimators[0].currentValue;
    bpState->rotation.y += bpState->axisRotAnimators[1].currentValue;
    bpState->rotation.z += bpState->axisRotAnimators[2].currentValue;
}
