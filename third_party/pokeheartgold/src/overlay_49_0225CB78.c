#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "heap.h"
#include "overlay_49.h"

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

extern u32 ov49_02269A6C;

UnkStruct_ov70_02260AD4 *ov49_0225CB78(enum HeapID param_1);

void ov49_0225CBDC(UnkStruct_ov70_02260AD4 *arg0);
s32 ov49_02259154(void *, void *);
void ov49_0225CBF4(UnkStruct_ov70_02260AD4 *arg0);
void ov49_0225CC20(UnkStruct_ov70_02260AD4 *arg0, s32 arg1, s32 arg2, s32 arg3);
void ov49_0225CC40(UnkStruct_ov70_02260AD4 *arg0, void *arg1);
void ov49_0225CC44(UnkStruct_ov70_02260AD4 *arg0);

UnkStruct_ov70_02260AD4 *ov49_0225CB78(enum HeapID param_1) {
    UnkStruct_ov70_02260AD4 *puVar1;
    Camera *pCVar2;
    u32 iVar3;
    u8 *puVar4;

    puVar1 = Heap_Alloc(param_1, sizeof(UnkStruct_ov70_02260AD4));
    puVar4 = (u8 *)puVar1;
    iVar3 = sizeof(UnkStruct_ov70_02260AD4);
    do {
        *puVar4 = 0;
        puVar4++;
        iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    pCVar2 = Camera_New(param_1);
    puVar1->camera = pCVar2;
    Camera_Init_FromTargetDistanceAndAngle(&puVar1->pos, 0x29aec1, (CameraAngle *)&ov49_02269A6C, 0x5c1, 0, 1, puVar1->camera);
    Camera_SetStaticPtr(puVar1->camera);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(150), FX32_CONST(900), puVar1->camera);
    return puVar1;
}

void ov49_0225CBDC(UnkStruct_ov70_02260AD4 *ptr) {
    Camera_UnsetStaticPtr();
    Camera_Delete(ptr->camera);
    Heap_Free(ptr);
}

void ov49_0225CBF4(UnkStruct_ov70_02260AD4 *arg0) {
    void *v0;

    v0 = arg0->unk_4;
    if (v0 != 0) {
        ov49_02259154(v0, &arg0->pos);
        arg0->pos.x = arg0->pos.x + 0x8000;
        arg0->pos.z = arg0->pos.z - 0x20000;
    }
    Camera_PushLookAtToNNSGlb();
}

void ov49_0225CC20(UnkStruct_ov70_02260AD4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->pos.x = arg1;
    arg0->pos.y = arg2;
    arg0->pos.z = arg3;
}

void ov49_0225CC28(UnkStruct_ov70_02260AD4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->pos.x = arg1 + 0x8000;
    arg0->pos.y = arg2;
    arg0->pos.z = arg3 - 0x20000;
}

void ov49_0225CC40(UnkStruct_ov70_02260AD4 *arg0, void *arg1) {
    arg0->unk_4 = arg1;
}

void ov49_0225CC44(UnkStruct_ov70_02260AD4 *arg0) {
    arg0->unk_4 = 0;
}
