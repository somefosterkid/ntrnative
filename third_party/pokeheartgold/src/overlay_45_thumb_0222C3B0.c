#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "error_handling.h"
#include "overlay_45_wifi_plaza_system.h"

extern const s32 ov45_02254A3C[9];

void ov45_0222C3B0(UnkStruct_ov66_022302B0 *param0);

u32 ov45_0222C4B4(const UnkStruct_ov66_022302B0 *param0, u32 param1);
int ov45_0222C4E4(const UnkStruct_ov66_022302B0 *param0, u32 param1);
u32 ov45_0222C5B4(const UnkStruct_ov66_022302B0 *param0, u32 param1);
BOOL ov45_0222C61C(const UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
u32 ov45_0222C408(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2);
static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2);
void ov45_0222C480(UnkStruct_ov66_022302B0 *param0, u32 param1);

void ov45_0222C3B0(UnkStruct_ov66_022302B0 *param0) {
    int v0;

    if (param0->unk_00 == 0) {
        return;
    }

    for (v0 = 0; v0 < 9; v0++) {
        if (param0->unk_04 == ov45_02254A3C[v0]) {
            param0->unk_2C[v0] = 1;
        }

        if (param0->unk_2C[v0] == 1) {
            if ((param0->unk_08[v0] + 1) <= (40 * 30)) {
                param0->unk_08[v0]++;
            } else {
                param0->unk_2C[v0] = 2;
            }
        }
    }

    if ((param0->unk_04 + 1) <= (144 * 30)) {
        param0->unk_04++;
    }
}

u32 ov45_0222C408(UnkStruct_ov66_022302B0 *param0, u32 param1, int param2) {
    u32 v0;
    int v1, v2;
    BOOL v3;
    u32 v4;

    v0 = ov45_0222C5B4(param0, param1);

    if (v0 == 0xffffffff) {
        for (v1 = 0; v1 < 9; v1++) {
            if (ov45_0222C4E4(param0, v1) != 2) {
                for (v2 = 0; v2 < 3; v2++) {
                    v0 = (v1 * 3) + v2;
                    v3 = ov45_0222C61C(param0, v0, param2);

                    if (v3 == 0) {
                        v4 = ov45_0222C4B4(param0, v0);

                        if (v4 == 0xffffffff) {
                            param0->unk_50[v1][v2] = param1;

                            return v0;
                        }
                    }
                }
            }
        }
    }

    return 0xffffffff;
}

static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2) {
    *param1 = param0 / 3;
    *param2 = param0 % 3;
}

void ov45_0222C480(UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1, v2;

    v0 = ov45_0222C5B4(param0, param1);

    if (v0 != 0xffffffff) {
        inline_ov66_0222EC04(v0, &v1, &v2);
        param0->unk_50[v1][v2] = 0xffffffff;
    }
}

u32 ov45_0222C4B4(const UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);

    return param0->unk_50[v0][v1];
}
