#ifndef POKEHEARTGOLD_OVERLAY_116_H
#define POKEHEARTGOLD_OVERLAY_116_H

#include "global.h"
#include "linear_interpolation_task.h"
#include "overlay_01_021F467C.h"
#include "quadratic_interpolation_task.h"
#include "sys_task.h"

// Tuning of the Lugia / Ho-Oh encounter effect (the rodata tables in asm/overlay_116.s)
typedef struct LugiaHoOhEncounterParam {
    u8 perspectiveSteps;
    u8 delayFrames;
    u8 distanceSteps;
    u8 fadeFrames;
    s32 perspectiveDelta;
    fx32 distanceDelta;
    fx32 distanceInitialRate;
    u16 blendCoeffA;
    u16 blendCoeffB;
} LugiaHoOhEncounterParam; // 0x14

// EncounterEffect::param of the Lugia / Ho-Oh encounter effect (0x38 on the DS)
typedef struct LugiaHoOhEncounterEffect {
    FieldMotionBlur *motionBlur;
    int unused_04;
    LinearInterpolationTaskS32 perspectiveTask;
    QuadraticInterpolationTaskFX32 distanceTask;
    int delay;
} LugiaHoOhEncounterEffect;

void ov116_0225F364(SysTask *arg0, void *arg1);
void ov116_0225F374(SysTask *arg0, void *arg1);

#endif // POKEHEARTGOLD_OVERLAY_116_H
