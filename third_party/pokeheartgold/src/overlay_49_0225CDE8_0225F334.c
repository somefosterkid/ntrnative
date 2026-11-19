#include "global.h"

#include "math_util.h"
#include "overlay_49_unk_struct_ov70_02263270.h"

void *ov49_02258C5C(void *, s32, s32, s32);

s32 ov49_02258E7C(void *, void *, s32, s32);

void *ov49_02259FF0();

void *ov49_0225A008(void *);

s32 ov49_0225CC40(void *, void *);

void ov49_0225F334(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov49_0225F334(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void *v1;
    s32 v0;
    void *v2;
    void *v3;
    void *v4;

    v0 = arg3;
    v4 = ov49_02259FF0();
    v2 = ov49_0225A008(arg0);
    v1 = v2;
    v3 = ov49_02258C5C(v4, arg1, arg2, arg3);
    ov49_02258E7C(v4, v3, 0, arg4);
    ov49_0225CC40(v2, v3);
}

void ov49_0225F374(void *arg0);

void ov49_0225F374(void *arg0) {
    *(s32 *)arg0 = 0;
    ((UnkStruct_ov70_02263270 *)arg0)->unk_04 = 0;
    *((s8 *)&((UnkStruct_ov70_02263270 *)arg0)->unk_05) = 1;
    *((s8 *)&((UnkStruct_ov70_02263270 *)arg0)->unk_06) = (s8)(MTRandom() & 3);
    *((s8 *)&((UnkStruct_ov70_02263270 *)arg0)->unk_07) = 1;
}
