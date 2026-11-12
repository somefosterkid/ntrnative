// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_camera.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_CAMERA_H
#define BWPORT_FIELD_FIELD_CAMERA_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

// An event camera animation's target and what it animates. Names and layouts from swan
typedef struct {
    VecFx32 cameraPos;
    VecFx32 targetPos;
    VecFx32 extraTranslation;
    u16 pitch;
    u16 yaw;
    fx32 distance;
    u16 fov;
    u16 padFov;
} FieldEvCameraAnimationCoords;

typedef struct {
    BOOL animateExtraTranslation;
    BOOL animatePitch;
    BOOL animateYaw;
    BOOL animateTargetDistance;
    BOOL animateFOV;
    BOOL animateTargetPos;
} FieldEvCameraAnimationFlags;

typedef struct {
    FieldEvCameraAnimationCoords targetCoords;
    FieldEvCameraAnimationFlags flags;
} FieldEvCameraAnimationSetup;

void FieldCamera_CoordsGetEye(FieldCamera *camera, VecFx32 *eye);
void FieldCamera_CoordsGetTarget(FieldCamera *camera, VecFx32 *target);
void FieldCamera_CoordsSetTarget(FieldCamera *camera, const VecFx32 *target);
void FieldCamera_CoordsSetEye(FieldCamera *camera, const VecFx32 *eye);
// Stop the camera following its target
void FieldCamera_ClearBind(FieldCamera *camera);
void FieldCamera_ChangeTransformType(FieldCamera *camera, u32 type);
u32 FieldCamera_GetTransformType(FieldCamera *camera);
void FieldCamera_SetTransformType(FieldCamera *camera, u32 type);
// Runs the camera's own callback (anm.callback, with anm.calcArg), then its transform type's function
void FieldCamera_CalcTransform(FieldCamera *camera, u16 keys);
void FieldCamera_FinishDelay(FieldCamera *camera);
BOOL FieldCamera_IsDelayActive(FieldCamera *camera);
// What the camera follows
void *FieldCamera_GetBind(FieldCamera *camera);
void FieldCamera_SetDefaultsIndex(FieldCamera *camera, u32 index);
// The index of the defaults the camera has
u32 func_ov021_0218e188(FieldCamera *camera);
void FieldCamera_SetUseBoundaryEnable(FieldCamera *camera, BOOL enable);
void FieldCamera_EnableDelay(FieldCamera *camera);
void FieldCamera_SetBind(FieldCamera *camera, const VecFx32 *bind);
void FieldCamera_EVCameraInit(FieldCamera *camera);
void FieldCameraAnm_EnsureInitDone(FieldCamera *camera);
void FieldCameraAnm_SetAnimation(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames);
void FieldCameraAnm_SetReturnAnimation(FieldCamera *camera, const FieldEvCameraAnimationFlags *flags, u16 frames);
BOOL FieldCamera_IsAnimating(FieldCamera *camera);
void FieldCameraAnm_EVCameraEnd(FieldCamera *camera);
void func_ov021_0218f930(FieldCamera *camera, const VecFx32 *cameraPos, const VecFx32 *targetPos, const FieldEvCameraAnimationFlags *flags, u16 frames);

typedef struct FieldCameraAnimationController FieldCameraAnimationController;

// What plays a FieldCameraAnimation on the field's camera: the camera's transform type, boundary and bind it takes
// over are kept for the end
struct FieldCameraAnimationController {
    Field *field;
    FieldCamera *camera;
    const FieldCameraAnimation *animation;
    u32 started;
    u32 transformType;
    u32 useBoundaryEnable;
    const VecFx32 *bind;
    u32 hasBind;
};

// field_camera_anime.c's: the camera readied for the animation, put at its start coords, the animation started
// (at its end at once when it has no frames), what the controller took over given back, the controller freed
void FieldCameraAnimationController_PrepareCamera(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_CommitSrcCoords(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_StartAnimation(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_FinishCamera(FieldCameraAnimationController *controller);
void FieldCameraAnimationController_FreeAnimation(FieldCameraAnimationController *controller);

// Layouts from Black's assembly (ov021 0x0218e018-0x0218fe78); the fields not named are not known yet
// One 0x18-byte entry of the camera's boundary
typedef struct {
    s32 type;
    s32 state;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    // The last word is one value in some functions and two angles (u16 unk_14, unk_16) in others
    union {
        s32 unk_14;
        u16 angles[2];
    };
} FieldCameraBoundaryEntry;

// The camera's animation state, at 0xf0 of the camera (pokebw2's FieldCameraEvAnimation and calcFunc)
struct FieldCameraAnm {
    u32 savedTransformType;
    FieldEvCameraAnimationCoords saved1;
    FieldEvCameraAnimationCoords saved2;
    FieldEvCameraAnimationCoords coords;
    u16 frames;
    u16 frame;
    BOOL initDone;
    BOOL active;
    FieldEvCameraAnimationFlags flags;
    // What callback is called with
    void *calcArg;
    // Called before each transform, such as the event camera's animation
    void (*callback)(FieldCamera *camera, void *arg);
};

// One 0x2c-byte record of a camera's defaults, read from the ov021 camera archive. With bind, the camera follows
// what it is bound to, at target from it; without, it looks at target
struct FieldCameraDefaults {
    s32 distance;
    s32 pitch;
    s32 yaw;
    u8 unk_0C[0x5];
    u8 bufferMode;
    u16 fov;
    fx32 zNear;
    fx32 zFar;
    BOOL bind;
    VecFx32 target;
};

// The camera's delay: rings of the last targets and eyes it follows behind (pokebw2's), in field_camera.c
typedef struct FieldCameraDelay FieldCameraDelay;

// Field names from pokebw2 (the same layout, 0x1b0 bytes)
struct FieldCamera {
    HeapID heapId;
    ArcTool *defaultsArc;
    ArcTool *boundaryArc;
    G3DCamera *g3dCamera;
    u32 defaultsIndex;
    u32 transformType;
    const VecFx32 *refBind;
    const VecFx32 *bind;
    const VecFx32 *eyeBind;
    VecFx32 eye;
    VecFx32 eyeOffset;
    VecFx32 eyeActual;
    VecFx32 target;
    VecFx32 targetActual;
    VecFx32 targetOffset;
    VecFx32 targetBase;
    u16 pitch;
    u16 yaw;
    fx32 zoom;
    u16 fov;
    u16 angleYaw;
    u16 anglePitch;
    FieldCameraBoundaryEntry boundary[4];
    u8 boundaryCount;
    u8 useBoundaryEnable;
    u16 boundaryId;
    FieldCameraDelay *delay;
    struct FieldCameraAnm anm;
};

// Layout checked against Black's assembly (0x18 bytes); the fields' meaning is not known yet
// How the event camera shakes
struct FieldEvCameraShake {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u8 unk0E;
    u8 unk0F;
    u32 unk10;
    u32 unk14;
};

// Black 2's, not checked against Black
// Where a camera animation of the camera animation controller starts or ends
struct FieldCameraCoords {
    u16 pitch;
    u16 yaw;
    fx32 distance;
    VecFx32 target;
    VecFx32 offset;
};

// Black 2's; twinned code confirms the offsets of frames, dst, unk48 and unk4C in Black
struct FieldCameraAnimation {
    u32 frames;
    FieldCameraCoords src;
    FieldCameraCoords dst;
    BOOL unk48;
    BOOL unk4C;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
FieldCameraAnimationController *Field_CreateCameraAnimationController(Field *field);
void FieldCameraAnimationController_SetAnimation(FieldCameraAnimationController *controller,
                                                 const FieldCameraAnimation *animation);
G3DCamera *FieldCamera_GetG3DCamera(FieldCamera *camera);
void FieldCamera_CoordsGetTargetOffset(FieldCamera *camera, VecFx32 *offset);
void FieldCamera_CoordsSetTargetOffset(FieldCamera *camera, const VecFx32 *offset);
u16 FieldCamera_CoordsGetPitch(FieldCamera *camera);
u16 FieldCamera_CoordsGetYaw(FieldCamera *camera);
fx32 FieldCamera_CoordsGetZoom(FieldCamera *camera);
void FieldCamera_CoordsSetPitch(FieldCamera *camera, u16 pitch);
void FieldCamera_CoordsSetZoom(FieldCamera *camera, fx32 zoom);
void FieldCamera_SetFOV(FieldCamera *camera, u16 fov);
void FieldCamera_CoordsSetYaw(FieldCamera *camera, u16 yaw);
// Whether the camera keeps inside the zone's boundary
BOOL FieldCamera_IsUseBoundaryEnable(FieldCamera *camera);
void FieldCamera_ResetBind(FieldCamera *camera);
void FieldCamera_LoadDefaults(FieldCamera *camera);
void FieldCamera_DisableDelay(FieldCamera *camera);
// Whether the camera has a delay it can finish
BOOL FieldCamera_SupportsDelay(FieldCamera *camera);
void FieldCameraAnm_SetAnimationRealTime(FieldCamera *camera, const FieldEvCameraAnimationSetup *setup, u16 frames);
void FieldCameraAnm_SetLoadDefaultsAnimation(FieldCamera *camera, u16 frames);
void FieldCameraBoundary_LoadDummy(FieldCamera *camera);
void FieldCameraBoundary_ChangeID(FieldCamera *camera, u16 boundaryId, HeapID heapId);
// The shake state a caller of func_ov043_021f41b8 keeps (pokebw2's DriftveilGymShake, Black's offsets)
typedef struct FieldCameraShake {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    s32 unk_04;
    FieldCamera *unk_08;
    void *unk_0C;
    VecFx32 unk_10;
} FieldCameraShake;

GameEvent *EventEvCameraShake_Create(GameSystem *gsys, const FieldEvCameraShake *shake);
void func_ov021_0218ed04(FieldCamera *camera, const VecFx32 *src);
// The camera's pitch
u16 *func_ov021_0218ed3c(FieldCamera *camera);
// The camera's yaw (angleYaw)
u16 *func_ov021_0218ed50(FieldCamera *camera);
void FieldCamera_ReadDefaults(FieldCamera *camera, struct FieldCameraDefaults *defaults);
FieldCamera *func_ov021_0218e018(u32 param_1, u32 param_2, G3DCamera *camera, VecFx32 *param_4, HeapID heap);
void func_ov021_0218e0e4(FieldCamera *camera);
void func_ov021_0218ed20(FieldCamera *camera, VecFx32 *up);
// The pitch, yaw and distance of a direction
void FieldCamera_CoordsDerive(VecFx32 *vec, u16 *pitch, u16 *yaw, fx32 *zoom);
// The target and the eye as the camera last put them, after its boundaries
void FieldCamera_GetTarget(FieldCamera *camera, VecFx32 *out);
void FieldCamera_GetEye(FieldCamera *camera, VecFx32 *out);
void FieldCamera_GetAnimationCoords(FieldCamera *camera, FieldCameraCoords *coords);


// The coords of the camera animation in progress (the camera's anm.coords)
FieldEvCameraAnimationCoords *func_ov021_0218fe70(FieldCamera *camera);

#endif // BWPORT_FIELD_FIELD_CAMERA_H
