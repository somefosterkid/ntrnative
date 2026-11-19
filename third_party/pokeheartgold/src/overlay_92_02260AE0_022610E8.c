#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "overlay_92_unk_struct_ov116_02264fec.h"

void ov92_02260AE0(SysTask *task, void *data);

void ov92_022610E8(UnkStruct_ov116_02262A8C *arg0, BOOL *arg1);

void ov92_022610E8(UnkStruct_ov116_02262A8C *arg0, BOOL *arg1) {
    UnkStruct_ov116_02264FEC *v0;

    v0 = Heap_Alloc(HEAP_ID_113, sizeof(UnkStruct_ov116_02264FEC));
    v0->unk_00 = arg0;
    v0->unk_04 = 0;
    v0->unk_08 = 0;
    v0->unk_10 = arg1;
    SysTask_CreateOnMainQueue(ov92_02260AE0, v0, 0x1000U);
}
