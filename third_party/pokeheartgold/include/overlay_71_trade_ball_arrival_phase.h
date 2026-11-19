#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_ARRIVAL_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_ARRIVAL_PHASE_H

// TradeBallArrivalPhase: merged by tools/hg/promote.py from 6 m2c local copies (1 variants, base
// src/overlay_71_02246B58_0224926C_02249D28_02249D28.c) and 8 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct Sprite;
struct SysTask;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;

typedef struct TradeBallArrivalPhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int unused_08;
    BOOL screenSwapPending;
    struct { void *cellBuffer; void *animBuffer; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *animBank; } animResources;
    struct Sprite *sprite;
    struct BgConfig *bgConfig;
    struct Trade3DScene *scene;
    struct Trade3DModel *model;
    struct SysTask *task;
    struct { void *arrivalPhase; struct SysTask *task; struct Trade3DScene *scene; struct Trade3DModel *model; struct { fx32 x; fx32 y; fx32 z; } orbitCenter; struct { fx32 x; fx32 y; fx32 z; } orbitOffset; struct { fx32 x; fx32 y; fx32 z; } modelPos; struct { fx16 x; fx16 y; fx16 z; } rotation; u8 unk_3C_pad[0x2]; fx32 orbitCenterStepY; fx32 orbitCenterStepZ; fx32 orbitRadiusX; fx32 orbitRadiusY; fx32 orbitRadiusXStep; fx32 orbitRadiusYStep; fx32 targetOrbitRadiusX; fx32 targetOrbitRadiusY; fx32 scale; fx32 scaleStep; fx32 targetScale; int transitionFrames; int subState; int exitTimer; int frameCount; fx16 rotationAngle; fx16 rotationSpeed; } bapState;
} TradeBallArrivalPhase;

// sizeof(TradeBallArrivalPhase) == 0xb0

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_ARRIVAL_PHASE_H
