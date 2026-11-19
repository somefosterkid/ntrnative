#include "global.h"
#include "save.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "field/field_3d_model.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c
typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    struct PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;

typedef enum HeapID HeapID;

struct Background;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct PaletteFadeControl;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct PlayerProfile;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct Window;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    BOOL unk_38;
    u32 unk_3C;
} UnkStruct_ov66_02231134;

typedef struct UnkStruct_ov114_02025CAD4_t UnkStruct_ov114_0225CAD4;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;

typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;

typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

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

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;

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

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;

void PaletteData_LoadPaletteSlotFromHardware(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 start, u32 size);

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

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct ManagedSprite {
    Sprite *sprite;
    SpriteResourcesHeader *resourceHeader;
    SpriteResourcesHeaderList *resourceHeaderList;
    BOOL vramTransfer;
} ManagedSprite;

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    int capacity;
    int count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct GF_2DGfxResHeader {
    void *entries;
    int count;
    GfGfxResType type;
    u8 loadFromNARC;
} GF_2DGfxResHeader;

typedef struct GF_2DGfxResObjList {
    SpriteResource **resources;
    int capacity;
    int count;
} GF_2DGfxResObjList;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

void G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType, GXOBJVRamModeChar vramMode);

void G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE vramType);

typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;

typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;

SpriteList *SpriteManager_GetSpriteList(SpriteManager *spriteMan);

typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    G3DPipelineBuffers *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;

typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;

typedef struct {
    u32 unk_00;
} UnkStruct_ov116_02260490;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u8 padding_08[4];
} UnkStruct_ov116_02262A8C_sub1;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20[4];
    int unk_28[4];
    int unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    UnkStruct_ov116_0226469C unk_74;
    UnkStruct_ov116_0226469C unk_8C;
    BOOL unk_A4;
    BOOL unk_A8;
    BOOL unk_AC;
    BOOL unk_B0;
    BOOL unk_B4;
    s16 unk_B8;
    s16 unk_BA;
} UnkStruct_ov116_02262A8C_sub2;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Field3dObject *obj, BOOL visible);

typedef struct {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;

typedef struct {
    int unk_00;
    int unk_04;
    u8 padding_08[4];
    ManagedSprite *unk_0C;
    UnkStruct_ov116_0226469C unk_10;
    int unk_28;
    int *unk_2C;
    BOOL unk_30;
    UnkStruct_ov116_0226501C *unk_34;
    UnkStruct_ov116_0226501C *unk_38;
    UnkStruct_ov116_022649E4 *unk_3C;
    int unk_40;
} UnkStruct_ov116_02262D08;

typedef struct {
    BOOL unk_00;
    int unk_04;
    ManagedSprite *unk_08[3];
    UnkStruct_ov116_0226469C unk_14[3][2];
    UnkStruct_ov116_022649E4 *unk_A4;
} UnkStruct_ov116_022647BC;

typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

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

typedef struct {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;

typedef struct {
    ManagedSprite *unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[6];
    int unk_10;
    s16 unk_14;
    s16 unk_16;
    int unk_18;
    int unk_1C;
    UnkStruct_ov116_0226469C unk_20;
    UnkStruct_ov116_0226469C unk_38;
} UnkStruct_ov116_02265D84;

typedef struct {
    UnkStruct_ov116_02265D84 unk_00[2];
    UnkStruct_ov116_02265D84 unk_A0[2];
    int unk_140;
    int unk_144;
    int unk_148;
    int unk_14C;
} UnkStruct_ov116_02265AA4;

typedef struct {
    UnkStruct_ov116_02265AA4 unk_00;
    u8 padding_04[4];
    SysTask *unk_08;
    int *unk_0C;
    int *unk_10;
    int *unk_14;
    int unk_18;
    int unk_1C;
    u8 padding_20[4];
    int unk_24;
    BOOL unk_28;
    BOOL unk_2C;
    UnkStruct_ov116_022649E4 *unk_30;
    UnkStruct_ov116_0226469C unk_34[5];
} UnkStruct_ov116_022660A8;

typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    u8 padding_10[4];
    UnkStruct_ov116_022649E4 *unk_14;
    u8 padding_18[4];
    ManagedSprite *unk_1C[3];
    ManagedSprite *unk_20[3];
    ManagedSprite *unk_24[3];
    ManagedSprite *unk_28[36];
    ManagedSprite *unk_B8[4];
    ManagedSprite *unk_C8[2];
    ManagedSprite *unk_D0[2];
    ManagedSprite *unk_D8;
    u16 unk_DC[4];
    u8 padding_E4[12];
    int unk_F0;
    int unk_F4;
    int unk_F8;
    UnkStruct_ov116_0226501C unk_FC;
    UnkStruct_ov116_0226501C unk_308[3];
    UnkStruct_ov116_0226501C unk_92C;
    UnkStruct_ov116_0226501C unk_B38[8];
    UnkStruct_ov116_0226501C unk_1B98;
    UnkStruct_ov116_0226501C unk_1DA4;
    VecFx32 unk_1FB0;
    UnkStruct_ov116_02262A8C_sub1 unk_1FBC;
    Window unk_1FC8;
    Window unk_1FD8[4];
    UnkStruct_ov116_022660A8 unk_2018;
    UnkStruct_ov116_0226534C unk_20C4;
    UnkStruct_ov116_022647BC unk_214C[8];
    UnkStruct_ov116_02262D08 unk_268C[4];
    UnkStruct_ov116_02262A8C_sub2 unk_279C;
    UnkStruct_ov116_02260490 unk_2858[4];
    u8 padding_2868[4];
    u32 unk_286C;
    int unk_2870;
    BOOL unk_2874;
} UnkStruct_ov116_02262A8C;

typedef struct {
    UnkStruct_ov116_02262A44 *unk_00;
    UnkStruct_ov116_02262A8C *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    UnkStruct_ov116_0226048C unk_14;
    UnkStruct_ov116_02260494 unk_1C;
    UnkStruct_ov116_02260498 unk_24;
    u32 unk_44;
    UnkStruct_ov116_022649E4 unk_48;
    u32 unk_78;
    UnkStruct_ov114_0225CAD4 *unk_7C;
    UnkStruct_ov66_02231134 *unk_80;
    UnkStruct_ov114_0225C76C unk_84;
    UnkStruct_ov114_0225D678 *unk_94;
    UnkStruct_ov114_0225C9A8 unk_98;
    UnkStruct_ov114_0225E854 *unk_B0;
    int unk_B4[4];
    BOOL unk_C4;
} UnkStruct_ov116_0226139C;

BOOL ov92_0225D8C4(UnkStruct_ov116_0226139C *param0);

void ov00_021E69A8(enum HeapID heapID);

UnkStruct_ov114_0225CAD4 *ov90_02258BD4(SpriteList *param0, u32 heapID);

void ov92_0225C5A8(UnkStruct_ov116_0226139C *param0);

void ov92_0225D9A8(UnkStruct_ov116_0226501C *param0, NARC *param1, int param2);

void ov92_0225D9B4(UnkStruct_ov116_0226501C *param0, int param1);

void ov92_0225DE08(UnkStruct_ov116_02262A8C *param0);

void ov92_0225DE70(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E130(UnkStruct_ov116_0226139C *param0, int param1);

void ov92_0225E3C4(UnkStruct_ov116_0226139C *param0);

void ov92_0225E6A4(UnkStruct_ov116_0226139C *param0, UnkStruct_ov116_02262A8C *param1);

void ov92_0225E820(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E93C(UnkStruct_ov116_02262A8C *param0);

void ov92_0225E9B4(UnkStruct_ov116_0226139C *param0);

UnkStruct_ov116_02262A44 *ov92_0225EB40(int param0, UnkStruct_ov116_022649E4 *param1);

UnkStruct_ov116_02262A8C *ov92_0225EB88(int param0, u32 param1, UnkStruct_ov116_022649E4 *param2);

int ov92_0226077C(UnkStruct_ov116_0226139C *param0, int param1);

int sub_020347A0(void);

u16 sub_0203769C(void);

void Heap_Free(void *ptr);

u32 GetLCRNGSeed(void);

void sub_0203A880(void);

void *sub_0203A4AC(enum HeapID heapID);

typedef void (*Callback)(void *);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void EnableTouchPad(void);

BOOL InitializeTouchPad(u32 autoSamplingFrequency);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

void ov92_0225D3CC(UnkStruct_ov116_0226139C *param0);

void ov92_0225D894(void *param0);

void ov92_0225C5C4(UnkStruct_ov116_0226139C *param0);

typedef struct {
    int a[16][2];
} ov92_02263A14_Array;

extern const ov92_02263A14_Array ov92_02263A14;

typedef struct {
    int a[4];
} ov92_022638A0_Array;

extern const ov92_022638A0_Array ov92_022638A0;

void ov92_0225C5C4(UnkStruct_ov116_0226139C *param0) {
    param0->unk_78 = GetLCRNGSeed();

    ov92_0225D3CC(param0);
    Main_SetVBlankIntrCB(ov92_0225D894, param0);
    HBlankInterruptDisable();
    GF_CreateVramTransferManager(32, HEAP_ID_113);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    sub_0203A880();

    NNSG2dPaletteData *v0;
    void *v1 = sub_0203A4AC(HEAP_ID_113);

    NNS_G2dGetUnpackedPaletteData(v1, &v0);
    PaletteData_LoadPalette(param0->unk_48.unk_14, v0->pRawData, PLTTBUF_MAIN_OBJ, ((14) * (16)), ((16) * sizeof(u16)));
    Heap_Free(v1);

    int v2 = sub_020347A0();

    param0->unk_44 = sub_0203769C();
    ov92_0225C5A8(param0);

    if (ov92_0225D8C4(param0) == 1) {
        param0->unk_00 = ov92_0225EB40(v2, &param0->unk_48);
    }

    param0->unk_04 = ov92_0225EB88(v2, param0->unk_44, &param0->unk_48);
    param0->unk_04->unk_2870 = param0->unk_14.unk_00;
    param0->unk_04->unk_286C = param0->unk_78;

    u32 v3;

    EnableTouchPad();
    v3 = InitializeTouchPad(4);

    ov92_0225E9B4(param0);
    ov92_0225E3C4(param0);
    ov92_0225E6A4(param0, param0->unk_04);
    ov92_0225DE70(param0->unk_04);
    ov92_0225DE08(param0->unk_04);
    ov92_0225E820(param0->unk_04);
    ov92_0225E93C(param0->unk_04);
    ov92_0225E130(param0, ov92_0226077C(param0, param0->unk_44));

    {
        {
            int v4;
            ov92_02263A14_Array v5 = ov92_02263A14;

            for (v4 = 0; v4 < 8; v4++) {
                ov92_0225D9A8(&param0->unk_04->unk_B38[v4], param0->unk_48.unk_00, v5.a[v4][0]);
                ov92_0225D9B4(&param0->unk_04->unk_B38[v4], param0->unk_14.unk_00);

                sub_020180BC(&param0->unk_04->unk_B38[v4].unk_88[0], &param0->unk_04->unk_B38[v4].unk_78, param0->unk_48.unk_00, v5.a[v4][1], (HeapID)(113), &param0->unk_48.unk_24);
                sub_020181D4(&param0->unk_04->unk_B38[v4].unk_00, &param0->unk_04->unk_B38[v4].unk_88[0]);
                sub_02018198(&param0->unk_04->unk_B38[v4].unk_88[0], 0);
                sub_020182A0(&param0->unk_04->unk_B38[v4].unk_00, 0);

                {
                    int v6 = 5;

                    if (param0->unk_14.unk_00) {
                        v6 = 5;
                    }

                    if (v4 % 2) {
                        param0->unk_04->unk_B38[v4].unk_1E0.x += ((fx32)(((v4 * 5) > 0) ? ((v4 * 5) * (1 << 12) + 0.5f) : ((v4 * 5) * (1 << 12) - 0.5f)));
                    } else {
                        param0->unk_04->unk_B38[v4].unk_1E0.x -= ((fx32)(((v4 * 5) > 0) ? ((v4 * 5) * (1 << 12) + 0.5f) : ((v4 * 5) * (1 << 12) - 0.5f)));
                    }

                    if (v4 < 8) {
                        param0->unk_04->unk_B38[v4].unk_1E0.z += ((fx32)((((v4 + 1) * v6) > 0) ? (((v4 + 1) * v6) * (1 << 12) + 0.5f) : (((v4 + 1) * v6) * (1 << 12) - 0.5f)));
                    } else {
                        param0->unk_04->unk_B38[v4].unk_1E0.z -= ((fx32)(((v4 * 5) > 0) ? ((v4 * 5) * (1 << 12) + 0.5f) : ((v4 * 5) * (1 << 12) - 0.5f)));
                    }
                }
            }
        }

        ov92_0225D9A8(&param0->unk_04->unk_92C, param0->unk_48.unk_00, 71);
        ov92_0225D9B4(&param0->unk_04->unk_92C, param0->unk_14.unk_00);
        sub_020182A0(&param0->unk_04->unk_92C.unk_00, 0);

        {
            ov92_022638A0_Array v7 = ov92_022638A0;

            int v8 = v7.a[ov92_0226077C(param0, param0->unk_44)];

            ov92_0225D9A8(&param0->unk_04->unk_FC, param0->unk_48.unk_00, v8);
            ov92_0225D9B4(&param0->unk_04->unk_FC, param0->unk_14.unk_00);
            ov92_0225D9A8(&param0->unk_04->unk_308[0], param0->unk_48.unk_00, 51);
            ov92_0225D9A8(&param0->unk_04->unk_308[1], param0->unk_48.unk_00, 57);
            ov92_0225D9A8(&param0->unk_04->unk_308[2], param0->unk_48.unk_00, 55);
            ov92_0225D9B4(&param0->unk_04->unk_308[0], param0->unk_14.unk_00);
            ov92_0225D9B4(&param0->unk_04->unk_308[1], param0->unk_14.unk_00);
            ov92_0225D9B4(&param0->unk_04->unk_308[2], param0->unk_14.unk_00);

            {
                UnkStruct_ov116_0226501C *v9;
                int v10;

                v9 = &param0->unk_04->unk_308[0];

                v10 = 53;
                sub_020180BC(&v9->unk_88[0], &v9->unk_78, param0->unk_48.unk_00, v10, (HeapID)(113), &param0->unk_48.unk_24);
                sub_020181D4(&v9->unk_00, &v9->unk_88[0]);
                sub_02018198(&v9->unk_88[0], 0);

                v10 = 52;
                sub_020180BC(&v9->unk_88[1], &v9->unk_78, param0->unk_48.unk_00, v10, (HeapID)(113), &param0->unk_48.unk_24);
                sub_02018198(&v9->unk_88[1], 0);

                v10 = 58;
                sub_020180BC(&v9->unk_88[2], &v9->unk_78, param0->unk_48.unk_00, v10, (HeapID)(113), &param0->unk_48.unk_24);
                sub_02018198(&v9->unk_88[2], 0);

                v9 = &param0->unk_04->unk_308[1];
                v10 = 56;
                sub_020180BC(&v9->unk_88[0], &v9->unk_78, param0->unk_48.unk_00, v10, (HeapID)(113), &param0->unk_48.unk_24);
                sub_020181D4(&v9->unk_00, &v9->unk_88[0]);
                sub_02018198(&v9->unk_88[0], 0);

                v9 = &param0->unk_04->unk_308[2];
                v10 = 54;
                sub_020180BC(&v9->unk_88[0], &v9->unk_78, param0->unk_48.unk_00, v10, (HeapID)(113), &param0->unk_48.unk_24);
                sub_020181D4(&v9->unk_00, &v9->unk_88[0]);
                sub_02018198(&v9->unk_88[0], 0);
            }
        }

        if (param0->unk_14.unk_00) {
            param0->unk_04->unk_FC.unk_1E0.y = (-8 * ((fx32)0x0000000000001000L));
            param0->unk_04->unk_308[0].unk_1E0.y = (-8 * ((fx32)0x0000000000001000L));
            param0->unk_04->unk_308[1].unk_1E0.y = (-8 * ((fx32)0x0000000000001000L));
            param0->unk_04->unk_308[2].unk_1E0.y = (-8 * ((fx32)0x0000000000001000L));
        }
    }

    param0->unk_7C = ov90_02258BD4(SpriteManager_GetSpriteList(param0->unk_48.unk_0C), HEAP_ID_113);
    PaletteData_LoadPaletteSlotFromHardware(param0->unk_48.unk_14, PLTTBUF_MAIN_OBJ, ((0) * (16)), 16 * ((16) * sizeof(u16)));

    if (param0->unk_80->unk_3C) {
        ov00_021E69A8(HEAP_ID_113);
    }
}
