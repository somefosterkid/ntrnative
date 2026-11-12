// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_camera.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle view's 3D camera. The name is the ROM's string, from GFL_HeapAllocate's assert. BtlvCamera_Create is
// swan's name; the others are ours

#include "battle/btlv_camera.h"
#include "types.h"
#include "battle/btlv_effect.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"

// The camera's work, 0xb8 bytes
struct BtlvCamera {
    TCBManager *tcbManager; // 0x00  never read
    G3DCamera *camera;      // 0x04
    s32 pitch;              // 0x08  the angles and distance of the position from the target
    s32 yaw;                // 0x0c
    fx32 distance;          // 0x10
    u32 flags;              // 0x14  bit 0: the position moves, bit 1: the target moves, bit 2: a shake runs
    VecFx32 pos;            // 0x18
    VecFx32 target;         // 0x24
    VecFx32 posGoal;        // 0x30
    VecFx32 targetGoal;     // 0x3c
    VecFx32 posStep;        // 0x48
    VecFx32 targetStep;     // 0x54
    VecFx32 shakeOffset;    // 0x60  added to both the position and the target
    s32 wait;               // 0x6c  frames until the next step
    s32 waitReset;          // 0x70
    s32 brakeFrames;        // 0x74  steps until the steps are halved, 0 for never
    BtlvEffToolMove shake;  // 0x78
    HeapID heapId;          // 0xb4
};

#define CAMERA_MOVE_POS (1 << 0)
#define CAMERA_MOVE_TARGET (1 << 1)
#define CAMERA_SHAKE (1 << 2)

static void func_ov094_021f723c(BtlvCamera *camera);
static void func_ov094_021f73a4(BtlvCamera *camera);

// The position, target and up vector the camera starts with; the declaration order lays them out as in the ROM
static const VecFx32 data_ov094_02209cf4 = { FX32_CONST(6.7), FX32_CONST(6.7), FX32_CONST(17.3) };
static const VecFx32 data_ov094_02209ce8 = { 0, FX32_CONST(2.6), 0 };
static const VecFx32 data_ov094_02209d00 = { 0, FX32_ONE, 0 };

BtlvCamera *BtlvCamera_Create(TCBManager *tcbManager, HeapID heapId) {
    BtlvCamera *camera = GFL_HeapAllocate(heapId, sizeof(BtlvCamera), TRUE, "btlv_camera.c", 0x56);

    camera->heapId = heapId;
    camera->tcbManager = tcbManager;
    // The sine and cosine of 13 degrees
    camera->camera =
        GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, 0x399, 0xf97, FX32_CONST(1.35), 0, FX32_ONE, FX32_CONST(512),
                            0, &data_ov094_02209cf4, &data_ov094_02209d00, &data_ov094_02209ce8, heapId);
    func_ov094_021f73a4(camera);
    GFL_G3DCameraFlush(camera->camera);
    func_ov094_021f7208(&camera->pos, &camera->target);
    return camera;
}

// pokebw2's BtlvCamera_Delete
void func_ov094_021f6ec0(BtlvCamera *camera) {
    GFL_G3DCameraFree(camera->camera);
    GFL_HeapFree(camera);
}

// pokebw2's BtlvCamera_Main
void func_ov094_021f6ed4(BtlvCamera *camera) {
    func_ov094_021f723c(camera);
}

// pokebw2's BtlvCamera_SetPosTarget
void func_ov094_021f6edc(BtlvCamera *camera, const VecFx32 *pos, const VecFx32 *target) {
    if (pos != NULL) {
        camera->pos.x = pos->x;
        camera->pos.y = pos->y;
        camera->pos.z = pos->z;
        GFL_G3DCameraSetLookatPos(camera->camera, pos);
    }
    if (target != NULL) {
        camera->target.x = target->x;
        camera->target.y = target->y;
        camera->target.z = target->z;
        GFL_G3DCameraSetLookatTarget(camera->camera, target);
    }
    GFL_G3DCameraFlush(camera->camera);
    func_ov094_021f73a4(camera);
}

// pokebw2's BtlvCamera_Rotate
void func_ov094_021f6f20(BtlvCamera *camera, s32 pitch, s32 yaw) {
    func_ov094_021f7108(camera, pitch, yaw, &camera->pos, &camera->target);
    GFL_G3DCameraSetLookatPos(camera->camera, &camera->pos);
    GFL_G3DCameraFlush(camera->camera);
}

void func_ov094_021f6f48(BtlvCamera *camera, const VecFx32 *pos, const VecFx32 *target, int frames, int wait,
                       int brakeFrames) {
    camera->brakeFrames = brakeFrames;
    camera->wait = wait;
    camera->waitReset = wait;
    if (pos != NULL) {
        camera->posGoal.x = pos->x;
        camera->posGoal.y = pos->y;
        camera->posGoal.z = pos->z;
        func_ov094_021f918c(&camera->pos, pos, &camera->posStep, FX32_CONST(frames));
        camera->flags |= CAMERA_MOVE_POS;
    }
    if (target != NULL) {
        camera->targetGoal.x = target->x;
        camera->targetGoal.y = target->y;
        camera->targetGoal.z = target->z;
        func_ov094_021f918c(&camera->target, target, &camera->targetStep, FX32_CONST(frames));
        camera->flags |= CAMERA_MOVE_TARGET;
    }
}

// Shakes the camera sideways (dir 1) or vertically by amplitude: count full swings of 4 * frames steps
// pokebw2's BtlvCamera_Shake
void func_ov094_021f7000(BtlvCamera *camera, int dir, fx32 amplitude, int unused, int frames, int wait, int count) {
    camera->shake.type = 3;
    camera->shake.stepTime = frames;
    camera->shake.stepTimeReset = frames;
    camera->shake.wait = 0;
    camera->shake.waitReset = wait;
    camera->shake.start.x = 0;
    camera->shake.count = count * 4;
    camera->shake.start.y = 0;
    camera->shake.start.z = 0;
    if (dir == 1) {
        camera->shake.end.x = amplitude;
        camera->shake.end.y = 0;
        camera->shake.step.x = FX_Div(amplitude, FX32_CONST(frames));
        camera->shake.step.y = 0;
    } else {
        camera->shake.end.x = 0;
        camera->shake.end.y = amplitude;
        camera->shake.step.x = 0;
        camera->shake.step.y = FX_Div(amplitude, FX32_CONST(frames));
    }
    camera->shake.end.z = 0;
    camera->shake.step.z = 0;
    camera->flags |= CAMERA_SHAKE;
}

// pokebw2's BtlvCamera_GetPosTarget
void func_ov094_021f70f0(BtlvCamera *camera, VecFx32 *pos, VecFx32 *target) {
    GFL_G3DCameraGetLookatPos(camera->camera, pos);
    GFL_G3DCameraGetLookatTarget(camera->camera, target);
}

// Turns the camera's angles by pitch and yaw and gives the position they put it at around its target
// pokebw2's BtlvCamera_CalcRotatedPos
void func_ov094_021f7108(BtlvCamera *camera, s32 pitch, s32 yaw, VecFx32 *pos, VecFx32 *target) {
    GFL_G3DCameraGetLookatTarget(camera->camera, target);
    camera->pitch += pitch;
    camera->yaw += yaw;
    pos->x = FX_Mul(FX_CosIdx(camera->yaw), FX_CosIdx(camera->pitch));
    pos->y = FX_SinIdx(camera->pitch);
    pos->z = FX_Mul(FX_SinIdx(camera->yaw), FX_CosIdx(camera->pitch));
    pos->x = FX_Mul(pos->x, camera->distance);
    pos->y = FX_Mul(pos->y, camera->distance);
    pos->z = FX_Mul(pos->z, camera->distance);
    pos->x += target->x;
    pos->y += target->y;
    pos->z += target->z;
}

// pokebw2's BtlvCamera_GetDefaultPosTarget
void func_ov094_021f7208(VecFx32 *pos, VecFx32 *target) {
    pos->x = FX32_CONST(6.7);
    pos->y = FX32_CONST(6.7);
    pos->z = FX32_CONST(17.3);
    target->x = 0;
    target->y = FX32_CONST(2.6);
    target->z = 0;
}

// pokebw2's BtlvCamera_IsMoving
BOOL func_ov094_021f722c(BtlvCamera *camera) {
    if (camera->flags) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvCamera_Update
static void func_ov094_021f723c(BtlvCamera *camera) {
    BOOL done = TRUE;
    VecFx32 pos;
    VecFx32 target;

    if (camera->flags) {
        if (camera->wait == 0) {
            camera->wait = camera->waitReset;
            if (camera->brakeFrames != 0 && --camera->brakeFrames == 0) {
                if (camera->posStep.x >> 1) {
                    camera->posStep.x >>= 1;
                }
                if (camera->posStep.y >> 1) {
                    camera->posStep.y >>= 1;
                }
                if (camera->posStep.z >> 1) {
                    camera->posStep.z >>= 1;
                }
                if (camera->targetStep.x >> 1) {
                    camera->targetStep.x >>= 1;
                }
                if (camera->targetStep.y >> 1) {
                    camera->targetStep.y >>= 1;
                }
                if (camera->targetStep.z >> 1) {
                    camera->targetStep.z >>= 1;
                }
            }
            if (camera->flags & CAMERA_MOVE_POS) {
                func_ov094_021f9220(&camera->pos.x, &camera->posStep.x, &camera->posGoal.x, &done);
                func_ov094_021f9220(&camera->pos.y, &camera->posStep.y, &camera->posGoal.y, &done);
                func_ov094_021f9220(&camera->pos.z, &camera->posStep.z, &camera->posGoal.z, &done);
                if (done == TRUE) {
                    camera->flags &= ~CAMERA_MOVE_POS;
                }
            }
            if (camera->flags & CAMERA_MOVE_TARGET) {
                func_ov094_021f9220(&camera->target.x, &camera->targetStep.x, &camera->targetGoal.x, &done);
                func_ov094_021f9220(&camera->target.y, &camera->targetStep.y, &camera->targetGoal.y, &done);
                func_ov094_021f9220(&camera->target.z, &camera->targetStep.z, &camera->targetGoal.z, &done);
                if (done == TRUE) {
                    camera->flags &= ~CAMERA_MOVE_TARGET;
                }
            }
            if (camera->flags & CAMERA_SHAKE) {
                done = func_ov094_021f9260(&camera->shake, &camera->shakeOffset);
                if (done == TRUE) {
                    camera->flags &= ~CAMERA_SHAKE;
                }
            }
            pos.x = camera->pos.x + camera->shakeOffset.x;
            pos.y = camera->pos.y + camera->shakeOffset.y;
            pos.z = camera->pos.z + camera->shakeOffset.z;
            target.x = camera->target.x + camera->shakeOffset.x;
            target.y = camera->target.y + camera->shakeOffset.y;
            target.z = camera->target.z + camera->shakeOffset.z;
            GFL_G3DCameraSetLookatPos(camera->camera, &pos);
            GFL_G3DCameraSetLookatTarget(camera->camera, &target);
            GFL_G3DCameraFlush(camera->camera);
            func_ov094_021f73a4(camera);
        } else {
            camera->wait--;
        }
    }
}

// Takes the angles and distance of the position from the target from the G3D camera
// pokebw2's BtlvCamera_UpdateAngles
static void func_ov094_021f73a4(BtlvCamera *camera) {
    VecFx32 pos;
    VecFx32 target;

    GFL_G3DCameraGetLookatPos(camera->camera, &pos);
    GFL_G3DCameraGetLookatTarget(camera->camera, &target);
    pos.x -= target.x;
    pos.y -= target.y;
    pos.z -= target.z;
    camera->pitch = FX_Atan2Idx(pos.y, pos.z);
    camera->yaw = FX_Atan2Idx(pos.z, pos.x);
    camera->distance = VEC_Mag(&pos);
}
