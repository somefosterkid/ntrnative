#include "global.h"

#include "error_handling.h"
#include "unk_0203BA5C.h"

s32 SpawnIdToTableIndex(s32 arg0) {
    s32 v0;

    v0 = arg0;
    if ((v0 <= 0) || ((u32)v0 > 0x1EU)) {
        GF_AssertFail();
        v0 = 1;
    }
    return v0 - 1;
}
