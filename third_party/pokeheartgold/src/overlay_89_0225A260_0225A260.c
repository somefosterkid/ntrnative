#include "global.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"
#include "constants/heap.h"
#include "unk_02013534_font_system.h"
#include "font_types_def.h"
#include "font.h"

struct BgConfig;

struct Camera;

struct FontOAM;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MessageLoader;

struct NARC;

struct PaletteData;

struct SaveData;

struct SpriteList;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct TextPrinterTemplate;

struct UnkStruct_02012744_t;

struct UnkStruct_ov113_0225DBCC_t;

struct UnkStruct_ov113_0225E6B8_t;

struct UnkStruct_ov66_0222DFF8_t;

struct YesNoTouchMenu;

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

void InitWindow(Window *window);

void AddTextWindowTopLeftCorner(BgConfig *bgConfig, Window *window, u8 width, u8 height, u16 baseTile, u8 fillVal);

void RemoveWindow(Window *window);

typedef struct {
    const UnkStruct_02012744 *unk_00;
    const Window *unk_04;
    SpriteList *unk_08;
    const NNSG2dImagePaletteProxy *unk_0C;
    const Sprite *unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_020127E8;

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



SpriteList *SpriteManager_GetSpriteList(SpriteManager *spriteMan);

typedef struct StringTemplate StringTemplate;

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

typedef BOOL (*TextPrinterCallback)(TextPrinterTemplate *, u16);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColorAndSpacing(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, u32 letterSpacing, u32 lineSpacing, TextPrinterCallback callback);

FontOAM *sub_020135D8(const UnkStruct_020127E8 *param0);

int sub_02013688(const Window *param0, int param1, enum HeapID heapID);

void sub_020136B4(FontOAM *fontOAM, int x, int y);

void sub_020138E0(FontOAM *param0, u32 param1);

typedef struct YesNoTouchMenu YesNoTouchMenu;

typedef struct {
    Easy3DModel unk_00;
    Easy3DObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;

typedef struct {
    FontOAM *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

void ov89_0225A368(const String *param0, int param1, int *param2, int *param3);

void ov89_0225A260(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9);

void ov89_0225A260(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9) {
    UnkStruct_020127E8 v0;
    Window v1;
    UnkStruct_02021AC8 v2;
    int v3;
    FontOAM *v4;
    BgConfig *v5;
    SpriteManager *v6;
    int v7, v8;

    ((param1->unk_00 == 0L) ? (void)0 : GF_AssertFail());

    v5 = param0->unk_08;
    v6 = param0->unk_20;

    ov89_0225A368(param2, param3, &v7, &v8);

    InitWindow(&v1);
    AddTextWindowTopLeftCorner(v5, &v1, v8, 16 / 8, 0, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&v1, param3, param2, 0, 0, 0xFF, param4, 0, 0, 0L);

    v3 = sub_02013688(&v1, NNS_G2D_VRAM_TYPE_2DMAIN, (enum HeapID)125);
    sub_02021AC8(v3, 1, NNS_G2D_VRAM_TYPE_2DMAIN, &v2);

    if (param9 == 1) {
        param7 -= v7 / 2;
    }

    param8 -= 8;

    v0.unk_00 = param0->unk_10;
    v0.unk_04 = &v1;
    v0.unk_08 = SpriteManager_GetSpriteList(v6);
    v0.unk_0C = SpriteManager_FindPlttResourceProxy(v6, param6);
    v0.unk_10 = 0L;
    v0.unk_14 = v2.offset;
    v0.unk_18 = param7;
    v0.unk_1C = param8;
    v0.unk_20 = 0;
    v0.unk_24 = 51;
    v0.unk_28 = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.heapID = (enum HeapID)125;

    v4 = sub_020135D8(&v0);

    sub_020138E0(v4, param5);
    sub_020136B4(v4, param7, param8);
    RemoveWindow(&v1);

    param1->unk_00 = v4;
    param1->unk_04 = v2;
    param1->unk_10 = v7;
}
