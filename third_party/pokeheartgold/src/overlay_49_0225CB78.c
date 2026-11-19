#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "heap.h"
#include "overlay_49.h"

typedef struct UnkStruct_ov49_0225CBF4 {
    u8 unk_0[0x4];
    void *unk_4;
    s32 unk_8;
    u8 unk_C[0x4];
    s32 unk_10;
} UnkStruct_ov49_0225CBF4;

extern u32 ov49_02269A6C;

u32 *ov49_0225CB78(enum HeapID param_1);

void ov49_0225CBDC(Camera **arg0);
s32 ov49_02259154(void *, void *);
void ov49_0225CBF4(UnkStruct_ov49_0225CBF4 *arg0);
void ov49_0225CC20(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void ov49_0225CC40(void *arg0, void *arg1);
void ov49_0225CC44(void *arg0);

u32 *ov49_0225CB78(enum HeapID param_1) {
    u32 *puVar1;
    Camera *pCVar2;
    u32 iVar3;
    u32 *puVar4;

    puVar1 = Heap_Alloc(param_1, 0x14);
    puVar4 = puVar1;
    iVar3 = 0x14;
    do {
        *(u8 *)puVar4 = 0;
        puVar4 = (u32 *)((u8 *)puVar4 + 1);
        iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    pCVar2 = Camera_New(param_1);
    *puVar1 = (u32)(pCVar2);
    Camera_Init_FromTargetDistanceAndAngle((VecFx32 *)(puVar1 + 2), 0x29aec1, (CameraAngle *)&ov49_02269A6C, 0x5c1, 0, 1, (Camera *)*puVar1);
    Camera_SetStaticPtr((Camera *)*puVar1);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(150), FX32_CONST(900), (Camera *)*puVar1);
    return puVar1;
}

void ov49_0225CBDC(Camera **ptr) {
    Camera_UnsetStaticPtr();
    Camera_Delete(*ptr);
    Heap_Free(ptr);
}

void ov49_0225CBF4(UnkStruct_ov49_0225CBF4 *arg0) {
    void *v0;

    v0 = arg0->unk_4;
    if (v0 != 0) {
        ov49_02259154(v0, (void *)&arg0->unk_8);
        arg0->unk_8 = (s32)(arg0->unk_8 + 0x8000);
        arg0->unk_10 = (s32)(arg0->unk_10 - 0x20000);
    }
    Camera_PushLookAtToNNSGlb();
}

void ov49_0225CC20(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)((u8 *)arg0 + 8) = arg1;
    *(s32 *)((u8 *)arg0 + 0xC) = arg2;
    *(s32 *)((u8 *)arg0 + 0x10) = arg3;
}

void ov49_0225CC28(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)((u8 *)arg0 + 8) = (s32)(arg1 + 0x8000);
    *(s32 *)((u8 *)arg0 + 0xC) = arg2;
    *(s32 *)((u8 *)arg0 + 0x10) = (s32)(arg3 - 0x20000);
}

void ov49_0225CC40(void *arg0, void *arg1) {
    *(void **)((u8 *)arg0 + 4) = arg1;
}

void ov49_0225CC44(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 0;
}
