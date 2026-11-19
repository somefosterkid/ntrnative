#include "global.h"
struct UnkStruct_ov66_02230A6C;

#include "error_handling.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_02230a6c.h"

s32 ov45_0222CCB8(void *);

u8 ov45_0222CC50(void *arg0, u32 arg1);

u8 ov45_0222CC50(void *arg0, u32 arg1) {
    s32 v0;

    if (arg1 >= 0xCU) {
        GF_AssertFail();
    }
    ov45_0222CCB8(arg0);
    v0 = *(u8 *)((u8 *)arg0 + 0x42) - (arg1 + 1);
    if (v0 < 0) {
        v0 += 0xD;
    }
    return *(u8 *)((u8 *)(arg0 + v0) + 0x34);
}

s32 ov45_0222CC7C(void *arg0, u32 arg1);

s32 ov45_0222CC7C(void *arg0, u32 arg1) {
    s32 v0;

    if (arg1 >= 0xCU) {
        GF_AssertFail();
    }
    ov45_0222CCB8(arg0);
    v0 = *(u8 *)((u8 *)arg0 + 0x42) - (arg1 + 1);
    if (v0 < 0) {
        v0 += 0xD;
    }
    return *(s32 *)(arg0 + (v0 * 4));
}

void ov45_0222CCA4(struct UnkStruct_ov66_02230A6C *data);

void ov45_0222CCA4(UnkStruct_ov66_02230A6C *data) {
    data->unk_48 = SaveArray_CalcCRC16(data->saveData, data, 0x44U);
}
