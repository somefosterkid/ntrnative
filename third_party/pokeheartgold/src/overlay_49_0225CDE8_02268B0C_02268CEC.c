#include "global.h"

#include "overlay_49.h"
#include "unk_struct_ov49_02268db0.h"

s32 ov49_0225E624(void *, s32);

s32 ov49_0225E82C(void *, s32, s32);

s32 ov49_02268DB0(UnkStruct_ov49_02268DB0 *arg0);

void ov49_02268CEC(UnkStruct_ov49_02268DB0 *arg0) {
    ov49_02268DB0(arg0);
    ov49_0225E82C(arg0->unk_4, 0, 0x1000);
    ov49_0225E624(arg0->unk_4, 0);
}
