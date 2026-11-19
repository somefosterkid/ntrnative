#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_manager.h"
#include "save.h"

typedef struct UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData {
    u8 unk_0[0xC];
    SaveData *unk_C;
    u8 unk_10[0x4];
    UnkStruct_0208C06C *unk_14;
    struct UnkStruct_0208BA84 unk_18;
    void *unk_28;
} UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData;

UnkStruct_0208C06C *sub_02087A78(OverlayManager *param_1);

UnkStruct_0208C06C *sub_02087A78(OverlayManager *param_1) {
    UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData *args;

    args = OverlayManager_GetArgs(param_1);
    return args->unk_14;
}

void sub_02087A84(u32 *param_1, u32 param_2, u32 param_3);

void sub_02087A84(u32 *param_1, u32 param_2, u32 param_3) {
    *param_1 = param_2;
    param_1[1] = param_3;
    return;
}

struct UnkStruct_020F3DCC_t *sub_02087FF8(UnkStruct_0208C06C *, u32);

static void sub_02087A8C(OverlayManager *param_1, u32 param_2);

static void sub_02087A8C(OverlayManager *param_1, u32 param_2) {
    UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData *dest;
    void *pvVar1;
    struct UnkStruct_020F3DCC_t *uVar2;
    UnkStruct_0208C06C *puVar3;

    Heap_Create(HEAP_ID_3, HEAP_ID_126, 0x10000);
    dest = OverlayManager_CreateAndGetData(param_1, sizeof(UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData), HEAP_ID_126);
    MI_CpuFill8(dest, 0, sizeof(UnkStruct_sub_02087A8C_OverlayManager_CreateAndGetData));
    pvVar1 = OverlayManager_GetArgs(param_1);
    dest->unk_28 = pvVar1;
    dest->unk_C = *(SaveData **)((u8 *)pvVar1 + 0xc);
    dest->unk_14 = Heap_Alloc(HEAP_ID_126, sizeof(UnkStruct_0208C06C));
    MI_CpuFill8(dest->unk_14, 0, sizeof(UnkStruct_0208C06C));
    dest->unk_14->unk_868 = &dest->unk_18;
    dest->unk_14->saveData = dest->unk_C;
    dest->unk_14->unk_00 = param_2;
    puVar3 = dest->unk_14;
    uVar2 = sub_02087FF8(puVar3, puVar3->unk_00);
    puVar3->unk_81C[puVar3->unk_534.unk_1A4] = uVar2;
    return;
}

u32 sub_02087B10(OverlayManager *param_1);

u32 sub_02087B10(OverlayManager *param_1) {
    sub_02087A8C(param_1, 0);
    return 1;
}

u32 sub_02087B1C(OverlayManager *param_1);

u32 sub_02087B1C(OverlayManager *param_1) {
    sub_02087A8C(param_1, 1);
    return 1;
}

u32 sub_02087B28(OverlayManager *param_1);

u32 sub_02087B28(OverlayManager *param_1) {
    sub_02087A8C(param_1, 2);
    return 1;
}

u32 sub_02087B34(OverlayManager *param_1);

u32 sub_02087B34(OverlayManager *param_1) {
    sub_02087A8C(param_1, 3);
    return 1;
}

u32 sub_02087B40(OverlayManager *param_1);

u32 sub_02087B40(OverlayManager *param_1) {
    sub_02087A8C(param_1, 4);
    return 1;
}

u32 sub_02087B4C(OverlayManager *param_1);

u32 sub_02087B4C(OverlayManager *param_1) {
    sub_02087A8C(param_1, 5);
    return 1;
}

u32 sub_02087B58(OverlayManager *param_1);

u32 sub_02087B58(OverlayManager *param_1) {
    sub_02087A8C(param_1, 6);
    return 1;
}
