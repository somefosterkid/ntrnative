#include "global.h"

#include "error_handling.h"
#include "overlay_49.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

s32 ov49_0225D328(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *, s32);

u8 ov49_0225D450(UnkStruct_ov70_022610B8 *, s32);

void ov49_0225D394(UnkStruct_ov70_02260BB8 *arg0, UnkStruct_ov70_022610B8 *arg1) {
    s32 v0;

    v0 = 0;
    do {
        if (ov49_0225D450(arg1, v0) == 1) {
            ov49_0225D328(arg0, arg1, v0);
        }
        v0 += 1;
    } while (v0 < 3);
}

u8 ov49_0225D3BC(s32 arg0, void *arg1, s32 arg2);

u8 ov49_0225D3BC(s32 arg0, void *arg1, s32 arg2) {
    if (arg2 >= 3) {
        GF_AssertFail();
    }
    if ((u32) * (u16 *)((u8 *)arg1 + 2) >= 0x12U) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)(arg0 + (0xC * *(u16 *)((u8 *)arg1 + 2)) + (arg2 * 4)) + 0x3BC) != 0) {
        return *(u8 *)((u8 *)(arg1 + arg2) + 0xB8);
    }
    return 0U;
}
