#include "global.h"

#include "error_handling.h"
#include "field/overlay_01_021FB878.h"

struct Field3dObject;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

int ov89_0225C8BC(int param0, int param1);

void sub_020182B0(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

void sub_020182CC(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
} UnkStruct_ov113_0225EDA4;

extern const UnkStruct_ov113_0225EDA4 ov89_0225CD3C[3];

void ov89_0225ADA4(Field3dObject *param0, const UnkStruct_ov113_02260818 *param1, UnkStruct_ov113_0225EDA4 *param2, int param3);

s32 ov89_0225AEA8(void *arg0, void *arg1);

void ov89_0225ADA4(Field3dObject *param0, const UnkStruct_ov113_02260818 *param1, UnkStruct_ov113_0225EDA4 *param2, int param3) {
    fx32 v0, v1, v2;
    fx32 v3, v4, v5;
    int v6;
    fx64 v7, v8, v9, v10;

    v6 = ov89_0225C8BC(param1->unk_02, param1->unk_08);
    GF_ASSERT(v6 < NELEMS(ov89_0225CD3C));

    sub_020182B0(param0, &v0, &v1, &v2);

    param2->unk_00 = v0 + ov89_0225CD3C[v6].unk_00;
    param2->unk_04 = v0 + ov89_0225CD3C[v6].unk_04;
    param2->unk_08 = v1 + ov89_0225CD3C[v6].unk_08;
    param2->unk_0C = v1 + ov89_0225CD3C[v6].unk_0C;

    if (param3 == 1) {
        sub_020182CC(param0, &v3, &v4, &v5);

        v7 = ((fx64)v3) * 100 / FX32_ONE;
        v9 = (param2->unk_04 - param2->unk_00) * v7 / 100 - (param2->unk_04 - param2->unk_00);

        param2->unk_04 += v9 / 2;
        param2->unk_00 += -(v9 / 2);

        v8 = ((fx64)v4) * 100 / FX32_ONE;
        v10 = (param2->unk_08 - param2->unk_0C) * v8 / 100 - (param2->unk_08 - param2->unk_0C);

        param2->unk_08 += v10 / 2;
        param2->unk_0C += -(v10 / 2);
    }
}

s32 ov89_0225AEA8(void *arg0, void *arg1) {
    if (((s32) * (s32 *)arg0 <= (s32) * (s32 *)((u8 *)arg1 + 4)) && ((s32) * (s32 *)arg1 <= (s32) * (s32 *)((u8 *)arg0 + 4)) && ((s32) * (s32 *)((u8 *)arg0 + 8) >= (s32) * (s32 *)((u8 *)arg1 + 0xC)) && ((s32) * (s32 *)((u8 *)arg1 + 8) >= (s32) * (s32 *)((u8 *)arg0 + 0xC))) {
        return 1;
    }
    return 0;
}
