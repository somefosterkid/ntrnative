#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "filesystem.h"
#include "pm_string.h"
#include "pokepic.h"
#include "overlay_71_trade3dmodel.h"

typedef struct UnkStruct_ov71_0224B138_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x8];
    PokepicManager *unk_10;
    s32 unk_14;
    u8 unk_18[0x40];
    struct BgConfig *unk_58;
    u8 unk_5C[0x10];
    String *unk_6C;
    String *unk_70;
    s32 unk_74;
    u8 unk_78[0x4];
    s32 unk_7C;
    s32 unk_80;
    NARC *unk_84;
} UnkStruct_ov71_0224B138_Heap_Alloc;

struct Trade3DModel;

typedef struct ModelSlideState {
    SysTask **taskHandle;
    int framesRemaining;
    Trade3DModel *model;
    VecFx32 position;
    fx32 yStep;
} ModelSlideState;

void ov71_0224B11C(SysTask *task);

struct BgConfig *ov71_02247384(void *);
void *ov71_0224B138(void *arg0);

void ov71_0224B11C(SysTask *task) {
    if (task) {
        ModelSlideState *msState = SysTask_GetData(task);

        *(msState->taskHandle) = NULL;

        Heap_Free(msState);
        SysTask_Destroy(task);
    }
}

void *ov71_0224B138(void *arg0) {
    UnkStruct_ov71_0224B138_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_0224B138_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_4 = 0;
        v0->unk_58 = ov71_02247384(arg0);
        v0->unk_10 = PokepicManager_Create(HEAP_ID_57);
        v0->unk_14 = 0;
        v0->unk_6C = String_New(0x12CU, HEAP_ID_57);
        v0->unk_70 = String_New(0x12CU, HEAP_ID_57);
        v0->unk_7C = 0;
        v0->unk_80 = 0;
        v0->unk_74 = 0;
        v0->unk_84 = NARC_New(NARC_a_1_8_0, HEAP_ID_57);
    }
    return v0;
}
