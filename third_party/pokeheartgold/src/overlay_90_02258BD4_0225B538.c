#include "global.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"

#include "overlay_90.h"
#include "unk_02035900.h"

u8 ov90_0225B538(UnkStruct_ov114_0225F270 *arg0) {
    return arg0->unk_1F;
}

void ov90_0225B53C(UnkStruct_ov114_0225F270 *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov90_0225B53C(UnkStruct_ov114_0225F270 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = 1 << arg1;
    if (!(arg0->unk_1C & v0)) {
        if (arg2 == 0) {
            arg0->unk_1D = 0U;
        }
        arg0->unk_1C = (u8)(arg0->unk_1C | v0);
    }
    v2 = 0;
    v1 = 0;
    do {
        if ((1 << v1) & arg0->unk_1C) {
            v2 += 1;
        }
        v1 += 1;
    } while (v1 < 4);
    if (v2 == arg3) {
        if (arg0->unk_1D != 0) {
            sub_02037030(0x18, NULL, 0);
            return;
        }
        sub_02037030(0x19, NULL, 0);
    }
}

void ov90_0225B594(UnkStruct_ov114_0225F270 *arg0, s8 arg1);

void ov90_0225B594(UnkStruct_ov114_0225F270 *arg0, s8 arg1) {
    arg0->unk_1E = 1;
    arg0->unk_1F = arg1;
}
