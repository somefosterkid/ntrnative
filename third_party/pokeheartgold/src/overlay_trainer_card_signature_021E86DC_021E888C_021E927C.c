#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"

struct BgConfig;

struct GameStats;

struct MsgData;

struct Options;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct YesNoPrompt;

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct Options Options;

u8 Options_GetTextFrameDelay(const Options *options);

void Heap_Free(void *ptr);

void String_Delete(String *string);

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

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

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct GameStats GameStats;

typedef struct MsgData MsgData;

void ReadMsgDataIntoString(const MsgData *loader, u32 entryID, String *string);

typedef struct MessageFormat MessageFormat;

void StringExpandPlaceholders(const MessageFormat *template, String *dst, const String *fmtString);

typedef struct YesNoPrompt YesNoPrompt;

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
    BgConfig *bgConfig;
    u8 padding[4];
    GameStats *records;
    Options *options;
    MessageFormat *template;
    MsgData *loader;
    String *dummyStrings[5];
    String *buttonText;
    String *descriptionText;
    String *messageText;
    int printerID;
    SpriteList *spriteList;
    G2dRenderer g2DRenderer;
    SpriteResourceCollection *spriteResourceCollection[(GF_GFX_RES_TYPE_ANIM + 1)];
    SpriteResource *spriteResources[2][(GF_GFX_RES_TYPE_ANIM + 1)];
    SpriteResourcesHeader dummyResourcesHeader;
    SpriteResourcesHeader spriteResourcesHeader;
    u8 padding2[112];
    Sprite *sprite[1];
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
    u8 pad_5B96[2];
    u8 *savedSignature;
    u8 buttonPixelBuffer[256];
    YesNoPrompt *yesNoMenu;
    DrawingState drawingState;
    u8 pad_5CA5[3];
    int lastTouchedX;
    int lastTouchedY;
} SignatureApp;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

u8 AddTextPrinterParameterized(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, PrinterCallback_t callback);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);

void ov52_021E927C(SignatureApp *signatureApp, int bankEntry);

void ov52_021E927C(SignatureApp *signatureApp, int bankEntry) {
    String *fmtString = String_New(40 * 2, HEAP_ID_39);

    ReadMsgDataIntoString(signatureApp->loader, bankEntry, fmtString);
    StringExpandPlaceholders(signatureApp->template, signatureApp->messageText, fmtString);
    String_Delete(fmtString);
    FillWindowPixelBuffer(&signatureApp->messageWindow, 15);
    DrawFrameAndWindow2(&signatureApp->messageWindow, FALSE, 1, 10);

    signatureApp->printerID = AddTextPrinterParameterized(&signatureApp->messageWindow, FONT_MESSAGE, signatureApp->messageText, 0, 0, Options_GetTextFrameDelay(signatureApp->options), NULL);
}
