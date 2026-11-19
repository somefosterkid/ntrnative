#include "global.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;

typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
} UnkStruct_ov22_022599A0;

void *SysTask_GetData(SysTask *task);

SysTask *CreateSysTaskAndEnvironment(SysTaskFunc callback, int paramSize, u32 priority, enum HeapID heapID);

typedef struct {
    UnkStruct_ov22_022597BC *unk_00;
    UnkStruct_ov22_022599A0 unk_04;
    BOOL *unk_34;
    int unk_38;
    int unk_3C;
    int unk_40;
    int unk_44;
    int unk_48;
} UnkStruct_ov22_022598F4;

void ov41_02249E60(UnkStruct_ov22_022599A0 *param0, int *param1, int *param2);

void ov41_02249F0C(SysTask *param0, void *param1);

void ov41_02249FFC(enum NarcId narcID, u32 param1, BgConfig *param2, u32 param3, u32 param4, enum HeapID heapID);

void ov22_02259B8C(enum NarcId narcID, u32 param1, int param2, u32 param3, u32 param4, enum HeapID heapID);

void ov41_02249DB4(UnkStruct_ov22_022597BC *param0, UnkStruct_ov22_022599A0 *param1, int param2, int param3, int param4, BOOL *param5);

void ov41_02249DB4(UnkStruct_ov22_022597BC *param0, UnkStruct_ov22_022599A0 *param1, int param2, int param3, int param4, BOOL *param5) {
    SysTask *v1 = CreateSysTaskAndEnvironment(ov41_02249F0C, sizeof(UnkStruct_ov22_022598F4), 0, (enum HeapID)(13));
    UnkStruct_ov22_022598F4 *v0 = SysTask_GetData(v1);

    v0->unk_00 = param0;
    v0->unk_04 = *param1;
    v0->unk_34 = param5;
    v0->unk_38 = param4;
    v0->unk_3C = param2 / param4;
    v0->unk_40 = param3 / param4;
    v0->unk_44 = Bg_GetXpos(param0->unk_00, (enum GFBgLayer)(param0->unk_1C));
    v0->unk_48 = Bg_GetYpos(param0->unk_00, (enum GFBgLayer)(param0->unk_1C));

    param1->unk_28 = 128;
    param1->unk_24 = 0x5;
    param1->unk_14 -= param2;
    param1->unk_18 -= param3;
    param1->heapID = HEAP_ID_14;

    ov41_02249E60(param1, 0L, 0L);
}
