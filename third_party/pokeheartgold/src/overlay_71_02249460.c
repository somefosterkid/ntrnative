#include "global.h"

#include "sys_task.h"
#include "heap.h"
#include "system.h"
#include "bg_window.h"

typedef struct UnkStruct_ov71_022494A0_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x1C];
    struct BgConfig *unk_24;
    s32 unk_28;
    u8 unk_2C[0x8];
    u8 unk_34;
    u8 unk_35[0x6F];
} UnkStruct_ov71_022494A0_Heap_Alloc;

struct Trade3DModel;

struct Trade3DScene;

typedef struct Trade3DModel Trade3DModel;

typedef struct Trade3DScene Trade3DScene;

void ov71_022476C4(Trade3DModel *tradeModel, const VecFx32 *position);

void ov71_022476EC(Trade3DModel *tradeModel, const VecFx16 *rotation);

BOOL ov71_02247738(Trade3DModel *tradeModel);

typedef struct AxisRotAnimator {
    fx16 currentValue;
    fx16 step;
    fx16 targetValue;
    u16 framesRemaining;
} AxisRotAnimator;

typedef struct BallPathState {
    Trade3DModel *model;
    Trade3DScene *scene;
    VecFx32 orbitCenter;
    VecFx32 currentPos;
    VecFx16 rotation;
    u8 pad_26[2];
    fx32 currentY;
    fx32 yStep;
    fx32 yAccel;
    fx32 unused_34;
    fx32 orbitRadiusX;
    fx32 orbitRadiusZ;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusZStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusZ;
    u32 orbitTransitionFrames;
    s16 angleAccum;
    s16 rotationSpeed;
    s16 targetRotationSpeed;
    s16 rotationSpeedStep;
    AxisRotAnimator axisRotAnimators[3];
    u32 rotSpeedTransitionFrames;
    BOOL exitedView;
    BOOL midpointReached;
    int entryFramesRemaining;
} BallPathState;

void ov71_0224903C(BallPathState *bpState);

void ov71_02249460(SysTask *unused, void *param);

struct BgConfig *ov71_02247384(void *);
s32 ov71_02249988(void *);
void *ov71_022494A0(void *arg0);
s32 ov71_022497C8(void *);
s32 ov71_022498E0(void *);
s32 ov71_02249970(void *);
s32 ov71_02249990(void *);
void ov71_022494D0(void *arg0);

void ov71_02249460(SysTask *unused, void *param) {
    BallPathState *bpState = param;

    if (bpState->exitedView == 0) {
        ov71_0224903C(bpState);

        bpState->currentY += bpState->yStep;
        bpState->currentPos.y = bpState->currentY;

        ov71_022476C4(bpState->model, &(bpState->currentPos));
        ov71_022476EC(bpState->model, &(bpState->rotation));

        if (ov71_02247738(bpState->model) == 0) {
            bpState->exitedView = 1;
        }
    }
}

void *ov71_022494A0(void *arg0) {
    UnkStruct_ov71_022494A0_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_022494A0_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_4 = 0;
        v0->unk_24 = ov71_02247384(arg0);
        v0->unk_28 = 0;
        ov71_02249988((void *)&v0->unk_34);
    }
    return v0;
}

void ov71_022494D0(void *ptr) {
    if (ptr != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        ov71_022497C8(ptr);
        ov71_022498E0(ptr);
        ov71_02249970(ptr);
        ov71_02249990(ptr + 0x34);
        Heap_Free(ptr);
    }
}
