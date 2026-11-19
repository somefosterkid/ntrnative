#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_FLYBY_PHASE_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_FLYBY_PHASE_H

// TradeBallFlybyPhase: merged by tools/hg/promote.py from 5 m2c local copies (1 variants, base
// src/overlay_71_02246B58_0224926C_02249500_02249500.c) and 10 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct Sprite;
struct SysTask;
struct Trade3DModel;
struct Trade3DScene;
struct TradeSequenceData;

typedef struct TradeBallFlybyPhase {
    struct TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    BOOL screenSwapPending;
    struct { void *cellBuffer; void *animBuffer; struct NNSG2dCellDataBank *cellBank; struct NNSG2dAnimBankData *animBank; } animResources;
    struct Sprite *sprite;
    struct BgConfig *bgConfig;
    struct Trade3DScene *scene;
    struct Trade3DModel *model;
    struct SysTask *unused_30;
    struct { void *phase; struct SysTask *taskHandle; struct Trade3DScene *scene; struct Trade3DModel *model; struct { fx32 x; fx32 y; fx32 z; } center; struct { fx32 x; fx32 y; fx32 z; } orbitOffset; struct { fx32 x; fx32 y; fx32 z; } currentPos; struct { fx16 x; fx16 y; fx16 z; } rotation; u8 unk_3C_pad[0x2]; fx32 centerYStep; fx32 centerZStep; fx32 orbitRadiusX; fx32 orbitRadiusY; fx32 orbitRadiusXStep; fx32 orbitRadiusYStep; fx32 scale; fx32 scaleStep; int transitionFrames; int state; int timer; int frameCount; fx16 angle; fx16 angleSpeed; } ballOrbit;
} TradeBallFlybyPhase;

// sizeof(TradeBallFlybyPhase) == 0xa4

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_BALL_FLYBY_PHASE_H
