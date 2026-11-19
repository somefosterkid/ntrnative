#ifndef POKEHEARTGOLD_SIGNATURE_APP_H
#define POKEHEARTGOLD_SIGNATURE_APP_H
#include "bg_window.h"
#include "sprite_resources_header.h"

#include "g2d_renderer.h"

// SignatureApp: merged by tools/hg/promote.py from 12 m2c local copies (1 variants, base
// src/overlay_trainer_card_signature_021E8248.c) and 60 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct GameStats;
struct MsgData;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct GF_2DGfxResMan;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;
struct MessageFormat;
struct YesNoPrompt;

typedef struct SignatureTouchDataBuffer {
    u8 x[8];
    u8 y[8];
    u8 brushTypeUnused : 4;
    u8 bufferSize : 4;
} SignatureTouchDataBuffer;

typedef struct PreviousTouchDataBuffer {
    u8 lastX;
    u8 lastY;
    u16 bufferSize;
} PreviousTouchDataBuffer;

typedef struct DrawingState {
    s8 previousXDirection;
    s8 previousYDirection;
    s8 currentXDirection;
    s8 currentYDirection;
    u8 padding;
} DrawingState;

typedef struct SignatureApp {
    struct BgConfig *bgConfig;
    u8 padding[4];
    struct GameStats *records;
    struct Options *options;
    struct MessageFormat *template;
    struct MsgData *loader;
    struct String *dummyStrings[5];
    struct String *buttonText;
    struct String *descriptionText;
    struct String *messageText;
    int printerID;
    struct SpriteList *spriteList;
    G2dRenderer g2DRenderer;
    struct GF_2DGfxResMan *spriteResourceCollection[4];
    struct SpriteResource *spriteResources[4][2];
    SpriteResourcesHeader dummyResourcesHeader;
    SpriteResourcesHeader spriteResourcesHeader;
    u8 padding2[112];
    struct Sprite *sprite[1];
    u8 padding3[32];
    Window dummyWindows[5];
    Window signatureWindow;
    Window messageWindow;
    Window buttonWindow;
    Window descriptionWindow;
    u8 padding4[8];
    int subState;
    u8 padding5[16392];
    u16 glowEffectDegreesUnused;
    u8 brushTypeUnused;
    u8 padding6;
    SignatureTouchDataBuffer touchDataBuffer;
    SignatureTouchDataBuffer touchDataBuffers[5];
    PreviousTouchDataBuffer prevTouchData[5];
    u8 padding7[6144];
    u8 pad_5B96[0x2];
    int *savedSignature;
    u8 buttonPixelBuffer[256];
    struct YesNoPrompt *yesNoMenu;
    DrawingState drawingState;
    u8 pad_5CA5[0x3];
    int lastTouchedX;
    int lastTouchedY;
} SignatureApp;

// sizeof(SignatureApp) == 0x5cb0

// Sub-state handlers (ov52_021E96C0[signatureApp->subState]).
int ov52_021E8CF4(SignatureApp *signatureApp, int state);
int ov52_021E8D8C(SignatureApp *signatureApp, int state);
int ov52_021E8DC4(SignatureApp *signatureApp, int state);
int ov52_021E8E64(SignatureApp *signatureApp, int state);
int ov52_021E8EDC(SignatureApp *signatureApp, int state);
int ov52_021E8EFC(SignatureApp *signatureApp, int state);

#endif // POKEHEARTGOLD_SIGNATURE_APP_H
