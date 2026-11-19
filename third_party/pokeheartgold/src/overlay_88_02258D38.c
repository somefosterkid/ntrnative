#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200B150.h"
#include "gf_gfx_planes.h"

struct G3DPipelineBuffers;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
struct WiFiPlazaNewsCountdown;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct SpriteList SpriteList;
typedef struct Sprite Sprite;
struct Sprite {
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
};
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
BOOL SpriteList_Delete(SpriteList *collection);
typedef struct SpriteResource SpriteResource;
typedef struct SpriteResourceCollection SpriteResourceCollection;
void Destroy2DGfxResObjMan(SpriteResourceCollection *spriteResources);
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct {
    BgConfig *unk_00;
    SpriteList *unk_04;
    G2dRenderer unk_08;
    SpriteResourceCollection *unk_194[4];
    G3DPipelineBuffers *unk_1A4;
    NARC *unk_1A8;
} UnkStruct_ov69_0225D35C;
typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;
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
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef struct WiFiPlazaNewsDigits {
    void *buffer;
    NNSG2dScreenData *screenData;
} WiFiPlazaNewsDigits;
typedef struct WiFiPlazaNewsCountdown WiFiPlazaNewsCountdown;
typedef struct WiFiPlazaNewsGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *spriteResources[4];
    NARC *narc;
} WiFiPlazaNewsGraphics;
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
u32 ov45_0222AD4C(const WiFiPlazaSystem *param0);
struct WiFiPlazaNewsCountdown {
    UnkStruct_ov66_02231428 remainingTime;
    u8 waitingForTimeSync;
    u8 needsRedraw;
    u16 blinkCounter;
    u32 secondsPerSegment;
};
BOOL ov88_02258EFC(WiFiPlazaNewsCountdown *countdown, const UnkStruct_ov66_0223177C *timeSource, const UnkStruct_ov66_02231428 *closeTime);
void ov88_02258F60(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics);

void ov88_02258D38(UnkStruct_ov69_0225D35C *param0);
void ov88_02258D64(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID);
void ov88_02258D84(ChooseStarterCursor *cursor);
void ov88_02258D90(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u32 segmentState, u8 column, u8 row);
void ov88_02258DE8(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u8 column, u8 row, u32 param4, u32 param5, BOOL param6, BOOL param7, BOOL param8);
void ov88_02258EA8(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsGraphics *graphics, WiFiPlazaNewsDigits *digits, const UnkStruct_ov66_0223177C *timeSource, const UnkStruct_ov66_02231428 *closeTime, const WiFiPlazaSystem *plazaSystem, enum HeapID heapID);

void ov88_02258D38(UnkStruct_ov69_0225D35C *param0) {
    int v0;

    SpriteList_Delete(param0->unk_04);

    for (v0 = 0; v0 < 4; v0++) {
        Destroy2DGfxResObjMan(param0->unk_194[v0]);
    }

    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void ov88_02258D64(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, enum HeapID heapID) {
    digits->buffer = GfGfxLoader_GetScrnDataFromOpenNarc(graphics->narc, 5, 0, &digits->screenData, heapID);
}

void ov88_02258D84(ChooseStarterCursor *cursor) {
    Heap_Free(cursor->sprite);
}

void ov88_02258D90(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u32 segmentState, u8 column, u8 row) {
    CopyToBgTilemapRect(graphics->bgConfig, GF_BG_LYR_MAIN_3, 5 + (column * 1), 4 + (row * 2), 1, 2, digits->screenData->rawData, 6 + segmentState, 0, digits->screenData->screenWidth / 8, digits->screenData->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_MAIN_3);
}

void ov88_02258DE8(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u8 column, u8 row, u32 param4, u32 param5, BOOL param6, BOOL param7, BOOL param8) {
    u32 iconCol = param4 % 3;
    u32 iconRow = param4 / 3;

    if (param7) {
        iconRow += 2;
    }

    if (param5 == 1) {
        iconRow++;
    }

    CopyToBgTilemapRect(graphics->bgConfig, GF_BG_LYR_MAIN_3, 21 + (column * 2), 4 + (row * 2), 2, 2, digits->screenData->rawData, 0 + (iconCol * 2), 0 + (iconRow * 2), digits->screenData->screenWidth / 8, digits->screenData->screenHeight / 8);

    if (param8) {
        BgTilemapRectChangePalette(graphics->bgConfig, GF_BG_LYR_MAIN_3, 21 + (column * 2), 4 + (row * 2), 2, 2, 4);
    } else {
        if (param6) {
            BgTilemapRectChangePalette(graphics->bgConfig, GF_BG_LYR_MAIN_3, 21 + (column * 2), 4 + (row * 2), 2, 2, 3);
        }
    }

    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, GF_BG_LYR_MAIN_3);
}

void ov88_02258EA8(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsGraphics *graphics, WiFiPlazaNewsDigits *digits, const UnkStruct_ov66_0223177C *timeSource, const UnkStruct_ov66_02231428 *closeTime, const WiFiPlazaSystem *plazaSystem, enum HeapID heapID) {
    u32 secondsUntilClose;
    u32 minutesUntilClose;

    secondsUntilClose = ov45_0222AD4C(plazaSystem);
    minutesUntilClose = secondsUntilClose / 60;

    countdown->remainingTime.unk_00_val2_unk_00 = 0;
    countdown->remainingTime.unk_00_val2_unk_01 = minutesUntilClose + 1;
    countdown->remainingTime.unk_00_val2_unk_02 = 0;
    countdown->secondsPerSegment = secondsUntilClose / (4 * 10);
    countdown->waitingForTimeSync = 1;
    countdown->blinkCounter = 0;
    countdown->needsRedraw = 0;

    ov88_02258EFC(countdown, timeSource, closeTime);
    ov88_02258F60(countdown, digits, graphics);
}
