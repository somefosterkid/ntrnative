#include "global.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

void ov49_0225BF80(UnkStruct_ov70_0225FA84 *);

void ov49_0225B99C(UnkStruct_ov70_0225F350 *param_1);

s32 ov49_0225BA20(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225E4EC *);
s32 ov49_0225C970(UnkStruct_ov70_02260744 *, UnkStruct_ov70_0225E4EC *, UnkStruct_ov70_0225F208 *, const WiFiPlazaProfile *, NARC *, s32);
void ov49_0225B9AC(UnkStruct_ov70_0225F350 *arg0, UnkStruct_ov70_0225E4EC *arg1, UnkStruct_ov70_0225F208 *arg2, s32 arg3, const WiFiPlazaProfile *arg4);

void ov49_0225B99C(UnkStruct_ov70_0225F350 *param_1) {
    ov49_0225BF80(&param_1->unk_08);
    param_1->unk_01 = 0;
    return;
}

void ov49_0225B9AC(UnkStruct_ov70_0225F350 *arg0, UnkStruct_ov70_0225E4EC *arg1, UnkStruct_ov70_0225F208 *arg2, s32 arg3, const WiFiPlazaProfile *arg4) {
    u8 v0;

    v0 = arg0->unk_01;
    if ((v0 == 1) || ((u32)(u8)(v0 + 0xFD) <= 1U)) {
        ov49_0225BA20(arg0, arg1);
    }
    ov49_0225C970(&arg0->unk_114, arg1, arg2, arg4, arg0->unk_14C, arg3);
    arg0->unk_01 = 1U;
}
