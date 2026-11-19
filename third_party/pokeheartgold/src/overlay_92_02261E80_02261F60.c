#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov116_0226469C;

typedef struct {
    ManagedSprite *unk_00;
    int unk_04;
    s16 unk_08;
    u8 padding_0A[6];
    int unk_10;
    s16 unk_14;
    s16 unk_16;
    int unk_18;
    int unk_1C;
    UnkStruct_ov116_0226469C unk_20;
    UnkStruct_ov116_0226469C unk_38;
} UnkStruct_ov116_02265D84;

s32 ov92_02260860(UnkStruct_ov116_0226469C *, s32, s32, s32);

s32 ov92_02260870(UnkStruct_ov116_0226469C *);

s32 ov92_022619C4(s32);

s32 ov92_02261F60(UnkStruct_ov116_02265D84 *param0, s32 param1, s32 param2, s32 param3);

s32 ov92_02261F60(UnkStruct_ov116_02265D84 *param0, s32 param1, s32 param2, s32 param3) {
    s32 temp_r0;
    s32 temp_r4;

    temp_r0 = param0->unk_04;
    switch (temp_r0) {
    case 0:
        if (param2 == 0) {
            ov92_02260860(&param0->unk_20, 0x134000, 0x80000, ov92_022619C4(param1));
        } else {
            ov92_02260860(&param0->unk_20, 0xFFFC2000, 0x80000, ov92_022619C4(param1));
        }
        if (param3 == 2) {
            ov92_02260860(&param0->unk_38, 0xA0000, 0x88000, ov92_022619C4(param1));
        } else {
            ov92_02260860(&param0->unk_38, 0xA0000, 0xB8000, ov92_022619C4(param1));
        }
        param0->unk_04 += 1;
    case 1:
        temp_r4 = ov92_02260870(&param0->unk_20);
        ov92_02260870(&param0->unk_38);
        ManagedSprite_SetPositionFxXYWithSubscreenOffset(param0->unk_00, param0->unk_20.unk_00, param0->unk_38.unk_00, FX32_CONST(256));
        if (temp_r4 != 0) {
            return 1;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

s32 ov92_02262018(UnkStruct_ov116_02265D84 *param0, s32 param1, s32 param2, s32 param3);

s32 ov92_02262018(UnkStruct_ov116_02265D84 *param0, s32 param1, s32 param2, s32 param3) {
    s32 temp_r0;
    s32 temp_r4;

    temp_r0 = param0->unk_04;
    switch (temp_r0) {
    case 0:
        if (param2 == 0) {
            ov92_02260860(&param0->unk_20, 0x134000, 0x80000, ov92_022619C4(param1));
        } else {
            ov92_02260860(&param0->unk_20, 0xFFFC2000, 0x80000, ov92_022619C4(param1));
        }
        if (param3 == 2) {
            ov92_02260860(&param0->unk_38, 0x60000, 0x48000, ov92_022619C4(param1));
        } else {
            ov92_02260860(&param0->unk_38, 0x60000, 0x78000, ov92_022619C4(param1));
        }
        param0->unk_04 += 1;
    case 1:
        temp_r4 = ov92_02260870(&param0->unk_20);
        ov92_02260870(&param0->unk_38);
        ManagedSprite_SetPositionFxXYWithSubscreenOffset(param0->unk_00, param0->unk_20.unk_00, param0->unk_38.unk_00, FX32_CONST(256));
        if (temp_r4 != 0) {
            return 1;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
