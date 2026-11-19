#include "global.h"

#include "overlay_01_021FD1B8.h"

void ov01_021FD20C(UnkStruct_021F1118 *param0);

void ov01_021FD21C(UnkStruct_021F114C *param0);

UnkStruct_021F1118 *ov01_021FD244(UnkStruct_021F114C *param0, u32 param1);

void ov01_021FD21C(UnkStruct_021F114C *param0) {
    u32 v0 = 0, v1 = param0->unk_04;
    UnkStruct_021F1118 *v2 = param0->unk_08;

    while (v0 < v1) {
        if (v2->unk_00 != 0xffff) {
            ov01_021FD20C(v2);
        }

        v2++;
        v0++;
    }
}

UnkStruct_021F1118 *ov01_021FD244(UnkStruct_021F114C *param0, u32 param1) {
    u32 v0 = param0->unk_04;
    UnkStruct_021F1118 *v1 = param0->unk_08;

    do {
        if (v1->unk_00 == param1) {
            return v1;
        }

        v1++;
        v0--;
    } while (v0);

    return 0L;
}
