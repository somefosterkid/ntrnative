#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"

struct SpriteList;

struct SpriteResourceCollection;

typedef struct {
    union {
        u32 unk_00_val1;
        struct {
            u8 unk_00_val2_unk_00;
            u8 unk_00_val2_unk_01;
            u8 unk_00_val2_unk_02;
            u8 unk_00_val2_unk_03;
        };
    };
} UnkStruct_ov66_02231428;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct WiFiPlazaNewsBlinkStage {
    u32 tileIndex;
} WiFiPlazaNewsBlinkStage;

typedef struct WiFiPlazaNewsDigits {
    void *buffer;
    NNSG2dScreenData *screenData;
} WiFiPlazaNewsDigits;

typedef struct WiFiPlazaNewsCountdown {
    UnkStruct_ov66_02231428 remainingTime;
    u8 waitingForTimeSync;
    u8 needsRedraw;
    u16 blinkCounter;
    u32 secondsPerSegment;
} WiFiPlazaNewsCountdown;

typedef struct WiFiPlazaNewsGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *spriteResources[4];
    NARC *narc;
} WiFiPlazaNewsGraphics;

void ov88_02258D90(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u32 segmentState, u8 column, u8 row);

extern const WiFiPlazaNewsBlinkStage ov88_02259924[4];

void ov88_02258F88(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics);

void ov88_0225918C(void *, void *, void *, void *, u32);
void ov88_022590C0(void *param_1, void *param_2, void *param_3, void *param_4, void *param_5);

void ov88_022590D4(void);
s32 ov45_0222DD38(u8 *);
s32 ov88_022590D8(void *arg0, void *arg1);

void ov88_02258F88(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics) {
    int i, j;
    u32 segmentsRemaining;
    u16 segmentState;
    s32 column, row;
    s32 segmentIndex;

    segmentsRemaining = countdown->remainingTime.unk_00_val2_unk_02;
    segmentsRemaining += countdown->remainingTime.unk_00_val2_unk_01 * 60;
    segmentsRemaining += countdown->remainingTime.unk_00_val2_unk_00 * (60 * 60);
    segmentsRemaining /= countdown->secondsPerSegment;

    if (countdown->needsRedraw) {
        countdown->blinkCounter = 0;
        countdown->needsRedraw = 0;

        FillBgTilemapRect(graphics->bgConfig, GF_BG_LYR_MAIN_3, 0, 5, 4, 10 * 1, 4 * 2, 0);
        ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_MAIN_3);

        for (i = 0; i < 4; i++) {
            row = (4 - 1) - i;

            for (j = 0; j < 10; j++) {
                column = (10 - 1) - j;
                segmentIndex = ((row * 10) + column);

                if (segmentIndex >= segmentsRemaining) {
                    if (segmentIndex == segmentsRemaining) {
                        segmentState = 1;
                    } else if (row == (4 - 1)) {
                        segmentState = 2;
                    } else {
                        segmentState = 0;
                    }

                    ov88_02258D90(digits, graphics, segmentState, column, row);
                }
            }
        }
    } else {
        if (segmentsRemaining < (4 * 10)) {
            u32 blinkPhase;
            u32 blinkFrame;
            u32 blinkThreshold;

            countdown->blinkCounter++;

            blinkPhase = countdown->blinkCounter % 32;
            column = segmentsRemaining % 10;
            row = segmentsRemaining / 10;

            if (blinkPhase == 0) {
                blinkThreshold = (countdown->secondsPerSegment * 30) / 4;
                blinkFrame = 6;

                for (i = 0; i < 4; i++) {
                    if (blinkThreshold * (i + 1) > countdown->blinkCounter) {
                        blinkFrame = ov88_02259924[i].tileIndex;
                        break;
                    }
                }

                ov88_02258D90(digits, graphics, blinkFrame, column, row);
            } else if (blinkPhase == (32 / 2)) {
                ov88_02258D90(digits, graphics, 3, column, row);
            }
        }
    }
}

void ov88_022590C0(void *param_1, void *param_2, void *param_3, void *param_4, void *param_5) {
    ov88_0225918C(param_3, param_2, param_4, param_5, 0);
    return;
}

void ov88_022590D4(void) {
}

s32 ov88_022590D8(void *arg0, void *arg1) {
    s32 v0;
    s32 v1;

    v1 = 0;
    v0 = ov45_0222DD38(arg1);
    switch (v0) {
    case 0:
        break;
    case 1:
        v1 = 1;
        *(s16 *)((u8 *)arg0 + 2) = 1;
        break;
    case 2:
        *(s16 *)((u8 *)arg0 + 2) = 3;
        v1 = 1;
        break;
    }
    return v1;
}
