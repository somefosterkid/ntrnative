#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_TUBE_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_TUBE_PHASE_H

// TradeTubePhase: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02247990_02247990.c) and 9 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct BgScaleAnimation;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct PaletteShimmerEffect;
struct Sprite;
struct SysTask;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;

typedef struct TradeTubePhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    struct { void *cellBuffer; void *animBuffer; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *animBank; } animResources;
    struct Sprite *sprites[20][2];
    u32 rngSeed;
    struct BgConfig *bgConfig;
    struct PaletteShimmerEffect *shimmerEffect;
    BOOL shimmerActive;
    struct BgScaleAnimation *bgScaleAnim;
    BOOL bgScaleAnimDone;
    struct Trade3DScene *scene;
    struct Trade3DModel *model;
    struct { struct Trade3DModel *model; struct Trade3DScene *scene; struct { fx32 x; fx32 y; fx32 z; } orbitCenter; struct { fx32 x; fx32 y; fx32 z; } currentPos; struct { fx16 x; fx16 y; fx16 z; } rotation; u8 unk_28_pad[0x2]; fx32 currentY; fx32 yStep; fx32 yAccel; fx32 unused_34; fx32 orbitRadiusX; fx32 orbitRadiusZ; fx32 orbitRadiusXStep; fx32 orbitRadiusZStep; fx32 targetOrbitRadiusX; fx32 targetOrbitRadiusZ; u32 orbitTransitionFrames; s16 angleAccum; s16 rotationSpeed; s16 targetRotationSpeed; s16 rotationSpeedStep; struct { fx16 currentValue; fx16 step; fx16 targetValue; u16 framesRemaining; } axisRotAnimators[3]; u32 rotSpeedTransitionFrames; BOOL exitedView; BOOL midpointReached; int entryFramesRemaining; } ballAnim;
    struct SysTask *task0;
    struct SysTask *task1;
    BOOL screenSwapPending;
} TradeTubePhase;

// sizeof(TradeTubePhase) == 0x16c

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_TUBE_PHASE_H
