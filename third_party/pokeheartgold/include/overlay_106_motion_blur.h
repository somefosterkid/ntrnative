#ifndef POKEHEARTGOLD_OVERLAY_106_MOTION_BLUR_H
#define POKEHEARTGOLD_OVERLAY_106_MOTION_BLUR_H

#include "global.h"

#include "sys_task.h"

typedef struct MotionBlurParams {
    GXDispMode displayMode;
    GXBGMode bgMode;
    GXBG0As bg0Mode;
    GXCaptureSize captureSize;
    GXCaptureMode captureMode;
    GXCaptureSrcA captureSourceA;
    GXCaptureSrcB captureSourceB;
    GXCaptureDest captureDestination;
    int blendCoeffA;
    int blendCoeffB;
    enum HeapID heapID;
} MotionBlurParams;

typedef struct MotionBlur {
    GXVRamLCDC lcdcVRamBank;
    MotionBlurParams params;
    BOOL initialized;
    SysTask *task;
} MotionBlur;

#endif // POKEHEARTGOLD_OVERLAY_106_MOTION_BLUR_H
