#include "global.h"

#include "unk_02005D10.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02242508(s32 arg0, s32 arg1);

s32 ov70_02242508(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return 1;
    }
    return (s32)(arg0 + (arg1 - 1)) / arg1;
}

s32 ov70_02242574(void *, s32);

s32 ov70_0224251C(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

s32 ov70_0224251C(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    s32 v0;

    switch (arg1) {
    case 4:
        return -2;
    case 5:
        ov70_02242574(arg0, -1);
        return -1;
    case 6:
        ov70_02242574(arg0, 1);
        return -1;
    default:
        v0 = arg1 + (arg0->unk_5A * 4);
        if (v0 < (s32) arg0->unk_5C) {
            PlaySE(0x5DCU);
            return arg0->unk_34[v0].value;
        }
        return -1;
    }
}
