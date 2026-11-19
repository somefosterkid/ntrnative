#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "field/hblank_system_priv.h"
#include "overlay_07_bg_scroll_context.h"

struct BufferManager;

struct HBlankSystem;

struct HBlankTask;

struct UnkStruct_ov5_021EF4F8_t;

typedef struct UnkStruct_ov5_021EF4F8_t UnkStruct_ov5_021EF4F8;

void sub_02014A38(BufferManager *bufferManager);

struct UnkStruct_ov5_021EF4F8_t {
    u32 unk_00;
    int unk_04;
    int unk_08;
    u16 unk_0C[192];
    u16 unk_18C[192];
    BufferManager *bufferManager;
    HBlankSystem *unk_310;
    HBlankTask *unk_314;
    BOOL unk_318;
    SysTask *unk_31C;
    SysTask *unk_320;
    SysTask *unk_324;
};

void ov01_021FB82C(UnkStruct_ov5_021EF4F8 *param0);

UnkStruct_ov5_021EF4F8 *ov01_021FB5D4(enum HeapID heapID, HBlankSystem *param1);

BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);

UnkStruct_ov5_021EF4F8 *ov01_021FB5D4(enum HeapID heapID, HBlankSystem *param1) {
    UnkStruct_ov5_021EF4F8 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov5_021EF4F8));
    memset(v0, 0, sizeof(UnkStruct_ov5_021EF4F8));
    v0->unk_00 = 0;
    v0->bufferManager = sub_02014A08(heapID, v0->unk_0C, v0->unk_18C);
    v0->unk_310 = param1;
    return v0;
}

void ov01_021FB610(UnkStruct_ov5_021EF4F8 *param0);

void ov01_021FB610(UnkStruct_ov5_021EF4F8 *param0) {
    if (param0->unk_00 == 1) {
        ov01_021FB82C(param0);
    }

    sub_02014A38(param0->bufferManager);
    Heap_Free(param0);
}

HBlankTask *ov01_021FB530(HBlankSystem *hBlankSystem, HBlankCallback callback, void *param);

static void ov01_021FB6C4(SysTask *param0, void *param1);

void ov01_021FB7CC(SysTask *param0, void *param1);

void FieldSystem_DoPoisonEffect(UnkStruct_ov5_021EF4F8 *param0);

void ov01_021FB800(SysTask *task, void *data);

void ov01_021FB7DC(struct HBlankTask *param0, void *param1);

void FieldSystem_DoPoisonEffect(UnkStruct_ov5_021EF4F8 *param0) {
    GF_ASSERT(param0->unk_00 == 0);
    param0->unk_314 = ov01_021FB530(param0->unk_310, ov01_021FB7DC, param0);
    param0->unk_31C = SysTask_CreateOnMainQueue(ov01_021FB6C4, param0, 1024);
    param0->unk_320 = SysTask_CreateOnVBlankQueue(ov01_021FB7CC, param0, 1024);
    param0->unk_324 = SysTask_CreateOnVBlankQueue(ov01_021FB800, param0, 1024);
    memset(param0->unk_0C, 0, sizeof(u16) * 192);
    memset(param0->unk_18C, 0, sizeof(u16) * 192);
    param0->unk_00 = 1;
    param0->unk_08 = 0;
    PlaySE(SEQ_SE_DP_DOKU2);
}

void *sub_02014A4C(const BufferManager *bufferManager);

void ov01_021FB788(u16 *param0, int param1, int param2);

static void ov01_021FB6C4(SysTask *param0, void *param1) {
    UnkStruct_ov5_021EF4F8 *v0 = param1;
    u16 *v1;

    switch (v0->unk_08) {
    case 0:
        v0->unk_04 = 3;
        v0->unk_08++;
        break;
    case 1:
        v0->unk_04--;
        v1 = sub_02014A4C(v0->bufferManager);

        ov01_021FB788(v1, 3 - v0->unk_04, 3);

        if (v0->unk_04 <= 0) {
            v0->unk_04 = 3;
            v0->unk_08++;
        }
        break;
    case 2:
        v0->unk_04--;
        v1 = sub_02014A4C(v0->bufferManager);

        ov01_021FB788(v1, v0->unk_04, 3);

        if (v0->unk_04 <= 0) {
            v0->unk_08++;
        }
        break;
    case 3:
        ov01_021FB82C(v0);
        G3X_SetHOffset(0);
        break;
    }
}
