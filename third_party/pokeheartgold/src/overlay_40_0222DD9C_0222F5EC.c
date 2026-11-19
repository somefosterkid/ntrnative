#include "global.h"
struct UnkStruct_ov62_02233310;

#include "error_handling.h"
#include "unk_02005D10.h"
#include "overlay_40_unk_struct_ov62_02233310.h"

s32 ov40_0222F8C0(void *);

void ov40_0222F5EC(struct UnkStruct_ov62_02233310 *arg0, s32 arg1);

void ov40_0222F5EC(UnkStruct_ov62_02233310 *arg0, s32 arg1) {
    s16 v11;
    s16 v12;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 v6;
    s32 v7;
    s32 v8;
    s32 v9;
    s32 v10;
    s32 v13;

    v12 = 0;
    v13 = 0xFF;
    if (arg1 < 0) {
        v0 = *((s32 *)&arg0->unk_38.unk_00);
        if (v0 > 0) {
            *((s32 *)&arg0->unk_38.unk_00) = (s32)(v0 - 1);
            v13 = 0;
            PlaySE(0x572U);
        }
    } else if (arg1 > 0) {
        v4 = *((s32 *)&arg0->unk_38.unk_00);
        if (v4 < (s32)(*((s32 *)&arg0->unk_04) - 1)) {
            *((s32 *)&arg0->unk_38.unk_00) = (s32)(v4 + 1);
            v13 = 1;
            PlaySE(0x572U);
        }
    }
    switch (v13) {
    case 0:
        v8 = *((s32 *)&arg0->unk_38.unk_0C);
        v5 = *((s32 *)&arg0->unk_38.unk_00);
        if (v5 < v8) {
            v12 = (s16)v5;
        } else {
            v1 = *((s32 *)&arg0->unk_04) - v8;
            if (v5 >= v1) {
                v12 = v8 + 1;
                if (v5 >= v1) {
                    v12 += v5 - v1;
                }
            } else {
                v12 = (s16)v8;
                if (v5 <= v8) {
                    v12 += v5 - v8;
                }
            }
        }
        break;
    case 1:
        v2 = *((s32 *)&arg0->unk_38.unk_00);
        v9 = *((s32 *)&arg0->unk_38.unk_08);
        if (v2 <= v9) {
            v12 = (s16)v2;
        } else {
            v12 = (s16)v9;
            v6 = *((s32 *)&arg0->unk_04) - v9;
            if (v2 >= v6) {
                v12 += v2 - (v6 - 1);
            }
        }
        break;
    }
    if ((s32)v12 < 0) {
        GF_AssertFail();
    }
    if (v13 != 0xFF) {
        *((s32 *)&arg0->unk_38.unk_04) = (s32)v12;
    }
    v10 = *((s32 *)&arg0->unk_38.unk_08);
    v11 = 0;
    if ((s32) *((s32 *)&arg0->unk_38.unk_04) >= v10) {
        v7 = *((s32 *)&arg0->unk_38.unk_00);
        v11 = v7 - v10;
        v3 = *((s32 *)&arg0->unk_04);
        if (v7 >= (s32)(v3 - (v10 + 1))) {
            v11 = v3 - *((s32 *)&arg0->unk_10);
        }
    }
    arg0->unk_08 = v11;
    ov40_0222F8C0(arg0);
}

s32 ov40_0222F878(void *);

void ov40_0222F6D0(void *arg0, s32 arg1);

void ov40_0222F6D0(void *arg0, s32 arg1) {
    s16 v0;
    s32 v1;
    s32 v2;

    v0 = *(s16 *)((u8 *)arg0 + 8);
    *(s16 *)((u8 *)arg0 + 8) = (s16)(v0 + arg1);
    if ((s32) * (s16 *)((u8 *)arg0 + 8) < 0) {
        *(s16 *)((u8 *)arg0 + 8) = 0;
    }
    if (*(s32 *)((u8 *)arg0 + 0x10) == *(s32 *)((u8 *)arg0 + 4)) {
        *(s16 *)((u8 *)arg0 + 8) = 0;
    }
    v2 = *(s32 *)((u8 *)arg0 + 0x10);
    v1 = *(s32 *)((u8 *)arg0 + 4);
    if ((s32)(*(s16 *)((u8 *)arg0 + 8) + v2) > v1) {
        *(s16 *)((u8 *)arg0 + 8) = (s16)(v1 - v2);
    }
    if (v0 != *(s16 *)((u8 *)arg0 + 8)) {
        PlaySE(0x572U);
    }
    ov40_0222F878(arg0);
}
