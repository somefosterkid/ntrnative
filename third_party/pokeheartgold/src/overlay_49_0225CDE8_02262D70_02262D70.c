#include "global.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

#include "error_handling.h"

struct UnkStruct_ov70_0225DEE8_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov49_0225A204(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, u32 param3, u8 param4);

void ov49_02262D70(UnkStruct_ov70_02266C68 *param0, UnkStruct_ov70_0225DEE8 *param1, u32 param2, u32 param3, s32 param4) {
    static const u8 v0[2] = {
        8,
        10,
    };

    GF_ASSERT(param3 < 2);

    memset(param0, 0, sizeof(UnkStruct_ov70_02266C68));

    param0->unk_06 = param3;
    param0->unk_03 = param2;

    ov49_02262DD4(param0, param4);
    ov49_0225A204(param1, 18, 3, 13, v0[param3]);
}
