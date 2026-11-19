#include "global.h"

#include "overlay_27_ov27task.h"

#include "error_handling.h"
#include "overlay_27.h"
#include "constants/heap.h"
#include "heap.h"

s32 ov27_0225C1AC(Ov27Task *param_1, s32 param_2);

void ov27_0225C1EC(Ov27Task *param_1) {
    s32 i;

    if (param_1->fieldSystem == NULL) {
        GF_AssertFail();
    }
    if (param_1->unk_470[param_1->unk_14].unk_0 == 0) {
        for (i = 0; i < 7; i++) {
            if (param_1->unk_470[i].unk_0 != 0) {
                param_1->fieldSystem->unkD3 = i;
                param_1->unk_14 = ov27_0225C1AC(param_1, param_1->fieldSystem->unkD3);
                return;
            }
        }
    }
}

void ov27_0225C238(void) {
    Heap_AllocAtEnd(HEAP_ID_3, 0x3E8U);
}

void ov27_0225C248(void) {
}

void ov27_0225C24C(void) {
}
