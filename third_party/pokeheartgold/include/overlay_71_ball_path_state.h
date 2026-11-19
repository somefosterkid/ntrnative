#ifndef POKEHEARTGOLD_OVERLAY_71_BALL_PATH_STATE_H
#define POKEHEARTGOLD_OVERLAY_71_BALL_PATH_STATE_H

// BallPathState: merged by tools/hg/promote.py from 14 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02247990_02247990.c) and 32 accesses; fields at every accessed offset, pads elsewhere.

struct Trade3DModel;
struct Trade3DScene;

typedef struct BallPathState {
    struct Trade3DModel *model;
    struct Trade3DScene *scene;
    struct { fx32 x; fx32 y; fx32 z; } orbitCenter;
    struct { fx32 x; fx32 y; fx32 z; } currentPos;
    struct { fx16 x; fx16 y; fx16 z; } rotation;
    u8 pad_26[0x2];
    fx32 currentY;
    fx32 yStep;
    fx32 yAccel;
    fx32 unused_34;
    fx32 orbitRadiusX;
    fx32 orbitRadiusZ;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusZStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusZ;
    u32 orbitTransitionFrames;
    s16 angleAccum;
    s16 rotationSpeed;
    s16 targetRotationSpeed;
    s16 rotationSpeedStep;
    struct { fx16 currentValue; fx16 step; fx16 targetValue; u16 framesRemaining; } axisRotAnimators[3];
    u32 rotSpeedTransitionFrames;
    BOOL exitedView;
    BOOL midpointReached;
    int entryFramesRemaining;
} BallPathState;

// sizeof(BallPathState) == 0x84

#endif // POKEHEARTGOLD_OVERLAY_71_BALL_PATH_STATE_H
