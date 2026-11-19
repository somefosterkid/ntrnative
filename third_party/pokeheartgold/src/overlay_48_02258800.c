#include "global.h"

#include "constants/heap.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_45_wifi_plaza_system.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "screen_fade_internal.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct Easy3DModel;

struct Easy3DObject;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct MessageLoader;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov66_0222DFF8_t;

struct WiFiHistory;

struct Window;

struct YesNoTouchMenu;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

void *OverlayManager_CreateAndGetData(ApplicationManager *appMan, u32 size, enum HeapID heapID);

void *OverlayManager_GetArgs(ApplicationManager *appMan);

typedef struct WiFiHistory WiFiHistory;

typedef struct {
    const UnkStruct_ov66_02231560 *unk_00;
    SaveData *saveData;
    UnkStruct_ov66_02231428 unk_08;
    WiFiPlazaSystem *unk_0C;
} UnkStruct_ov66_02230F50;

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

typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;

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

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

typedef struct MessageLoader MessageLoader;

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
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

Options *Save_PlayerData_GetOptionsAddr(SaveData *saveData);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

typedef struct StringTemplate StringTemplate;

WiFiHistory *Save_WiFiHistory_Get(SaveData *saveData);

u8 WifiHistory_GetPlayerCountry(const WiFiHistory *wiFiHistory);

u8 WiFiHistory_GetPlayerRegion(const WiFiHistory *wiFiHistory);

typedef struct YesNoTouchMenu YesNoTouchMenu;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u16 unk_00[2];
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov69_0225F118;

typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Easy3DObject unk_24;
    Easy3DModel unk_9C;
} UnkStruct_ov69_0225D71C;

typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Easy3DObject unk_B810[3];
    Easy3DModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;

typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_ov69_0225DA74 unk_04;
    WiFiHistory *wiFiHistory;
    Options *options;
    int unk_14;
    int unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    UnkStruct_ov69_0225D35C unk_20;
    UnkStruct_ov69_0225EF54 unk_1CC;
    UnkStruct_ov69_0225D71C unk_1DC;
    UnkStruct_ov69_0225CA7C unk_288;
    UnkStruct_ov69_0225DAEC unk_BC30;
    UnkStruct_ov69_0225DC48 unk_BC44;
    UnkStruct_ov69_0225E084 unk_BC70;
    UnkStruct_ov69_0225DDC8 unk_BF64;
} UnkStruct_ov69_0225CE64;

void ov48_02259090(void *param0);

void ov48_02259130(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1);

void ov48_022593F4(UnkStruct_ov69_0225CE64 *param0, const UnkStruct_ov66_02230F50 *param1);

void ov48_02259464(UnkStruct_ov69_0225D35C *param0, Options *options, u32 heapID);

void ov48_02259824(UnkStruct_ov69_0225D71C *param0, UnkStruct_ov69_0225D35C *param1, u32 heapID);

void ov48_022598EC(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, UnkStruct_ov69_0225D35C *param2, UnkStruct_ov69_0225DA74 param3, u32 heapID);

void ov48_02259BC0(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225DA74 param1, enum HeapID param2);

void ov48_02259D00(UnkStruct_ov69_0225DC48 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, u32 heapID);

void ov48_02259EAC(UnkStruct_ov69_0225DDC8 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, SaveData *saveData, u32 heapID);

void ov48_0225A00C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, enum HeapID heapID);

void ov48_0225B068(UnkStruct_ov69_0225EF54 *param0, u32 heapID);

void ov48_0225A894(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1, const UnkStruct_ov69_0225F118 *param2, u16 param3, enum HeapID heapID);

int ov48_02258800(ApplicationManager *appMan, int *param1);

int ov48_02258800(ApplicationManager *appMan, int *param1) {
    UnkStruct_ov69_0225CE64 *v0;
    UnkStruct_ov66_02230F50 *v1 = OverlayManager_GetArgs(appMan);
    Heap_Create(HEAP_ID_3, HEAP_ID_112, 0x50000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov69_0225CE64), HEAP_ID_112);
    memset(v0, 0, sizeof(UnkStruct_ov69_0225CE64));

    v0->wiFiHistory = Save_WiFiHistory_Get(v1->saveData);
    v0->options = Save_PlayerData_GetOptionsAddr(v1->saveData);
    v0->unk_14 = WifiHistory_GetPlayerCountry(v0->wiFiHistory);
    v0->unk_18 = WiFiHistory_GetPlayerRegion(v0->wiFiHistory);
    v0->unk_1C = v1->unk_08;

    ov48_022593F4(v0, v1);
    ov48_02259464(&v0->unk_20, v0->options, HEAP_ID_112);
    ov48_02259824(&v0->unk_1DC, &v0->unk_20, HEAP_ID_112);
    ov48_0225B068(&v0->unk_1CC, HEAP_ID_112);
    ov48_022598EC(&v0->unk_288, v1->unk_00, &v0->unk_20, v0->unk_04, HEAP_ID_112);
    ov48_02259BC0(&v0->unk_BC30, v0->unk_04, HEAP_ID_112);
    ov48_02259D00(&v0->unk_BC44, &v0->unk_20, &v0->unk_1CC, HEAP_ID_112);
    ov48_02259EAC(&v0->unk_BF64, &v0->unk_20, &v0->unk_1CC, v1->saveData, HEAP_ID_112);
    ov48_0225A00C(&v0->unk_BC70, &v0->unk_20, &v0->unk_1CC, HEAP_ID_112);
    ov48_02259130(v0, v1);

    Main_SetVBlankIntrCB(ov48_02259090, v0);
    HBlankInterruptDisable();

    return 1;
}

void *OverlayManager_GetData(ApplicationManager *appMan, int *);

BOOL ov45_0222A330(const WiFiPlazaSystem *param0);

void ov45_0222A4A8(WiFiPlazaSystem *param0);

void ov45_0222A520(WiFiPlazaSystem *param0, BOOL param1);

BOOL ov45_0222D844(void);

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

BOOL ov48_02258F64(UnkStruct_ov69_0225CE64 *param0);

void ov48_02259030(UnkStruct_ov69_0225CE64 *param0);

void ov48_02259050(UnkStruct_ov69_0225CE64 *param0);

void ov69_0225DEC0(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, enum HeapID heapID);

int ov48_02258920(ApplicationManager *appMan, int *param1);

int ov48_02258920(ApplicationManager *appMan, int *param1) {
    UnkStruct_ov69_0225CE64 *v0;
    UnkStruct_ov66_02230F50 *v1;
    BOOL v2;

    v0 = OverlayManager_GetData(appMan, param1);
    v1 = OverlayManager_GetArgs(appMan);

    switch (*param1) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_112);
        ov45_0222A520(v1->unk_0C, 1);
        (*param1)++;
        break;
    case 1:
        v2 = IsPaletteFadeFinished();

        if (v2 == 1) {
            (*param1)++;
        }
        break;
    case 2:
        v2 = ov48_02258F64(v0);
        ov48_02259030(v0);

        if (ov45_0222A330(v1->unk_0C) == 1) {
            ov45_0222A4A8(v1->unk_0C);
            v2 = 1;
        }

        if (ov45_0222D844() == 1) {
            v2 = 1;
        }

        if (v2 == 1) {
            (*param1) = 5;
        }
        break;
    case 5:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_112);
        (*param1)++;
        break;
    case 6:
        v2 = IsPaletteFadeFinished();

        if (v2 == 1) {
            return 1;
        }
        break;
    }

    ov48_02259050(v0);

    return 0;
}
