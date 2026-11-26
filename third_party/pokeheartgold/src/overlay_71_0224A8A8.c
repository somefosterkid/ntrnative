#include "global.h"

#include "sprite.h"
#include "unk_02026E30.h"
#include "heap.h"
#include "gf_gfx_planes.h"
#include "camera.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"
#include "overlay_71_trade_ball_arrival_phase.h"

typedef struct UnkStruct_ov71_0224A920_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x4];
    struct BgConfig *unk_C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 unk_1C[0x4];
    s32 unk_20;
    u8 unk_24[0xC];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
} UnkStruct_ov71_0224A920_Heap_Alloc;

void ov71_0224A8A8(TradeBallArrivalPhase *arg0);

struct BgConfig *ov71_02247384(void *);
void *ov71_0224A920(void *arg0);
s32 ov71_0224ADB0(void *arg0);
s32 ov71_0224AEF4(void *);
s32 ov71_0224AFB8();
s32 ov71_0224B068();
s32 ov71_0224B11C();
void ov71_0224A954(void *arg0);

void ov71_0224A8A8(TradeBallArrivalPhase *arg0) {
    arg0->screenSwapPending = 1;
}

struct SysTask;

struct SysTaskQueue;

struct Trade3DScene;

typedef struct Trade3DScene Trade3DScene;

void ov71_022474CC(Trade3DScene *tradeScene);

void ov71_0224781C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224A8B0(void *param);

void ov71_0224A8B0(void *param) {
    TradeBallArrivalPhase *arrivalPhase = param;

    if (arrivalPhase->scene) {
        G3X_Reset();
        ov71_022474CC(arrivalPhase->scene);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }

    if (arrivalPhase->screenSwapPending) {
        CameraAngle cameraAngle;

        cameraAngle.x = -0x1150;
        cameraAngle.y = 0;
        cameraAngle.z = 0;

        BgSetPosTextAndCommit(arrivalPhase->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 256);
        BgSetPosTextAndCommit(arrivalPhase->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);

        ov71_0224781C(arrivalPhase->scene, &cameraAngle);
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

        arrivalPhase->screenSwapPending = 0;
    }
}

void *ov71_0224A920(void *arg0) {
    UnkStruct_ov71_0224A920_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_0224A920_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_4 = 0;
        v0->unk_C = ov71_02247384(arg0);
        v0->unk_20 = 0;
        v0->unk_10 = 0;
        v0->unk_14 = 0;
        v0->unk_18 = 0;
        v0->unk_30 = 0;
        v0->unk_34 = 0;
        v0->unk_38 = 0;
    }
    return v0;
}

void ov71_0224A954(void *ptr) {
    if (ptr != NULL) {
        ov71_0224ADB0(ptr);
        ov71_0224AEF4(ptr);
        if (*(s32 *)((u8 *)ptr + 0x30) != 0) {
            ov71_0224B068();
        }
        if (*(s32 *)((u8 *)ptr + 0x34) != 0) {
            ov71_0224AFB8();
        }
        if (*(s32 *)((u8 *)ptr + 0x38) != 0) {
            ov71_0224B11C();
        }
        Heap_Free(ptr);
    }
}
