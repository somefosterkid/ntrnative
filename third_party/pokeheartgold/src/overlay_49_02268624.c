#include "global.h"

typedef struct {
    s8 unk_00[4];
} UnkStruct_ov70_0226C4CC;

void ov49_02268624(const UnkStruct_ov70_0226C4CC *param0, GXRgb *param1);

void ov49_02268640(void *arg0, u16 *arg1);

void ov49_02268664(const UnkStruct_ov70_0226C4CC *param0, const UnkStruct_ov70_0226C4CC *param1, UnkStruct_ov70_0226C4CC *param2, s32 param3, s32 param4);

void ov49_02268624(const UnkStruct_ov70_0226C4CC *param0, GXRgb *param1) {
    *param1 = GX_RGB(param0->unk_00[0], param0->unk_00[1], param0->unk_00[2]);
}

void ov49_02268640(void *arg0, u16 *arg1) {
    *(s8 *)arg0 = (s8)(*arg1 & 0x1F);
    *(s8 *)((u8 *)arg0 + 1) = (s8)((s32)(0x3E0 & *arg1) >> 5);
    *(s8 *)((u8 *)arg0 + 2) = (s8)((s32)(0x7C00 & *arg1) >> 0xA);
}

void ov49_02268664(const UnkStruct_ov70_0226C4CC *param0, const UnkStruct_ov70_0226C4CC *param1, UnkStruct_ov70_0226C4CC *param2, s32 param3, s32 param4) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = param1->unk_00[0] - param0->unk_00[0];
    v1 = param1->unk_00[1] - param0->unk_00[1];
    v2 = param1->unk_00[2] - param0->unk_00[2];

    param2->unk_00[0] = param0->unk_00[0] + ((param3 * v0) / param4);
    param2->unk_00[1] = param0->unk_00[1] + ((param3 * v1) / param4);
    param2->unk_00[2] = param0->unk_00[2] + ((param3 * v2) / param4);
}
