#include "global.h"

void ov07_022318A0(void *param0);

typedef struct UnkStruct_ov07_022318A0 {
    int unk_00[6];
} UnkStruct_ov07_022318A0;

void ov07_0223172C(void *param0, UnkStruct_ov07_022318A0 *param1);

void ov07_02231514(UnkStruct_ov07_022318A0 *param0, s16 param1, s16 param2);

void ov07_0223151C(UnkStruct_ov07_022318A0 *param0, s16 param1, s16 param2);

void ov07_0223174C(void *param0, UnkStruct_ov07_022318A0 *param1);

void ov07_022318A0(void *param0) {
    UnkStruct_ov07_022318A0 v0;

    ov07_0223172C(param0, &v0);
    ov07_02231514(&v0, 0, -3);
    ov07_0223151C(&v0, 12, 4);
    ov07_0223174C(param0, &v0);
}
