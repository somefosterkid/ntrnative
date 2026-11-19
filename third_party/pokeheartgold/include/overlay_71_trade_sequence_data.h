#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE_SEQUENCE_DATA_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE_SEQUENCE_DATA_H

#include "g2d_renderer.h"

// TradeSequenceData: merged by tools/hg/promote.py from 16 m2c local copies (1 variants, base
// src/overlay_71_02246B58_02246BB8.c) and 5 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct MessageFormat;
struct MsgData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct SpriteList;
struct String;
struct SysTask;
struct TradeAnimationTemplate;

typedef struct TradeSequenceData {
    struct TradeAnimationTemplate *animationTemplate;
    int currentPhase;
    struct BgConfig *bgConfig;
    struct MessageFormat *strTemplate;
    struct MsgData *msgLoader;
    struct String *strBuffer;
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct SysTask *task;
    BOOL flipSprite;
    u16 sendingSpecies;
    u16 sendingForm;
    u16 receivingSpecies;
    u16 receivingForm;
    void *currentPhaseData;
} TradeSequenceData;

// sizeof(TradeSequenceData) == 0x158

#ifdef __MWERKS__
// DS layout check: the pointer fields are 4 bytes there (the PC build's are wider)
typedef char TradeSequenceData_size_check[(sizeof(TradeSequenceData) == 0x158) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE_SEQUENCE_DATA_H
