#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "save.h"
#include "easy_chat.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222C8C8(UnkStruct_ov66_022307D4 *param0, u32 param1, u32 param2);

u32 ov45_0222C900(const UnkStruct_ov66_022307D4 *param0);
void ov45_0222C944(UnkStruct_ov66_02232068 *param0, u32 param1, BOOL param2);
BOOL ov45_0222C95C(const UnkStruct_ov66_02232068 *param0, u32 param1);

void ov45_0222C978(void *arg0, enum HeapID arg1);

void ov45_0222C8C8(UnkStruct_ov66_022307D4 *param0, u32 param1, u32 param2) {
    u32 v0;
    u8 *v1;

    if (param2 >= 0x14U) {
        GF_AssertFail();
    }
    if (param1 >= 0x1BU) {
        GF_AssertFail();
    }
    v1 = param0->unk_0C;
    if (v1[param2] == 0) {
        v0 = param1 / 3U;
        param0->unk_00[v0] = (u8)((s32)param0->unk_00[v0] >> 1);
        v1[param2] = 1;
    }
}

u32 ov45_0222C900(const UnkStruct_ov66_022307D4 *param0) {
    int v0;
    u32 v1;
    u32 v2;
    u32 v3;

    v1 = 0;

    for (v0 = 0; v0 < (27 / 3); v0++) {
        v1 += param0->unk_00[v0];
    }

    v3 = MTRandom() % v1;
    v2 = 0;

    for (v0 = 0; v0 < (27 / 3); v0++) {
        if ((v2 <= v3) && ((v2 + param0->unk_00[v0]) > v3)) {
            return v0 * 3;
        }

        v2 += param0->unk_00[v0];
    }

    GF_ASSERT(FALSE);
    return 0;
}

void ov45_0222C944(UnkStruct_ov66_02232068 *param0, u32 param1, BOOL param2) {
    GF_ASSERT(param1 < 20);

    if (param1 < 20) {
        param0->unk_00[param1] = param2;
    }
}

BOOL ov45_0222C95C(const UnkStruct_ov66_02232068 *param0, u32 param1) {
    GF_ASSERT(param1 < 20);

    if (param1 < 20) {
        return param0->unk_00[param1];
    }

    return 0;
}

void ov45_0222C978(void *dest, enum HeapID heapID) {
    MI_CpuFill8(dest, 0U, 0xB8U);
    *(WallpaperPasswordBank **)((u8 *)dest + 0xB4) = WallpaperPasswordBank_Create(heapID);
}
