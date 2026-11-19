#include "global.h"

struct Background;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct Easy3DAnim;

struct Easy3DModel;

struct Easy3DObject;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteBuffer;

struct PaletteData;

struct PaletteFadeControl;

struct Resource;

struct ResourceCollection;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourceList;

struct SpriteResourceTable;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskManager;

struct Window;

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

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;

    int xOffset;
    int yOffset;

    u8 type;
    u8 screenSize;
    u8 colorMode;
    u8 tileSize;

    u16 rotation;

    fx32 xScale;
    fx32 yScale;
    int xCenter;
    int yCenter;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    Background bgs[8];
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

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

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

typedef struct PaletteBuffer {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    PaletteFadeControl selected;
} PaletteBuffer;

typedef struct PaletteData {
    PaletteBuffer buffers[14];
    u16 selectedFlag : 2;
    u16 selectedBuffers : 14;
    u16 fadedBuffers : 14;
    u16 fadeInProgress : 1;
    u16 autoTransparent : 1;
    u8 forceExit;
} PaletteData;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

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



    u32 animData[29 ];
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

typedef struct SpriteResourcesHeader {
    const NNSG2dImageProxy *imageProxy;
    const NNSG2dCharacterData *charData;
    const NNSG2dImagePaletteProxy *paletteProxy;
    NNSG2dCellDataBank *cellBank;
    const NNSG2dCellAnimBankData *cellAnimBank;
    const NNSG2dMultiCellDataBank *multiCellBank;
    const NNSG2dMultiCellAnimBankData *multiCellAnimBank;
    BOOL isVRamTransfer;
    u8 priority;
    u8 padding_21[3];
} SpriteResourcesHeader;

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

typedef struct Resource {
    int id;
    void *data;
} Resource;

typedef struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
} ResourceCollection;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

typedef struct SpriteResource {
    Resource *rawResource;
    enum SpriteResourceType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    ResourceCollection *collection;
    SpriteResource *resources;
    int capacity;
    int count;
    enum SpriteResourceType type;
} SpriteResourceCollection;

typedef struct SpriteResourceTable {
    void *entries;
    int count;
    enum SpriteResourceType type;
    u8 loadFromNARC;
} SpriteResourceTable;

typedef struct SpriteResourceList {
    SpriteResource **resources;
    int capacity;
    int count;
} SpriteResourceList;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

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
    SpriteResourceTable *resourcePaths;
    SpriteResourceCollection *ownedResources[SPRITE_RESOURCE_MAX];
    SpriteResourceList *unownedResources[SPRITE_RESOURCE_MAX];
    int loadedResourceCount[SPRITE_RESOURCE_MAX];
    int numResourceTypes;
} SpriteManager;

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

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct Easy3DModel {
    void *data;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Easy3DModel;

typedef struct Easy3DAnim {
    void *data;
    void *anim;
    NNSG3dAnmObj *animObj;
    fx32 frame;
    BOOL dataBorrowed;
} Easy3DAnim;

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

typedef struct {
    Easy3DObject unk_00;
    Easy3DModel unk_78;
    Easy3DAnim unk_88[10];
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

BOOL ov116_0226432C(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

void ov116_0226469C(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

BOOL ov116_022646FC(UnkStruct_ov116_0226469C *param0);

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

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

u32 GetLCRNGSeed(void);

void SetLCRNGSeed(u32 seed);

u16 LCRandom(void);

inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID)
{
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

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0)
{
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail()) ;
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
        } while (1 );
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1)
{
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

typedef struct {
    int unk_00;
    int unk_04;
} UnkStruct_ov116_02267C88;

inline s8 inline_ov116_02263E20_1(void);

inline s8 inline_ov116_02263E20_2(s16 val);

extern const UnkStruct_ov116_02267C88 ov92_02263D84[4][4];

static inline u32 inline_ov116_02263E20()
{
    u32 v0 = GetLCRNGSeed();
    u32 v1 = LCRandom();
    SetLCRNGSeed(v0);

    return v1;
}

static inline s8 inline_ov116_02263E20_1(void)
{
    if (LCRandom() % 2) {
        return +1;
    } else {
        return -1;
    }
}

static inline s8 inline_ov116_02263E20_2(s16 param0)
{
    if (param0 >= 0) {
        return +1;
    } else {
        return -1;
    }
}


typedef union { struct { u8 code; u8 x; u8 y; u8 r; } circle; struct { u8 top; u8 bottom; u8 left; u8 right; } rect; } TouchscreenHitbox_hk35;
static const TouchscreenHitbox_hk35 ov92_02263C34[] = { { 0xFE, 0x80, 0x80, 0x30 }, { 0xFF, 0, 0, 0 } };
static const TouchscreenHitbox_hk35 ov92_02263C3C[] = { { 0xFE, 0x80, 0x80, 0x40 }, { 0xFF, 0, 0, 0 } };
void ov92_0225F878(UnkStruct_ov116_02262A8C *param0);  /* defined in its own nonmatching file */
void ov92_0225F8B0(UnkStruct_ov116_02262A8C *param0);  /* defined in its own nonmatching file */
void ov92_0225FF1C(UnkStruct_ov116_02262A8C *param0);


BOOL ov92_022607F8(UnkStruct_ov116_0226469C *param0);

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

void ov92_02260798(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

extern const VecFx32 ov92_02263C8C;
extern const VecFx32 ov92_02263C74;
extern const VecFx32 ov92_02263C80;
void ov92_0225FF1C(UnkStruct_ov116_02262A8C *param0)
{
    BOOL v0;
    BOOL v1;
    BOOL v2;
    BOOL v3;
    fx32 v4;
    fx32 v5;
    s16 v6 = 0;
    s16 v7 = 0;
    s16 v8[] = { (30 * 4), (30 * 3), (30 * 2), (30 * 1.5) };

    param0->unk_279C.unk_00++;

    if (param0->unk_279C.unk_40 != 0) {
        if (param0->unk_279C.unk_42 < (8 + 2) - 1) {
            if (param0->unk_279C.unk_42 <= 8) {
                v4 = param0->unk_279C.unk_74.unk_00;
                v5 = param0->unk_279C.unk_8C.unk_00;
                v2 = ov92_022607F8(&param0->unk_279C.unk_74);
                v3 = ov92_022607F8(&param0->unk_279C.unk_8C);
                v6 = (param0->unk_279C.unk_74.unk_00 - v4) >> 12 ;
                v7 = (param0->unk_279C.unk_8C.unk_00 - v5) >> 12 ;

                if ((v2 == 1) || (v3 == 1)) {
                    param0->unk_279C.unk_74.unk_00 -= v4;
                    param0->unk_279C.unk_8C.unk_00 -= v5;
                }
            } else {
                v6 = param0->unk_279C.unk_74.unk_00 >> 12 ;
                v7 = param0->unk_279C.unk_8C.unk_00 >> 12 ;
            }
        } else {
            v4 = param0->unk_279C.unk_44.unk_00;
            v5 = param0->unk_279C.unk_5C.unk_00;
            v2 = ov92_022607F8(&param0->unk_279C.unk_44);
            v3 = ov92_022607F8(&param0->unk_279C.unk_5C);
            v6 = (param0->unk_279C.unk_44.unk_00 - v4) >> 12 ;
            v7 = (param0->unk_279C.unk_5C.unk_00 - v5) >> 12 ;

            if ((v2 == 1) || (v3 == 1)) {
                if (param0->unk_279C.unk_A8 == 0) {
                    param0->unk_279C.unk_A8 = 1;
                    param0->unk_279C.unk_44.unk_00 -= v4;
                    param0->unk_279C.unk_5C.unk_00 -= v5;
                }
            }
        }

        v0 = ov92_02260428(&param0->unk_FC, 0, 0, v6, v7, (2.35 * 0.80), 0);
        v1 = ov92_02260428(&param0->unk_308[0], 0, 0, v6, v7, (0.80), 0);

        if (v1) {
            VecFx32 v9 = ov92_02263C8C;
            MTX_MultVec43(&v9, &param0->unk_308[0].unk_1B0, &param0->unk_1FB0);
        }

        param0->unk_279C.unk_42++;
        param0->unk_279C.unk_40--;
    } else {
        if ((param0->unk_279C.unk_00 % v8[param0->unk_279C.unk_0C]) == 0) {
            param0->unk_279C.unk_00 = 0;
            param0->unk_279C.unk_AC = 1;
        }

        if ((param0->unk_279C.unk_A4 == 0) && (param0->unk_279C.unk_A8 == 1)) {
            v6 = param0->unk_279C.unk_44.unk_00 >> 12 ;
            v7 = param0->unk_279C.unk_5C.unk_00 >> 12 ;
            v0 = ov92_02260428(&param0->unk_FC, 0, 0, v6, v7, (2.35 * 0.80), 0);
            v1 = ov92_02260428(&param0->unk_308[0], 0, 0, v6, v7, (0.80), 0);

            if (v1) {
                VecFx32 v11 = ov92_02263C74;

                MTX_MultVec43(&v11, &param0->unk_308[0].unk_1B0, &param0->unk_1FB0);
            }
        } else if ((param0->unk_279C.unk_A4 == 1) && (param0->unk_279C.unk_A8 == 1)) {
            v6 = param0->unk_279C.unk_44.unk_00 >> 12 ;
            v7 = param0->unk_279C.unk_5C.unk_00 >> 12 ;
            v0 = ov92_02260428(&param0->unk_FC, 0, 0, v6, v7, (2.35 * 0.80), 0);
            v1 = ov92_02260428(&param0->unk_308[0], 0, 0, v6, v7, (0.80), 0);

            if (v1) {
                VecFx32 v13 = ov92_02263C80;
                MTX_MultVec43(&v13, &param0->unk_308[0].unk_1B0, &param0->unk_1FB0);
            }
        }

        if ((param0->unk_279C.unk_B0 == 0) || ((param0->unk_279C.unk_AC == 1) && (param0->unk_279C.unk_A8 == 1))) {
            int v15 = inline_ov116_02263E20() % 100;
            int v16 = 0;
            int v17;
            int v18 = ov92_02263D84[param0->unk_279C.unk_0C][0].unk_04;
            s16 v19, v20;
            s16 v21, v22;
            s16 v23, v24, v25;

            v19 = v20 = 0;

            for (v17 = 0; v17 < 3; v17++) {
                if (v15 < v18) {
                    break;
                }

                v18 += ov92_02263D84[param0->unk_279C.unk_0C][v17 + 1].unk_04;
            }

            v16 = ov92_02263D84[param0->unk_279C.unk_0C][v17].unk_00;


            {
                v23 = param0->unk_1FB0.x >> 12 ;
                v25 = param0->unk_1FB0.y >> 12 ;
                v24 = param0->unk_1FB0.z >> 12 ;

                if (param0->unk_1FB0.z == ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) ) {
                    v24 = inline_ov116_02263E20_1();
                } else {
                    v24 = -inline_ov116_02263E20_2(v24);
                }

                if (param0->unk_1FB0.x == ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) ) {
                    v23 = inline_ov116_02263E20_1();
                } else {
                    v23 = inline_ov116_02263E20_2(v23);

                    if (v24 < 0) {
                        v23 *= -1;
                    }
                }

                if (param0->unk_279C.unk_0C == 0) {
                    v23 = 0;
                }

                v19 = v16 * v23;
                v20 = v16 * v24;
            }

            v21 = 20 * v23;
            v22 = 20 * v24;

            param0->unk_279C.unk_B8 = v19;
            param0->unk_279C.unk_BA = v20;

            ov92_02260798(&param0->unk_279C.unk_74, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , ((fx32)(((v21) > 0) ? ((v21) * (1 << 12) + 0.5f) : ((v21) * (1 << 12) - 0.5f))) , ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , 8);
            ov92_02260798(&param0->unk_279C.unk_8C, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , ((fx32)(((v22) > 0) ? ((v22) * (1 << 12) + 0.5f) : ((v22) * (1 << 12) - 0.5f))) , ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , 8);
            ov92_02260798(&param0->unk_279C.unk_44, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , ((fx32)(((v19) > 0) ? ((v19) * (1 << 12) + 0.5f) : ((v19) * (1 << 12) - 0.5f))) , ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , 12);
            ov92_02260798(&param0->unk_279C.unk_5C, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , ((fx32)(((v20) > 0) ? ((v20) * (1 << 12) + 0.5f) : ((v20) * (1 << 12) - 0.5f))) , ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))) , 12);

            param0->unk_279C.unk_40 = 12 + (8 + 2);
            param0->unk_279C.unk_42 = 0;
            param0->unk_279C.unk_3C = v19;
            param0->unk_279C.unk_3E = v20;
            param0->unk_279C.unk_A4 = 0;
            param0->unk_279C.unk_AC = 0;
            param0->unk_279C.unk_A8 = 0;
            param0->unk_279C.unk_B0 = 1;
        }
    }
}
