#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

struct UnkStruct_ov70_02261E10_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

s32 ov49_0225D214(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *, u8, s32 *);

s32 ov49_0225D328(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *, s32, s32);

s32 ov49_0225D394(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);

s32 ov49_0225D3F8(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *, s32, s32);

u8 ov49_0225D450(void *, s32);

s32 ov49_0225D4A0(UnkStruct_ov70_02260BB8 *, void *, s32);

void ov49_0225E8C4(UnkStruct_ov70_02261E10 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov49_0225E8C4(UnkStruct_ov70_02261E10 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    f32 var_r0;
    f32 var_r0_2;
    u32 temp_r0;
    UnkStruct_ov70_02260BB8 *temp_r0_2;
    UnkStruct_ov70_022610B8 *temp_r6;
    UnkStruct_ov70_022610B8 *temp_r7;
    temp_r6 = arg0->unk_22C.unk_00[arg1];
    temp_r7 = arg0->unk_22C.unk_0C[arg1];
    if ((arg4 == 1) || (arg3 == 1)) {
        if (ov49_0225D450(temp_r6, 1) == 0) {
            ov49_0225D214(arg0->unk_04, temp_r6, 1, 0);
        }
        if (arg4 == 1) {
            temp_r0 = arg2;
            temp_r0 = temp_r0 + 4;
            if ((temp_r0 * 4) != 0) {
                var_r0 = 0.5f + ((f32)(temp_r0 << 0xE));
            } else {
                var_r0 = ((f32)(temp_r0 << 0xE)) - 0.5f;
            }
            ov49_0225D3F8(arg0->unk_04, temp_r7, 0, (s32)var_r0);
            return;
        }
        arg0->unk_22C.unk_3E0 = (arg0->unk_22C.unk_3E0 + 1) % 28U;
        if (arg0->unk_22C.unk_3E0 < 0xEU) {
            if ((arg2 * 4) != 0) {
                var_r0_2 = 0.5f + ((f32)(arg2 << 0xE));
            } else {
                var_r0_2 = ((f32)(arg2 << 0xE)) - 0.5f;
            }
            ov49_0225D3F8(arg0->unk_04, temp_r7, 0, (s32)var_r0_2);
            return;
        }
        ov49_0225D3F8(arg0->unk_04, temp_r7, 0, 0);
        return;
    }
    temp_r0_2 = arg0->unk_04;
    if (arg5 != 0) {
        ov49_0225D394(arg0->unk_04, temp_r6);
        ov49_0225D394(arg0->unk_04, temp_r7);
        ov49_0225D4A0(arg0->unk_04, temp_r6, 1);
        return;
    }
    ov49_0225D328(arg0->unk_04, temp_r6, 1, arg3);
    ov49_0225D3F8(arg0->unk_04, temp_r7, 0, 0);
}
#else
s32 ov49_0225D214(void *, s32, s32, s32 *);

s32 ov49_0225D328(s32, s32, s32, s32);

s32 ov49_0225D394(s32, s32);

s32 ov49_0225D3F8(void *, s32, s32, s32);

s32 ov49_0225D450(s32, s32);

s32 ov49_0225D4A0(void *, s32, s32);

void ov49_0225E8C4(void *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov49_0225E8C4(void *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    f32 var_r0;
    f32 var_r0_2;
    u32 temp_r0;
    s32 temp_r0_2;
    u32 temp_r6;
    s32 temp_r7;
    void *temp_r1;
    temp_r1 = arg0 + (arg1 * 4);
    temp_r6 = *((s32 *)(((u8 *)temp_r1) + 0x22C));
    temp_r7 = *((s32 *)(((u8 *)temp_r1) + 0x238));
    if ((arg4 == 1) || (arg3 == 1)) {
        if (ov49_0225D450(temp_r6, 1) == 0) {
            ov49_0225D214(*((void **)(((u8 *)arg0) + 4)), temp_r6, 1, 0);
        }
        if (arg4 == 1) {
            temp_r0 = arg2;
            temp_r0 = temp_r0 + 4;
            if ((temp_r0 * 4) != 0) {
                var_r0 = 0.5f + ((f32)(temp_r0 << 0xE));
            } else {
                var_r0 = ((f32)(temp_r0 << 0xE)) - 0.5f;
            }
            ov49_0225D3F8(*((void **)(((u8 *)arg0) + 4)), temp_r7, 0, (s32)var_r0);
            return;
        }
        *((u32 *)(((u8 *)arg0) + 0x60C)) = (u32)(((u32)((*((u32 *)(((u8 *)arg0) + 0x60C))) + 1)) % 28U);
        if (((u32)(*((u32 *)(((u8 *)arg0) + 0x60C)))) < 0xEU) {
            if ((arg2 * 4) != 0) {
                var_r0_2 = 0.5f + ((f32)(arg2 << 0xE));
            } else {
                var_r0_2 = ((f32)(arg2 << 0xE)) - 0.5f;
            }
            ov49_0225D3F8(*((void **)(((u8 *)arg0) + 4)), temp_r7, 0, (s32)var_r0_2);
            return;
        }
        ov49_0225D3F8(*((void **)(((u8 *)arg0) + 4)), temp_r7, 0, 0);
        return;
    }
    temp_r0_2 = *((s32 *)(((u8 *)arg0) + 4));
    if (arg5 != 0) {
        ov49_0225D394(*((s32 *)(((u8 *)arg0) + 4)), temp_r6);
        ov49_0225D394(*((s32 *)(((u8 *)arg0) + 4)), temp_r7);
        ov49_0225D4A0(*((void **)(((u8 *)arg0) + 4)), temp_r6, 1);
        return;
    }
    ov49_0225D328(*((s32 *)(((u8 *)arg0) + 4)), temp_r6, 1, arg3);
    ov49_0225D3F8(*((void **)(((u8 *)arg0) + 4)), temp_r7, 0, 0);
}
#endif
