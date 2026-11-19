#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"

void ov45_0222ECB8(s64 *param0);

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02258918;

extern const UnkStruct_ov66_02258918 _02254A28[5];

void ov45_0222CAA0(UnkStruct_ov66_022309A4 *param0);

s32 ov45_0222CB3C(void *arg0);
s32 ov45_0222CB40(void *arg0);

void ov45_0222CAA0(UnkStruct_ov66_022309A4 *param0) {
    s64 v0;
    s64 v1;

    if (param0->unk_00) {
        ov45_0222ECB8(&v0);
        v1 = v0 - param0->unk_04;

        if (param0->unk_0C < (v1 * 30)) {
            param0->unk_0C = (v1 * 30);
        }

        if (param0->unk_0C < param0->unk_10) {
            param0->unk_0C++;

            {
                u32 v2;
                u32 v3;
                int v4;

                v2 = (param0->unk_0C * 256) / param0->unk_10;
                v3 = 0;

                for (v4 = 0; v4 < NELEMS(_02254A28); v4++) {
                    v3 += _02254A28[v4].unk_02;

                    if (v3 >= v2) {
                        if (param0->unk_14 != _02254A28[v4].unk_00) {
                            param0->unk_14 = _02254A28[v4].unk_00;
                            param0->unk_18 = 0;
                        }

                        break;
                    }
                }
            }
        } else {
            if (param0->unk_14 != 5) {
                param0->unk_14 = 5;
                param0->unk_18 = 0;
            }

            if (param0->unk_18 >= 120) {
                param0->unk_14 = 0;
                param0->unk_00 = 0;
            }
        }

        param0->unk_18++;
    }
}

s32 ov45_0222CB3C(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x18);
}

s32 ov45_0222CB40(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x14);
}
