#include "global.h"

#include "error_handling.h"
#include "unk_020632B0.h"

s32 sub_0206460C(SysTask *);

BOOL sub_02064598(SysTask *task) {
    if (task == NULL) {
        GF_AssertFail();
    }
    return sub_0206460C(task);
}
