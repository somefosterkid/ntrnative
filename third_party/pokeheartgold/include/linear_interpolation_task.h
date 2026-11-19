#ifndef POKEHEARTGOLD_LINEAR_INTERPOLATION_TASK_H
#define POKEHEARTGOLD_LINEAR_INTERPOLATION_TASK_H

#include "global.h"

typedef struct LinearInterpolationTaskS32 {
    int currentValue;
    int startValue;
    int delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskS32;

// sizeof(LinearInterpolationTaskS32) == 0x14

typedef struct LinearInterpolationTaskFX32 {
    fx32 currentValue;
    fx32 startValue;
    fx32 delta;
    int currentStep;
    int numSteps;
} LinearInterpolationTaskFX32;

// sizeof(LinearInterpolationTaskFX32) == 0x14

#endif // POKEHEARTGOLD_LINEAR_INTERPOLATION_TASK_H
