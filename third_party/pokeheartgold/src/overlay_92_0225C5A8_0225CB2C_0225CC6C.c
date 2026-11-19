#include "global.h"

#include "sprite_system.h"
#include "bg_window.h"
#include "camera.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

struct BgConfig;

struct Camera;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct Window;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

void ov92_02260A38(UnkStruct_ov116_02262A8C *param0, s16 param1, s16 param2);

void ov92_022630E8(UnkUnion_ov116_02266FEC *param0);

void ov92_0225CB2C(UnkStruct_ov116_02262A8C *param0, u32 param1, u32 param2);

BOOL ov92_0225CC6C(UnkStruct_ov116_02262A8C *param0);

typedef struct {
    s16 a[48][2];
} ov92_02263A94_Array;

extern const ov92_02263A94_Array ov92_02263A94;

BOOL ov92_0225CC6C(UnkStruct_ov116_02262A8C *param0) {
    int v0;
    int v1;
    int v2, v3;
    ov92_02263A94_Array v4 = ov92_02263A94;

    v0 = ManagedSprite_GetAnimationFrame(param0->unk_D8);
    v1 = (param0->unk_0C * 12) + v0;
    v2 = v4.a[v1][0];
    v3 = v4.a[v1][1];

    if ((v2 == 0) && (v3 == 0)) {
        ov92_0225CB2C(param0, 0, 0);
    } else {
        ov92_0225CB2C(param0, 128 + v2, 128 + v3);

        if (v2 != 0) {
            ov92_02260A38(param0, 88 + v2, 128 + v3);
        } else {
            ov92_02260A38(param0, 128 + v2, 128 + v3);
        }
    }

    if (v0 == 0) {
        if (param0->unk_08 != v0) {
            param0->unk_0C++;
        }
    }

    param0->unk_08 = v0;

    if (param0->unk_0C == 2) {
        if (ManagedSprite_GetActiveAnim(param0->unk_D8) != 2) {
            ManagedSprite_SetPositionXY(param0->unk_D8, 128, 100);
            ManagedSprite_SetAnimNoRestart(param0->unk_D8, 2);

            ov92_022630E8(&param0->unk_FC.unk_190);
            ov92_022630E8(&param0->unk_FC.unk_1A0);
            ov92_02260428(&param0->unk_FC, 0, 0, +5, +5, 0.80, 0);
            ov92_02260428(&param0->unk_FC, 0, 0, -5, -5, 0.80, 0);
        }
    }

    if ((param0->unk_0C == 3) && (v0 == 11)) {
        param0->unk_08 = 0;
        param0->unk_0C = 0;

        ManagedSprite_SetDrawFlag(param0->unk_D8, 0);

        ov92_022630E8(&param0->unk_FC.unk_190);
        ov92_022630E8(&param0->unk_FC.unk_1A0);
        ov92_02260428(&param0->unk_FC, 0, 0, +5, +5, 0.80, 0);
        ov92_02260428(&param0->unk_FC, 0, 0, -5, -5, 0.80, 0);

        return 1;
    }

    ManagedSprite_TickTwoFrames(param0->unk_D8);

    return 0;
}
