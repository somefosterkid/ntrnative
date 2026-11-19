#ifndef POKEHEARTGOLD_OVERLAY_01_021F467C_H
#define POKEHEARTGOLD_OVERLAY_01_021F467C_H

#include "global.h"

#include "constants/heap.h"

#include "overlay_106_motion_blur.h"

typedef struct FieldMotionBlur {
    MotionBlur *motionBlur;
} FieldMotionBlur;

FieldMotionBlur *ov01_021F467C(int coeffA, int coeffB);
void ov01_021F46DC(FieldMotionBlur **fieldMotionBlur);

#endif // POKEHEARTGOLD_OVERLAY_01_021F467C_H
