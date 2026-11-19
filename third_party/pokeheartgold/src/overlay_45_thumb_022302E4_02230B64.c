#include "global.h"

#include "overlay_42.h"

typedef struct UnkStruct_ov45_02230B64 {
    u8 unk_0[0x4];
    UnkStruct_ov42_02228110 *unk_4;
    void *unk_8;
} UnkStruct_ov45_02230B64;

s32 ov45_022309C4(s32, s32);

s32 sub_02023EE0(void *, s32);

s32 sub_02023F40(void *, s32);

void ov45_02230B64(UnkStruct_ov45_02230B64 *arg0);

void ov45_02230B64(UnkStruct_ov45_02230B64 *arg0) {
    sub_02023EE0(arg0->unk_8, ov45_022309C4(1, ov42_02228188(arg0->unk_4, 6)));
    sub_02023F40(arg0->unk_8, 0);
}
