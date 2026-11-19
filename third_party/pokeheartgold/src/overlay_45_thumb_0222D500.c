#include "global.h"

#include "error_handling.h"
#include "geonet_locations.h"
#include "overlay_45_wifi_plaza_system.h"

void ov45_0222D500(UnkStruct_ov66_02231428 *param0, const s64 *param1);

u32 ov45_0222D724(u8 *arg0, u32 arg1);

void ov45_0222D500(UnkStruct_ov66_02231428 *param0, const s64 *param1) {
    RTCDate v0;
    RTCTime v1;

    RTC_ConvertSecondToDateTime(&v0, &v1, *param1);

    param0->unk_00_val2_unk_00 = v1.hour;
    param0->unk_00_val2_unk_01 = v1.minute;
    param0->unk_00_val2_unk_02 = v1.second;
}

void ov45_0222D524(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2);

void ov45_0222D524(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2) {
    u32 v0;
    UnkStruct_ov66_02231428 v1;
    UnkStruct_ov66_02231428 v2;

    v1 = *param0;
    v2 = *param1;

    v1.unk_00_val2_unk_02 += v2.unk_00_val2_unk_02;
    v0 = v1.unk_00_val2_unk_02 / 60;
    v1.unk_00_val2_unk_01 += v0;

    param2->unk_00_val2_unk_02 = v1.unk_00_val2_unk_02 % 60;

    v1.unk_00_val2_unk_01 += v2.unk_00_val2_unk_01;
    v0 = v1.unk_00_val2_unk_01 / 60;
    v1.unk_00_val2_unk_00 += v0;

    param2->unk_00_val2_unk_01 = v1.unk_00_val2_unk_01 % 60;

    v1.unk_00_val2_unk_00 += v2.unk_00_val2_unk_00;
    param2->unk_00_val2_unk_00 = v1.unk_00_val2_unk_00 % 24;
}

void ov45_0222D594(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2);

void ov45_0222D594(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2) {
    u32 v0;
    s32 v1;
    UnkStruct_ov66_02231428 v2;
    UnkStruct_ov66_02231428 v3;

    v2 = *param0;
    v3 = *param1;
    v1 = ((s8)v2.unk_00_val2_unk_02 - (s8)v3.unk_00_val2_unk_02);

    if (v1 < 0) {
        v0 = ((-v1) / 60) + 1;
        v2.unk_00_val2_unk_02 += 60 * v0;
        v2.unk_00_val2_unk_01 -= v0;
    }

    param2->unk_00_val2_unk_02 = v2.unk_00_val2_unk_02 - v3.unk_00_val2_unk_02;

    v1 = ((s8)v2.unk_00_val2_unk_01 - (s8)v3.unk_00_val2_unk_01);

    if (v1 < 0) {
        v0 = ((-v1) / 60) + 1;
        v2.unk_00_val2_unk_01 += 60 * v0;
        v2.unk_00_val2_unk_00 -= v0;
    }

    param2->unk_00_val2_unk_01 = v2.unk_00_val2_unk_01 - v3.unk_00_val2_unk_01;

    v1 = ((s8)v2.unk_00_val2_unk_00 - (s8)v3.unk_00_val2_unk_00);

    if (v1 < 0) {
        v0 = ((-v1) / 24) + 1;
        v2.unk_00_val2_unk_00 += 24 * v0;
    }

    param2->unk_00_val2_unk_00 = v2.unk_00_val2_unk_00 - v3.unk_00_val2_unk_00;
}

void ov45_0222D638(UnkStruct_ov66_02231560 *param0, u16 param1, u8 param2, BOOL param3);

void ov45_0222D638(UnkStruct_ov66_02231560 *param0, u16 param1, u8 param2, BOOL param3) {
    int v0;
    BOOL v1;
    u32 v2;

    if (param1 == 0) {
        return;
    }

    v2 = GeonetLocations_GetLocationCountFromCountry(param1);

    if (param2 > v2) {
        return;
    }

    v1 = 0;

    for (v0 = 0; v0 < 50; v0++) {
        if (param0->unk_00[v0].unk_03_4 == 0) {
            v1 = 1;
        } else {
            if ((param0->unk_00[v0].unk_00 == param1) && (param0->unk_00[v0].unk_02 == param2)) {
                if (param3 == 1) {
                    return;
                } else {
                    v1 = 1;
                }
            }
        }

        if (v1) {
            param0->unk_00[v0].unk_00 = param1;
            param0->unk_00[v0].unk_02 = param2;
            param0->unk_00[v0].unk_03_0 = param3;
            param0->unk_00[v0].unk_03_4 = 1;
            return;
        }
    }
}

u16 ov45_0222D6B0(const UnkStruct_ov66_02231560 *param0, u8 param1);

u16 ov45_0222D6B0(const UnkStruct_ov66_02231560 *param0, u8 param1) {
    GF_ASSERT(param1 < 50);
    GF_ASSERT(param0->unk_00[param1].unk_03_4 == 1);

    return param0->unk_00[param1].unk_00;
}

u8 ov45_0222D6D4(const UnkStruct_ov66_02231560 *param0, u8 param1);

u8 ov45_0222D6D4(const UnkStruct_ov66_02231560 *param0, u8 param1) {
    GF_ASSERT(param1 < 50);
    GF_ASSERT(param0->unk_00[param1].unk_03_4 == 1);

    return param0->unk_00[param1].unk_02;
}

BOOL ov45_0222D6FC(const UnkStruct_ov66_02231560 *param0, u8 param1);

BOOL ov45_0222D6FC(const UnkStruct_ov66_02231560 *param0, u8 param1) {
    GF_ASSERT(param1 < 50);
    GF_ASSERT(param0->unk_00[param1].unk_03_4 == 1);

    return param0->unk_00[param1].unk_03_0;
}

u32 ov45_0222D724(u8 *arg0, u32 arg1) {
    if (arg1 >= 0x32U) {
        GF_AssertFail();
    }
    return (u32)(*(u8 *)((u8 *)(arg0 + (arg1 * 4)) + 3) << 0x18) >> 0x1C;
}
