#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2);

u32 ov45_0222C5B4(const UnkStruct_ov66_022302B0 *param0, u32 param1);

static inline void inline_ov66_0222EC04(u32 param0, u32 *param1, u32 *param2) {
    *param1 = param0 / 3;
    *param2 = param0 % 3;
}

void ov45_0222C514(UnkStruct_ov66_022302B0 *param0, u32 param1);

void ov45_0222C514(UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1;
    u32 v2 = ov45_0222C5B4(param0, param1);

    if (v2 != 0xffffffff) {
        inline_ov66_0222EC04(v2, &v0, &v1);
        param0->unk_128[v0][v1] = 1;
    }
}

BOOL ov45_0222C54C(const UnkStruct_ov66_022302B0 *param0, u32 param1);

BOOL ov45_0222C54C(const UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);
    return param0->unk_128[v0][v1];
}

void ov45_0222C580(UnkStruct_ov66_022302B0 *param0, u32 param1);

void ov45_0222C580(UnkStruct_ov66_022302B0 *param0, u32 param1) {
    u32 v0;
    u32 v1;

    GF_ASSERT(param1 < 9 * 3);

    inline_ov66_0222EC04(param1, &v0, &v1);
    param0->unk_128[v0][v1] = 0;
}
