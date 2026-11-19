#include "global.h"

#include "camera.h"
#include "fx_util.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "yes_no_prompt.h"
#include "font.h"

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CharTransferAllocation;

struct Field3dModel;

struct Field3dObject;

struct TextOBJ;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct PaletteData;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef u32 TextColor;

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;

typedef struct {
    FontOAM *unk_00;
    CharTransferAllocation unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

extern const CameraAngle ov89_0225C9B8;

void ov89_0225A260(UnkStruct_ov113_0225DBCC *param0, UnkStruct_ov113_0225E250 *param1, const String *param2, enum Font param3, TextColor param4, int param5, int param6, int param7, int param8, int param9);

void ov89_02259B00(UnkStruct_ov113_0225DBCC *param0);

void ov89_02259B00(UnkStruct_ov113_0225DBCC *param0) {
    VecFx32 v0 = { 0, -FX32_ONE * 8, 0 };

    param0->camera = Camera_New(HEAP_ID_125);

    Camera_Init_FromTargetDistanceAndAngle(&v0, FX32_CONST(124), &ov89_0225C9B8, (22 * 0xffff) / 360, 0, 0, param0->camera);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_ONE * 900, param0->camera);
    Camera_SetStaticPtr(param0->camera);

    {
        u16 v1;
        fx32 v2, v3;
        fx32 v4, v5;

        v1 = Camera_GetPerspectiveAngle(param0->camera);
        v2 = Camera_GetDistance(param0->camera);
        v3 = FX32_ONE * 4 / 3;

        FX_CalcLinearFov(v1, v2, v3, &v4, &v5);

        param0->unk_D0 = v4;
        param0->unk_D4 = v5;
    }
}
