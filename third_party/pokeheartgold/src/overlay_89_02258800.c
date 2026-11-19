#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "msgdata.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "font.h"
#include "field/field_3d_model.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CharTransferAllocation;

struct CharTransferTemplateWithModes;

struct TextOBJ;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct PaletteData;

struct RenderOamTemplate;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_02012744_t;

struct UnkStruct_ov113_0225DBCC_t;

struct UnkStruct_ov113_0225E6B8_t;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

struct YesNoPrompt;

typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;

typedef struct {
    BOOL unk_00;
    s32 unk_04;
} UnkStruct_ov113_022607EC;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

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

typedef struct TextOBJ TextOBJ;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

BgConfig *BgConfig_Alloc(enum HeapID heapID);

typedef struct ManagedSprite ManagedSprite;

typedef struct UnkStruct_ov66_0222DFF8_t WiFiPlazaSystem;

SaveData *ov66_0222E0C4(WiFiPlazaSystem *param0);

BOOL ov66_0222E0C8(const WiFiPlazaSystem *param0);

void ov45_0222A520(WiFiPlazaSystem *param0, BOOL param1);

typedef struct {
    WiFiPlazaSystem *unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov66_0223127C;

typedef struct UnkStruct_ov113_0225E6B8_t UnkStruct_ov113_0225E6B8;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_0225FE30;

typedef struct {
    VecFx32 unk_00;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FEE0;

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct Camera Camera;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FF8C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260064;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0226013C;

typedef struct {
    CameraAngle cameraAngle;
    fx32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E[2];
} UnkStruct_ov113_02260218;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_022602E4;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_02260394;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_0226046C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260544;

typedef struct {
    UnkStruct_ov113_0225FE30 unk_00;
    UnkStruct_ov113_0225FEE0 unk_08;
    UnkStruct_ov113_0225FF8C unk_18;
    UnkStruct_ov113_02260064 unk_28;
    UnkStruct_ov113_0226013C unk_38;
    UnkStruct_ov113_02260218 unk_48;
    UnkStruct_ov113_022602E4 unk_58;
    UnkStruct_ov113_02260394 unk_60;
    UnkStruct_ov113_0226046C unk_80;
    UnkStruct_ov113_02260544 unk_A0;
} UnkStruct_ov113_0225EB20_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov113_0225EDCC;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkStruct_ov113_0225FD80;

typedef struct {
    UnkStruct_ov113_0225EB20_sub1 unk_00;
    UnkStruct_ov113_0225FD80 unk_B0;
    UnkStruct_ov113_0225E6B8 *unk_B4[128];
    UnkStruct_ov113_0225E6B8 *unk_2B4[128];
    NNSG3dResTex *unk_4B4[32];
    u32 unk_534[2];
    UnkStruct_ov113_0225EDCC unk_53C[128];
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740;
    s16 unk_742;
} UnkStruct_ov113_0225EB20;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void ov89_0225A46C(UnkStruct_ov113_0225EB20 *param0, BOOL param1);

PaletteData *PaletteData_Init(enum HeapID heapID);

void PaletteData_AllocBuffers(PaletteData *paletteData, enum PaletteBufferId bufferID, u32 size, enum HeapID heapID);

void PaletteData_SetAutoTransparent(PaletteData *paletteData, BOOL val);

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

typedef struct CharTransferTemplateWithModes {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    GXOBJVRamModeChar modeMain;
    GXOBJVRamModeChar modeSub;
} CharTransferTemplateWithModes;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct RenderOamTemplate {
    int mainOamStart;
    int mainOamCount;
    int mainAffineOamStart;
    int mainAffineOamCount;
    int subOamStart;
    int subOamCount;
    int subAffineOamStart;
    int subAffineOamCount;
} RenderOamTemplate;

typedef struct G2dRenderer G2dRenderer;

void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *g2dRenderer, fx32 x, fx32 y);

void G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType, GXOBJVRamModeChar vramMode);

void G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType);

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

typedef union SpriteResourceCapacities {
    int asArray[GF_GFX_RES_TYPE_MAX];
    struct {
        int charCapacity;
        int plttCapacity;
        int cellCapacity;
        int animCapacity;
        int mcellCapacity;
        int manimCapacity;
    } asStruct;
} SpriteResourceCapacities;

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

G2dRenderer *SpriteSystem_GetRenderer(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const RenderOamTemplate *oamTemplate, const CharTransferTemplateWithModes *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCapacities *capacities);

void TextFlags_SetCanABSpeedUpPrint(BOOL val);

void TextFlags_SetAutoScrollParam(int flags);

void TextFlags_SetCanTouchSpeedUpPrint(BOOL val);

void Font_InitManager(enum Font font, enum HeapID heapID);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

enum VramManagerType {
    VRAM_MANAGER_TYPE_LINKED_LIST = 0,
    VRAM_MANAGER_TYPE_FRAME,
};

enum TextureVramSize {
    TEXTURE_VRAM_SIZE_128K = 1,
    TEXTURE_VRAM_SIZE_256K,
    TEXTURE_VRAM_SIZE_384K,
    TEXTURE_VRAM_SIZE_512K,
};

enum PaletteVramSize {
    PALETTE_VRAM_SIZE_16K = 1,
    PALETTE_VRAM_SIZE_32K,
    PALETTE_VRAM_SIZE_48K,
    PALETTE_VRAM_SIZE_64K,
    PALETTE_VRAM_SIZE_80K,
    PALETTE_VRAM_SIZE_96K,
};

typedef void (*G3DPipelineSetupCallback)(void);

G3DPipelineBuffers *GF_3DVramMan_Create(enum HeapID heapID, enum VramManagerType texVramManagerType, enum TextureVramSize texVramSize, enum VramManagerType plttVramManagerType, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb);

static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_DisableEngineBPlanes(void);

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void GfGfx_BothDispOn(void);

void GXLayers_SwapDisplay(void);

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

void sub_0203A880(void);

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

typedef void (*Callback)(void *);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void SetKeyRepeatTimers(int rate, int delay);

enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

typedef u32 TextColor;

void EnableTouchPad(void);

BOOL InitializeTouchPad(u32 autoSamplingFrequency);

UnkStruct_02012744 *FontSystem_NewInit(int param0, enum HeapID heapID);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

typedef struct YesNoPrompt YesNoPrompt;

YesNoPrompt *YesNoPrompt_Create(enum HeapID heapID);

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;

typedef struct {
    TextOBJ *unk_00;
    CharTransferAllocation unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

struct UnkStruct_ov113_0225DBCC_t {
    UnkStruct_ov66_0223127C *unk_00;
    SaveData *saveData;
    BgConfig *unk_08;
    PaletteData *unk_0C;
    UnkStruct_02012744 *unk_10;
    G3DPipelineBuffers *unk_14;
    SysTask *unk_18;
    SpriteSystem *unk_1C;
    SpriteManager *unk_20;
    YesNoPrompt *unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 pad_2A[2];
    MessageFormat *unk_2C;
    MsgData *unk_30;
    Window unk_34[8];
    Window unk_B4;
    String *unk_C4;
    u8 unk_C8;
    u8 pad_C9[3];
    Camera *camera;
    fx32 unk_D0;
    fx32 unk_D4;
    UnkStruct_ov113_0225DBCC_sub1 unk_D8;
    NARC *unk_160;
    NARC *unk_164;
    UnkStruct_ov113_022607EC unk_168;
    s32 unk_170[8];
    int unk_190;
    UnkStruct_ov113_0225EB20 unk_194;
    UnkStruct_ov113_02260818 unk_8D8[6];
    u8 unk_920;
    u8 unk_921;
    u8 pad_922[2];
    ManagedSprite *unk_924[6];
    ManagedSprite *unk_93C[6];
    ManagedSprite *unk_954[6];
    ManagedSprite *unk_96C;
    ManagedSprite *unk_970[8];
    ManagedSprite *unk_990[3];
    u16 unk_99C[8];
    u16 unk_9AC[8];
    int unk_9BC;
    u16 unk_9C0[2048];
    UnkStruct_ov113_0225E250 unk_19C0;
    int unk_19D4;
    int unk_19D8;
    int unk_19DC;
    BOOL unk_19E0;
};

void ov89_02258FF4(SysTask *param0, void *param1);

void ov89_0225901C(void *param0);

void ov89_0225905C(BgConfig *param0);

void ov89_02259264(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_02259CD0(UnkStruct_ov113_0225DBCC *param0);

G3DPipelineBuffers *sub_02077400(enum HeapID heapID);

void ov89_02259B00(UnkStruct_ov113_0225DBCC *param0);

void ov89_02259BAC(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_02259E18(UnkStruct_ov113_0225DBCC *param0);

void ov89_02259E50(int param0, SaveData *saveData, UnkStruct_ov113_02260818 *param2);

void ov89_02259408(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_02259734(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_02259588(UnkStruct_ov113_0225DBCC *param0);

void ov89_022597FC(UnkStruct_ov113_0225DBCC *param0);

void ov89_022598D0(struct UnkStruct_ov113_0225DBCC_t *param0);

extern const RenderOamTemplate ov89_0225CA58;

extern const CharTransferTemplateWithModes ov89_0225C9EC;

extern const SpriteResourceCapacities ov89_0225CA00;

void ov89_0225A260(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9);

int ov89_02258800(OverlayManager *appMan, int *param1);

SaveData *ov45_0222A2C8(WiFiPlazaSystem *param0);

BOOL ov45_0222A2CC(const WiFiPlazaSystem *param0);

G3DPipelineBuffers *ov89_02259D70(enum HeapID heapID);

void FontID_Alloc(enum Font fontId, enum HeapID heapID);

void GfGfx_SwapDisplay(void);

int ov89_02258800(OverlayManager *appMan, int *param1) {
    UnkStruct_ov113_0225DBCC *v0;

    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();

    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();

    Heap_Create(HEAP_ID_3, (enum HeapID)125, 0x50000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov113_0225DBCC), (enum HeapID)125);
    MI_CpuClear8(v0, sizeof(UnkStruct_ov113_0225DBCC));

    v0->unk_00 = OverlayManager_GetArgs(appMan);
    v0->saveData = ov45_0222A2C8(v0->unk_00->unk_00);
    v0->unk_19E0 = ov45_0222A2CC(v0->unk_00->unk_00);

    ov89_02259E18(v0);
    ov89_02259E50(v0->unk_00->unk_04, v0->saveData, v0->unk_8D8);

    v0->unk_19D4 = (32 << 8);
    v0->unk_19D8 = v0->unk_19D4;
    v0->unk_14 = ov89_02259D70((enum HeapID)125);
    v0->unk_0C = PaletteData_Init((enum HeapID)125);

    PaletteData_SetAutoTransparent(v0->unk_0C, 1);
    PaletteData_AllocBuffers(v0->unk_0C, PLTTBUF_MAIN_BG, ((16) * sizeof(u16)) * 16, (enum HeapID)125);
    PaletteData_AllocBuffers(v0->unk_0C, PLTTBUF_SUB_BG, ((16) * sizeof(u16)) * 16, (enum HeapID)125);
    PaletteData_AllocBuffers(v0->unk_0C, PLTTBUF_MAIN_OBJ, ((16) * sizeof(u16)) * 14, (enum HeapID)125);
    PaletteData_AllocBuffers(v0->unk_0C, PLTTBUF_SUB_OBJ, ((16) * sizeof(u16)) * 16, (enum HeapID)125);
    PaletteData_SetAutoTransparent(v0->unk_0C, 1);

    v0->unk_08 = BgConfig_Alloc((enum HeapID)125);

    GF_CreateVramTransferManager(64, (enum HeapID)125);
    SetKeyRepeatTimers(4, 8);

    ov89_0225905C(v0->unk_08);

    EnableTouchPad();
    InitializeTouchPad(4);
    FontID_Alloc(FONT_SUBSCREEN, (enum HeapID)125);

    v0->unk_2C = MessageFormat_New((enum HeapID)125);
    v0->unk_30 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (enum NarcId)27, 754, (enum HeapID)125);
    v0->unk_10 = FontSystem_NewInit(4, (enum HeapID)125);
    v0->unk_160 = NARC_New((enum NarcId)210, (enum HeapID)125);
    v0->unk_164 = NARC_New((enum NarcId)69, (enum HeapID)125);

    ov89_02259264(v0, v0->unk_160);
    ov89_02259BAC(v0, v0->unk_160);
    ov89_02259B00(v0);
    ov89_02259CD0(v0);

    v0->unk_C4 = String_New(256, (enum HeapID)125);
    v0->unk_1C = SpriteSystem_Alloc((enum HeapID)125);

    SpriteSystem_Init(v0->unk_1C, &ov89_0225CA58, &ov89_0225C9EC, 16 + 16);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);

    v0->unk_20 = SpriteManager_New(v0->unk_1C);

    SpriteSystem_InitSprites(v0->unk_1C, v0->unk_20, 64 + 64);
    SpriteSystem_InitManagerWithCapacities(v0->unk_1C, v0->unk_20, &ov89_0225CA00);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(v0->unk_1C), 0, (192 + 80) << 12);
    sub_0203A880();

    ov89_0225A46C(&v0->unk_194, v0->unk_19E0);
    ov89_02259408(v0, v0->unk_160);
    ov89_02259734(v0, v0->unk_160);
    ov89_02259588(v0);
    ov89_022597FC(v0);
    ov89_022598D0(v0);

    v0->unk_24 = YesNoPrompt_Create((enum HeapID)125);

    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, (enum HeapID)125);

    if (v0->unk_00->unk_00 != 0L) {
        ov45_0222A520(v0->unk_00->unk_00, 1);
    }

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);

    gSystem.screensFlipped = SCREEN_SUB;

    GfGfx_SwapDisplay();
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);

    v0->unk_18 = SysTask_CreateOnMainQueue(ov89_02258FF4, v0, 60000);

    Main_SetVBlankIntrCB(ov89_0225901C, v0);

    return 1;
}
