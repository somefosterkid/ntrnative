#include "global.h"

#include "overlay_91.h"
#include "unk_02035900.h"

void ov91_0225CA74(void *, void *, u32);

void ov91_0225CA90(void *, void *, u32);

struct UnkStruct_ov115_0226095C_t;
typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;
void ov91_0225CB50(UnkStruct_ov115_0226095C *param0);

void ov91_02261B64(u32 param_1, u32 param_2, void *param_3, void *param_4) {
    u16 uVar1;

    ov91_0225CA90(param_4, param_3, param_1);
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        ov91_0225CA74(param_4, param_3, param_1);
    }
    return;
}

s32 ov91_0225CA9C(UnkStruct_ov115_0226095C *, s32, s32);

void ov91_02261B8C(long long arg0, s32 *arg2, void *arg3) {
    ov91_0225CA9C(arg3, *arg2, arg0);
}

void ov91_0225CABC(void *, void *, u32);

void ov91_02261B9C(u32 param_1, u32 param_2, void *param_3, void *param_4) {
    ov91_0225CABC(param_4, param_3, param_1);
    return;
}

void ov91_0225CAEC(UnkStruct_ov115_0226095C *, u32, u32);

void ov91_02261BAC(u32 param_1, u32 param_2, u32 *param_3, void *param_4) {
    ov91_0225CAEC(param_4, *param_3, param_1);
    return;
}

void ov91_02261BBC(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov115_0226095C *v0 = param3;
    ov91_0225CB50(v0);
}
