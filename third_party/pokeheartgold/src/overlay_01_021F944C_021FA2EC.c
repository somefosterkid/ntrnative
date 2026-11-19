#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "constants/heap.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "sys_task.h"

struct UnkStruct_ov5_021ED0A4_t;

BillboardList *sub_020237EC(const BillboardListParams *params);




void ov01_021FA2EC(UnkStruct_ov5_021ED0A4 *param0, int param1);

BOOL sub_02023874(BillboardList *list);
BOOL ov01_021FA314(BillboardList *list);

void ov01_021FA2EC(UnkStruct_ov5_021ED0A4 *param0, int param1) {
    BillboardListParams v0;
    BillboardList *v1;

    v0.maxElements = param1;
    v0.heapID = HEAP_ID_FIELD1;

    v1 = sub_020237EC(&v0);
    ov01_021FA1C8(param0, v1);
    ov01_021FA1D8(param0, param1);
}

BOOL ov01_021FA314(BillboardList *list) {
    return sub_02023874(list);
}
