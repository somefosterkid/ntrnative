#include "global.h"
struct Ov27Task;

#include "error_handling.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225C170(Ov27Task *arg0, s32 arg1);

s32 ov27_0225C1AC(Ov27Task *arg0, s32 arg1);

s32 ov27_0225C170(Ov27Task *task, s32 idx) {
    s32 i;
    s32 count;

    count = -1;
    if ((u32)(idx - 7) <= 1) {
        return idx;
    }
    for (i = 0; i < idx + 1; i++) {
        if (task->unk_470[i].unk_0 != 0) {
            count++;
        }
    }
    if (count == -1) {
        GF_AssertFail();
    }
    return count;
}

s32 ov27_0225C1AC(Ov27Task *task, s32 pos) {
    s32 i;
    s32 result;
    s32 n;

    result = -1;
    n = 0;
    for (i = 0; i < 7; i++) {
        if (task->unk_470[i].unk_0 != 0) {
            if (pos == n) {
                result = i;
                break;
            }
            n++;
        }
    }
    if (result == -1) {
        result = 0;
        task->fieldSystem->unkD3 = 0;
    }
    return result;
}
