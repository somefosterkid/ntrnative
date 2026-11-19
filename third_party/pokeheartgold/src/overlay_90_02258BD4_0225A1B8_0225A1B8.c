#include "global.h"

#include "error_handling.h"
#include "unk_02034354.h"
#include "bg_window.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"

struct BgConfig;

struct BufferManager;


const void *sub_02014A60(const BufferManager *bufferManager);

void ov90_0225A1B8(void *param0);

void ov90_0225A1B8(void *param0) {
    UnkStruct_ov114_0225DFFC *v0 = param0;
    int v1;
    const s16 *v2;

    v1 = GX_GetVCount();

    if (v1 >= 192) {
        return;
    }

    v2 = sub_02014A60(v0->bufferManager);

    if (GX_IsHBlank()) {
        BgSetPosTextAndCommit(v0->unk_00->unk_00, 0, BG_POS_OP_SET_X, v2[v1]);
        BgSetPosTextAndCommit(v0->unk_00->unk_00, 1, BG_POS_OP_SET_X, v2[v1]);
    }
}

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

PlayerProfile *ov90_022588CC(const UnkStruct_ov114_0225C76C *param0, u32 param1);

void ov90_0225A204(UnkStruct_ov114_0225E0F8 *param0, const UnkStruct_ov114_0225C76C *param1);

void ov90_0225A204(UnkStruct_ov114_0225E0F8 *param0, const UnkStruct_ov114_0225C76C *param1) {
    int v0;

    for (v0 = 0; v0 < param1->unk_08; v0++) {
        param0->unk_08[v0] = ov90_022588CC(param1, param1->unk_04[v0]);
        GF_ASSERT(param0->unk_08[v0]);
        param0->unk_00[v0] = sub_020348A8(param1->unk_04[v0]);
        param0->unk_04[v0] = sub_020348CC(param1->unk_04[v0]);
    }
}

BOOL ov90_0225A258(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225E0F8 *param1);

BOOL ov90_0225A258(const UnkStruct_ov114_0225C76C *param0, const UnkStruct_ov114_0225E0F8 *param1) {
    int v0;
    BOOL v1 = 0;

    for (v0 = 0; v0 < param0->unk_08 - 1; v0++) {
        if ((param1->unk_00[v0] != param1->unk_00[v0 + 1]) || (param1->unk_00[v0] == 0) || (param1->unk_00[v0 + 1] == 0)) {
            v1 = 1;
        }
    }

    return v1;
}

u32 ov90_0225A28C(u32 param0);

u32 ov90_0225A28C(u32 param0) {
    u32 v0;
    u32 v1;
    u32 v2;

    v1 = param0 % 3;
    v0 = param0 / 3;

    if ((v1 + 1) < 3) {
        v1++;
    }

    v2 = v0 * 3;
    v2 += v1;

    return v2;
}
