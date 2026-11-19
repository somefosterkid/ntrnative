#include "global.h"

struct UnkStruct_ov115_0226095C_t;

typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;

void ov91_0225CAE0(UnkStruct_ov115_0226095C *param0, u32 param1);

void ov91_02261BC8(int param0, int param1, void *param2, void *param3);

void ov91_02261BC8(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov115_0226095C *v0 = param3;
    u32 v1 = *((u32 *)param2);

    if (param0 == 0) {
        ov91_0225CAE0(v0, v1);
    }
}
