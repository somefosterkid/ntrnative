#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"

void ov45_0222CCA4(UnkStruct_ov66_02230A6C *param0);

void ov45_0222CCB8(UnkStruct_ov66_02230A6C *param0);

BOOL ov45_0222CBD0(UnkStruct_ov66_02230A6C *param0, s32 param1, s32 param2);

BOOL ov45_0222CBD0(UnkStruct_ov66_02230A6C *param0, s32 param1, s32 param2) {
    int v0;
    BOOL v1 = 0;

    ov45_0222CCB8(param0);

    for (v0 = 0; v0 < (12 + 1); v0++) {
        if (param0->unk_00[v0] == param1) {
            param0->unk_00[v0] = param2;
            v1 = 1;
        }
    }

    ov45_0222CCA4(param0);

    return v1;
}

int ov45_0222CC00(UnkStruct_ov66_02230A6C *param0);

int ov45_0222CC00(UnkStruct_ov66_02230A6C *param0) {
    u8 v0;

    ov45_0222CCB8(param0);

    if (param0->unk_42 == param0->unk_41) {
        return 24;
    }

    v0 = param0->unk_34[param0->unk_41];

    param0->unk_34[param0->unk_41] = 24;
    param0->unk_00[param0->unk_42] = -1;
    param0->unk_41 = (param0->unk_41 + 1) % (12 + 1);

    ov45_0222CCA4(param0);

    return v0;
}
