#include "global.h"

#include "unk_struct_ov70_02242e58.h"

extern s32 ov70_02245F5C;

s32 ov70_02242364(UnkStruct_ov70_02242E58 *arg0, s32 arg1);

s32 ov70_02242364(UnkStruct_ov70_02242E58 *arg0, s32 arg1) {
    s16 v0;
    s32 v1;

    v1 = arg1;
    if (v1 != 3) {
        v0 = arg0->unk_3C;
        if ((s32)v0 <= 8) {
            if (v1 >= (s32) * (s32 *)(&ov70_02245F5C + v0 * 2)) {
                v1 = -1;
            }
            return v1;
        }
        return v1;
    }
    return -2;
}
