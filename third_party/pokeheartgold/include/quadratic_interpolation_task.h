#ifndef POKEHEARTGOLD_QUADRATIC_INTERPOLATION_TASK_H
#define POKEHEARTGOLD_QUADRATIC_INTERPOLATION_TASK_H

#include "global.h"

typedef struct QuadraticInterpolationTaskFX32 {
    fx32 currentValue;
    fx32 startValue;
    fx32 initialRate;
    fx32 quadraticCoeff;
    int currentStep;
    int numSteps;
} QuadraticInterpolationTaskFX32;

// sizeof(QuadraticInterpolationTaskFX32) == 0x18

#endif // POKEHEARTGOLD_QUADRATIC_INTERPOLATION_TASK_H
