#include "global.h"
#include "simple3dmodel.h"

#include "overlay_01_02200858_work.h"

#include "fx_util.h"
#include "unk_020689C8.h"
#include "overlay_01_021F1348.h"

struct FieldEffectManager;

struct PlayerAvatar;

struct Simple3DModel;

struct Simple3DRenderObj;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void sub_020699AC(Simple3DRenderObj *renderObj, const VecFx32 *position, const VecFx32 *scale, const MtxFx33 *rotation);

extern const VecFx32 ov01_02209354[4][2];

void ov01_02200A08(OverworldAnimManager *param0, void *param1);

void ov01_02200A08(OverworldAnimManager *param0, void *param1) {
    UnkStruct_021F6264 *v0 = param1;

    if ((v0->unk_00 == 1) || (v0->unk_04 == -1)) {
        return;
    }

    {
        u32 v1 = 0;
        MtxFx33 v2;
        VecFx32 v3, v4 = { FX32_ONE, FX32_ONE, FX32_ONE };
        Simple3DRenderObj *v5 = &v0->unk_24.unk_04->unk_18;
        const VecFx32 *v6 = &ov01_02209354[v0->unk_04][v0->unk_20];

        switch (v0->unk_04) {
        case 0:
            v1 = 180;
            break;
        case 1:
            break;
        case 2:
            v1 = 270;
            break;
        case 3:
            v1 = 90;
            break;
        }

        MTX_Rot33Angles(&v2, 0, v1, 0);
        sub_02068DB8(param0, &v3);

        v3.x += v6->x;
        v3.y += v6->y;
        v3.z += v6->z;

        sub_020699AC(v5, &v3, &v4, &v2);
    }
}
