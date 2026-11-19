#include "global.h"

#include "sprite.h"
#include "heap.h"
#include "overlay_71_trade_ball_flyby_phase.h"
#include "gf_gfx_planes.h"
#include "camera.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"

typedef struct UnkStruct_ov71_02249CF0_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x4];
    s32 unk_C;
    u8 unk_10[0x14];
} UnkStruct_ov71_02249CF0_Heap_Alloc;

void ov71_02249C90(void *arg0);

s32 ov71_02247384(void *);
void *ov71_02249CF0(void *arg0);
s32 ov71_0224A080();
void ov71_02249D14(void *arg0);

void ov71_02249C90(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0xC) = 1;
}

struct SysTask;

struct SysTaskQueue;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

void ov71_0224781C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void SysTask_Destroy(SysTask *task);

void ov71_02249C98(SysTask *task, void *param);

void ov71_02249C98(SysTask *task, void *param) {
    TradeBallFlybyPhase *tbfPhase = param;

    if (tbfPhase->screenSwapPending) {
        CameraAngle cameraAngle;

        tbfPhase->screenSwapPending = 0;

        cameraAngle.x = 4432;
        cameraAngle.y = 0;
        cameraAngle.z = 0;

        BgSetPosTextAndCommit(tbfPhase->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(tbfPhase->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 256);

        ov71_0224781C(tbfPhase->scene, &cameraAngle);
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

        SysTask_Destroy(task);
    }
}

void *ov71_02249CF0(void *arg0) {
    UnkStruct_ov71_02249CF0_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_02249CF0_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_4 = 0;
        v0->unk_C = ov71_02247384(arg0);
    }
    return v0;
}

void ov71_02249D14(void *ptr) {
    if (ptr != NULL) {
        ov71_0224A080();
        Heap_Free(ptr);
    }
}
