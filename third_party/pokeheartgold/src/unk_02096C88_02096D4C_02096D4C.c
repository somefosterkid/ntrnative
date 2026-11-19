#include "global.h"
#include "unk_struct_0209_bdf8.h"
struct UnkStruct_0209BDF8;

s32 sub_02096D14(void *, s32, void *, s32);

s32 sub_02096D4C(struct UnkStruct_0209BDF8 *arg0, s32 arg1, void *arg2, s32 arg3);

s32 sub_02096D4C(UnkStruct_0209BDF8 *arg0, s32 arg1, void *arg2, s32 arg3) {
    if (*((s32 *)&arg0->unk_1C) == 1) {
        return 0;
    }
    return sub_02096D14(arg0, arg1, arg2, arg3);
}
