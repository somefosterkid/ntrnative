#include "global.h"
#include "sys_task.h"

#include "overlay_90.h"
#include "vram_transfer_manager.h"
#include "gf_gfx_loader.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"

void ov90_022590C0(UnkStruct_ov114_0225CEF0 *param0);

void ov90_0225917C(UnkStruct_ov114_0225CFCC *param0);

void ov90_0225A198(UnkStruct_ov114_0225DFFC *param0);

void ov90_0225B254(UnkStruct_ov114_0225E854_t *arg0, NARC *arg1, enum HeapID arg2);
void ov90_0225B954(UnkStruct_ov114_0225E854_t *arg0);

void ov90_0225B230(SysTask *task, void *data) {
    UnkStruct_ov114_0225E854_t *param_2 = data;

    GF_RunVramTransferTasks();
    ov90_022590C0(&param_2->unk_4C);
    ov90_0225917C(&param_2->unk_A0);
    ov90_0225A198(&param_2->unk_3A0);
    return;
}

void ov90_0225B254(UnkStruct_ov114_0225E854_t *arg0, NARC *narc, enum HeapID heapID) {
    arg0->unk_6B8 = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &arg0->unk_6BC, heapID);
}

void ov90_0225B274(UnkStruct_ov114_0225E854_t *arg0) {
    s32 v1;
    u8 v0;

    arg0->unk_6C2 = 1;
    v0 = arg0->unk_0C.unk_08;
    v1 = 0;
    if ((s32)v0 > 0) {
loop_1:
        if ((arg0->unk_1C.unk_10[v1] == 0) && (arg0->unk_0C.unk_09 == v1)) {
            ov90_0225B954(arg0);
            return;
        }
        v1 += 1;
        if (!(v1 >= (s32)v0)) {
            goto loop_1;
        }
    }
}
