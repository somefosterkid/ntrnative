#ifndef POKEHEARTGOLD_OVERLAY_87_SCRATCH_OFF_CARD_APP_H
#define POKEHEARTGOLD_OVERLAY_87_SCRATCH_OFF_CARD_APP_H

#include "bg_window.h"
#include "touch_pad_data_buffer.h"
#include "g2d_renderer.h"

// ScratchOffCardApp: merged by tools/hg/promote.py from 28 m2c local copies (5 variants, base
// src/overlay_87_021E7490_021E7590_021E7590.c) and 120 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct FontSpecialCharsContext;
struct GF_2DGfxResMan;
struct MessageFormat;
struct MessagePrinter;
struct MsgData;
struct NARC;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct OverlayManager;
struct PaletteData;
struct SaveData;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct UnkStruct_020163E0;
struct YesNoPrompt;

struct Sprite;

typedef struct CoordinatesS16 {
    s16 x;
    union {
        s16 y;
        s16 z;
    };
} CoordinatesS16;

typedef struct ScratchOffCardsAppSprite {
    u16 unused;
    u8 onSubScreen;
    u8 unused2[9];
    struct Sprite *sprite;
} ScratchOffCardsAppSprite;

typedef struct ScratchOffCardsAppSpriteManager {
    struct SpriteList *spriteList;
    G2dRenderer g2dRenderer;
    struct GF_2DGfxResMan *resourceCollections[4];
    struct SpriteResource *resources[5][4];
} ScratchOffCardsAppSpriteManager;

enum ScratchOffCardsWindow {
    SCRATCH_WINDOW_STOP_SCRATCH = 0,
    SCRATCH_WINDOW_UNUSED_1,
    SCRATCH_WINDOW_ITEM_NAME_1,
    SCRATCH_WINDOW_ITEM_NAME_2,
    SCRATCH_WINDOW_ITEM_NAME_3,
    SCRATCH_WINDOW_ITEM_NAME_4,
    SCRATCH_WINDOW_MON_1,
    SCRATCH_WINDOW_MON_2,
    SCRATCH_WINDOW_MON_3,
    SCRATCH_WINDOW_MON_4,
    SCRATCH_WINDOW_START_CARD,
    SCRATCH_WINDOW_UNUSED_11,
    SCRATCH_WINDOW_TOO_BAD,
    SCRATCH_WINDOW_NEXT_CARD,
    SCRATCH_WINDOW_CANCEL,
    SCRATCH_WINDOW_SELECT_CARD,
    NUM_SCRATCH_WINDOWS,
};

typedef struct ScratchOffCardApp {
    struct OverlayManager *appMan;
    struct OverlayManager *unused;
    u8 subState;
    u8 dummy;
    u8 unused2[2];
    u8 dummy2;
    u8 fadeOutDelay;
    u8 currentCard;
    u8 subStateTimer;
    s16 xOffset;
    s16 xOffset2;
    u8 mosaicSize;
    u8 selectedCardIdxs[4];
    u8 dummy3[3];
    VecFx32 cardScaleFactor;
    VecFx32 unused3;
    struct UnkStruct_020163E0 *paletteAnimator;
    struct MsgData *msgLoader;
    struct MessageFormat *strTemplate;
    struct String *displayStr;
    struct String *fmtStr;
    u16 unused4[8];
    struct BgConfig *bgConfig;
    Window windows[NUM_SCRATCH_WINDOWS];
    struct PaletteData *plttData;
    struct MessagePrinter *specialChars;
    struct Options *options;
    struct SaveData *saveData;
    ScratchOffCardsAppSpriteManager spriteMan;
    ScratchOffCardsAppSprite *topScreenMonSprites[4];
    ScratchOffCardsAppSprite *scratchMonSprites[9];
    ScratchOffCardsAppSprite *cardSprites[4];
    ScratchOffCardsAppSprite *skipCardButtonSprite;
    ScratchOffCardsAppSprite *centerMessageBoxSprite;
    ScratchOffCardsAppSprite *matchingCellBoxSprites[3];
    ScratchOffCardsAppSprite *winMonSprites[3];
    ScratchOffCardsAppSprite *winPopupSprite;
    u8 scratchCellValues[9];
    u8 pad_369[0x1];
    u16 winnableItems[4];
    u8 pad_372[0x2];
    u16 *dummy4;
    u16 *wonItems;
    u16 *wonItemsCount;
    struct NARC *narc;
    void *scratchCellsRawCharData;
    struct NNSG2dCharacterData *scratchCellsCharData;
    u8 *scratchCellsDataBackup;
    struct YesNoPrompt *yesNoMenu;
    u8 cellIsScratched[9];
    u8 highlightPairPalette;
    u8 highlightPairCounter;
    u8 winningCellValue;
    u8 pairFound : 1;
    u8 centeringStep : 7;
    u8 numRevealedCells;
    u8 scratchedOffCellIdxs[3];
    u8 cellIsRevealed[9];
    u32 unused6[2];
    TouchPadDataBuffer tpBuffer;
    u8 scratchedPixels[240 * 160];
} ScratchOffCardApp;

// sizeof(ScratchOffCardApp) == 0x99fc (the DS allocation in ScratchOffCards_Init)

#endif // POKEHEARTGOLD_OVERLAY_87_SCRATCH_OFF_CARD_APP_H
