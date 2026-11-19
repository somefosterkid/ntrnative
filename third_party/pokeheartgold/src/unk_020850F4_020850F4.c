#include "global.h"

#include "unk_020210A0.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"

struct OverlayManager;

struct BgConfig;

struct TextOBJ;

struct ManagedSprite;

struct NARC;

struct Options;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct TouchHitboxController;

struct UnkStruct_02012744_t;

typedef struct Options Options;

typedef struct String String;

typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

BgConfig *BgConfig_Alloc(enum HeapID heapID);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

PaletteData *PaletteData_Init(enum HeapID heapID);

void PaletteData_AllocBuffers(PaletteData *paletteData, enum PaletteBufferId bufferID, u32 size, enum HeapID heapID);

void PaletteData_SetAutoTransparent(PaletteData *paletteData, BOOL val);

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef struct TouchHitboxController TouchHitboxController;

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
    TouchScreenRect unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    TextOBJ *unk_90[2];
    CharTransferAllocation unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;

typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchScreenRect *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;

typedef struct {
    UnkStruct_0208AF44 unk_00[16];
    UnkStruct_0208AF44 unk_1C0[3];
    UnkStruct_0208AF44 unk_214[3];
    UnkStruct_0208AF44 unk_268[2];
    s16 unk_2A0[5];
    u16 unk_2AA[5][2];
    u8 pad_2BE[2];
    int unk_2C0;
    int unk_2C4;
    int unk_2C8;
    int unk_2CC;
    int unk_2D0;
    int unk_2D4;
    int unk_2D8;
    int unk_2DC;
    int unk_2E0;
    int unk_2E4;
    int unk_2E8;
    UnkStruct_02089688_sub1 unk_2EC;
    UnkStruct_02089688_sub2 unk_3AC;
    UnkStruct_02089438 unk_3B8;
    int unk_3EC;
    int unk_3F0;
} UnkStruct_02089688;

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_DisableEngineBPlanes(void);

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

void sub_0203A880(void);

void sub_0203A948(BOOL isUpperScreen, u32 heapID);

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_CreateAndGetData(OverlayManager *appMan, u32 size, enum HeapID heapID);

void *OverlayManager_GetArgs(OverlayManager *appMan);

typedef void (*Callback)(void *);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void sub_02085688(UnkStruct_02089688 *param0);

void sub_020860B8(UnkStruct_02089688 *param0);

void sub_020863F4(UnkStruct_02089688 *param0);

void sub_0208545C(BgConfig *param0);

void sub_020855CC(void *param0);

int sub_020850F4(OverlayManager *appMan, int *param1);

int sub_020850F4(OverlayManager *appMan, int *param1) {
    UnkStruct_02089688 *v0;

    Heap_Create(HEAP_ID_3, HEAP_ID_108, 0x40000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_02089688), HEAP_ID_108);
    memset(v0, 0, sizeof(UnkStruct_02089688));
    v0->unk_3B8 = *((UnkStruct_02089438 *)OverlayManager_GetArgs(appMan));

    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);

    v0->unk_2EC.unk_00 = NARC_New((enum NarcId)190, HEAP_ID_108);
    v0->unk_2EC.unk_0C = BgConfig_Alloc(HEAP_ID_108);
    v0->unk_2EC.unk_10 = PaletteData_Init(HEAP_ID_108);

    PaletteData_SetAutoTransparent(v0->unk_2EC.unk_10, 1);
    PaletteData_AllocBuffers(v0->unk_2EC.unk_10, PLTTBUF_MAIN_BG, ((16) * sizeof(u16)) * 16, HEAP_ID_108);
    PaletteData_AllocBuffers(v0->unk_2EC.unk_10, PLTTBUF_SUB_BG, ((16) * sizeof(u16)) * 16, HEAP_ID_108);
    PaletteData_AllocBuffers(v0->unk_2EC.unk_10, PLTTBUF_MAIN_OBJ, ((16) * sizeof(u16)) * 16, HEAP_ID_108);
    PaletteData_AllocBuffers(v0->unk_2EC.unk_10, PLTTBUF_SUB_OBJ, ((16) * sizeof(u16)) * 16, HEAP_ID_108);
    sub_0208545C(v0->unk_2EC.unk_0C);
    sub_020863F4(v0);
    sub_02085688(v0);

    {
        EnableTouchPad();
        InitializeTouchPad(4);
        sub_020860B8(v0);
    }

    if (v0->unk_3B8.unk_30 != 0) {
        sub_0203A880();
        sub_0203A948(1, HEAP_ID_108);
    }

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 15, 7);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 7, 8);

    Main_SetVBlankIntrCB(sub_020855CC, v0);

    return 1;
}
