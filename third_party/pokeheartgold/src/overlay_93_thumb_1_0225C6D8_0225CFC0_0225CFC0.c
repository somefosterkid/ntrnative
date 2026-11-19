#include "global.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct Camera;

struct CameraAngle;

struct CharTransferAllocation;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct TextOBJ;

struct GF3DVramMan;

struct ManagedSprite;

struct MsgData;

struct PaletteData;

struct ParticleSystem;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

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

Camera *Camera_New(const enum HeapID heapID);

void Camera_SetStaticPtr(Camera *camera);

void Camera_SetPerspectiveClippingPlane(const fx32 nearClip, const fx32 farClip, Camera *camera);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

extern const CameraAngle ov93_02262A18;

void ov93_0225CFC0(UnkStruct_ov117_02261280_t *param0);

extern const VecFx32 ov93_02262A38;

void ov93_0225CFC0(UnkStruct_ov117_02261280_t *param0) {
    VecFx32 v0 = ov93_02262A38;

    param0->camera1 = Camera_New(HEAP_ID_117);

    Camera_Init_FromTargetDistanceAndAngle(&v0, 0x7b << FX32_SHIFT, &ov93_02262A18, (22 * 0xffff) / 360, 1, 0, param0->camera1);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_ONE * 900, param0->camera1);
    Camera_SetStaticPtr(param0->camera1);

    param0->camera2 = Camera_New(HEAP_ID_117);

    Camera_Init_FromTargetDistanceAndAngle(&v0, 80 << FX32_SHIFT, &ov93_02262A18, (22 * 0xffff) / 360, 1, 0, param0->camera2);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_ONE * 900, param0->camera2);
}
