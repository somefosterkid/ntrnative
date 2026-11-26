#include "global.h"

#include "constants/heap.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

void ov49_0225D4C8(UnkStruct_ov70_022610B8 *param0, fx32 param1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_0225D4D0(UnkStruct_ov70_022610B8 *param0, s8 param1, s8 param2);
#else
void ov49_0225D4D0(UnkStruct_ov70_022610B8 *param0, u8 param1, u8 param2);
#endif

void ov49_0225D4F0(UnkStruct_ov70_022610B8 *param0, fx32 param1, fx32 param2, fx32 param3);

BOOL ov49_0225EC30(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);

void ov49_0225ECD4(void *arg0, s32 arg1);

BOOL ov49_0225EC30(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1) {
    if (param0->unk_06 < 10) {
        param0->unk_06++;

        {
            fx32 v0;
            fx32 v1;

            v0 = param0->unk_08 - 0;
            v1 = FX_MulInline(param0->unk_06 << 12, v0);
            v1 = FX_Div(v1, 10 << 12);
            v1 += param0->unk_08;

            ov49_0225D4C8(param0->unk_00, v1);
        }

        {
            u32 v2;

            v2 = (param0->unk_06 * 31) / 10;
            v2 = 31 - v2;

            ov49_0225D4D0(param0->unk_00, v2, 31);
        }

        {
            fx32 v3;

            v3 = FX_MulInline(param0->unk_06 << 12, ((fx32)(((0.180f) > 0) ? ((0.180f) * (1 << 12) + 0.5f) : ((0.180f) * (1 << 12) - 0.5f))));
            v3 = FX_Div(v3, 10 << 12);
            v3 = ((fx32)0x0000000000001000L) + v3;

            ov49_0225D4F0(param0->unk_00, v3, v3, v3);
        }

        return 0;
    }

    return 1;
}

void ov49_0225ECD4(void *arg0, s32 arg1) {
    if ((u32)(u16)(*(u16 *)((u8 *)arg0 + 4) + 0xFFFF) > 1U) {
        *(u16 *)((u8 *)arg0 + 4) = 1U;
        *(s32 *)((u8 *)arg0 + 8) = arg1;
    }
}
