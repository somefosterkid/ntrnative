#include "global.h"

#include "error_handling.h"

struct UnkStruct_ov70_02261E10_t;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    union {
        struct {
            s16 unk_00;
            s16 unk_02;
            u16 unk_04;
            s16 unk_06;
            s16 unk_08;
        } val1;
    } unk_04;
} UnkStruct_ov70_0226D2A0;

void ov49_02269474(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

void ov49_02269494(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

BOOL ov49_022693F8(UnkStruct_ov70_0226D2A0 *param0, u32 param1, UnkStruct_ov70_02261E10 *param2, u32 param3);

BOOL ov49_022693F8(UnkStruct_ov70_0226D2A0 *param0, u32 param1, UnkStruct_ov70_02261E10 *param2, u32 param3) {
    extern void (*const ov49_0226A8D4[])(UnkStruct_ov70_0226D2A0 *, UnkStruct_ov70_02261E10 *, u32);
    GF_ASSERT(param1 < 2);

    if (param0->unk_00 == 1) {
        return 0;
    }

    param0->unk_02 = param1;
    param0->unk_00 = 1;
    ov49_0226A8D4[param1](param0, param2, param3);

    return 1;
}

BOOL ov49_022694B4(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

BOOL ov49_02269430(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2);

BOOL ov49_02269430(UnkStruct_ov70_0226D2A0 *param0, UnkStruct_ov70_02261E10 *param1, u32 param2) {
    BOOL v0;

    extern BOOL (*const ov49_0226A8CC[])(UnkStruct_ov70_0226D2A0 *, UnkStruct_ov70_02261E10 *, u32);

    if (param0->unk_00) {
        v0 = ov49_0226A8CC[param0->unk_02](param0, param1, param2);

        if (v0 == 1) {
            memset(param0, 0, sizeof(UnkStruct_ov70_0226D2A0));
        }

        return 1;
    }

    return 0;
}
