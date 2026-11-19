#include "global.h"

#include "constants/heap.h"

#include "save.h"

typedef struct {
    s32 unk_00[13];
    u8 unk_34[13];
    u8 unk_41;
    u8 unk_42;
    u8 unk_43;
    const SaveData *saveData;
    u16 unk_48;
    u16 unk_4A;
} UnkStruct_ov66_02230A6C;

void ov45_0222CCA4(UnkStruct_ov66_02230A6C *param0);

void ov45_0222CB44(UnkStruct_ov66_02230A6C *param0, const SaveData *saveData);

void ov45_0222CB44(UnkStruct_ov66_02230A6C *param0, const SaveData *saveData) {
    int v0;

    for (v0 = 0; v0 < (12 + 1); v0++) {
        param0->unk_34[v0] = 24;
        param0->unk_00[v0] = -1;
    }

    param0->unk_41 = 0;
    param0->unk_42 = 0;
    param0->saveData = saveData;

    ov45_0222CCA4(param0);
}
