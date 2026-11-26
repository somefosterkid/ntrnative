#include "global.h"

#include "error_handling.h"
#include "overlay_45_wifi_plaza_api.h"
#include "overlay_49_getters.h"
#include "overlay_49_plaza_api.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

extern const UnkStruct_ov70_0226DA18 ov49_02269BE0[];

void ov49_0225F1A8(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225F1A8(UnkStruct_ov70_0225DEE8 *param0) {
    UnkStruct_ov70_022630A4 *v2;
    u32 v1;
    UnkStruct_ov70_02263910 *v0;

    v0 = ov49_02259FEC(param0);
    v2 = ov49_0225A010(param0);
    v1 = ov49_0225A02C(param0);
    ov45_0222A5E8(ov49_02259FE8(param0), 1);
    if (v0->unk_06 >= 4U) {
        GF_AssertFail();
    }
    ov49_0225EF98(v2, v1, &ov49_02269BE0[v0->unk_06], 0);
}
