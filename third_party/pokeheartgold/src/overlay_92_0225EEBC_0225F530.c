#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overlay_92.h"
#include "screen_fade.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02018000.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

BOOL ov92_02260428(UnkStruct_ov116_0226501C *param0, int param1, int param2, int param3, int param4, f32 param5, BOOL param6);

s32 ov92_0225D9B4(Field3DModelAnimation *, s32);

s32 ov92_0225DA2C(Field3DModelAnimation *);

s32 ov92_022630E8(Field3DModelAnimation *);

s32 sub_020180BC(void *, Field3DModelAnimation *, NARC *, s32, s32, void *);

extern s32 ov92_02263CD0;

extern s32 ov92_02263CD4;

void ov92_0225F530(SysTask *param0, void *param1);

void ov92_0225F530(SysTask *param0, void *param1) {
    SysTask *var_r7;
    Field3DModelAnimation *var_r6;
    Field3DModelAnimation *var_r5;
    void *var_r4;
    s32 var_r3;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    s32 temp_r1;
    u32 temp_r0;

    var_r4 = param1;
    var_r7 = param0;
    var_r6 = ((Field3DModelAnimation *)((UnkStruct_ov116_02262D08 *)var_r4)->unk_34);
    var_r5 = ((Field3DModelAnimation *)((UnkStruct_ov116_02262D08 *)var_r4)->unk_38);
    if ((IsPaletteFadeFinished() == 0) || (temp_r2 = ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C, (*(u8 *)((u8 *)temp_r2 + 0x34) == 1))) {
        ov92_0225DA2C(var_r6);
        ov92_0225DA2C(var_r5);
        sub_020180F8(((void *)var_r6) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        var_r6 = (void *)((u8 *)var_r6 + (0x9C));
        sub_020180F8(var_r6, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        sub_020180F8(((void *)var_r5) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        var_r5 = (void *)((u8 *)var_r5 + (0x9C));
        sub_020180F8(var_r5, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        *((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_30) = 0;
        SysTask_Destroy(var_r7);
        return;
    }
    temp_r1 = *((s32 *)((UnkStruct_ov116_02262D08 *)var_r4)->unk_2C);
    var_r3 = temp_r1 - 1;
    if ((var_r3 < 0) || (var_r3 > 4)) {
        var_r3 = 0;
    }
    temp_r0 = *(u32 *)var_r4;
    switch (temp_r0) {
    case 0:
        ov92_0225D9A8((s32)var_r6, *(s32 *)temp_r2, *(s32 *)(&ov92_02263CD0 + var_r3 * 2));
        ov92_0225D9B4(var_r6, *((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_40));
        *(u32 *)var_r4 = (u32)(*(u32 *)var_r4 + 1);
        return;
    case 1:
        sub_020180BC(((void *)var_r6) + 0x88, ((void *)var_r6) + 0x78, *(void **)temp_r2, 0x27, 0x71, temp_r2 + 0x24);
        temp_r2_2 = ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C;
        sub_020180BC(((void *)var_r6) + 0x9C, ((void *)var_r6) + 0x78, *(void **)temp_r2_2, 0x28, 0x71, temp_r2_2 + 0x24);
        sub_020181D4((NNSG3dRenderObj *)var_r6, ((void *)var_r6) + 0x88);
        sub_020181D4((NNSG3dRenderObj *)var_r6, ((void *)var_r6) + 0x9C);
        sub_02018198(((void *)var_r6) + 0x88, 0);
        sub_02018198(((void *)var_r6) + 0x9C, 0);
        sub_020182A0(var_r6, 0);
        *(s32 *)((u8 *)var_r6 + 0x1EC) = 0;
        ov92_022630E8(((void *)var_r6) + 0x190);
        ov92_022630E8(((void *)var_r6) + 0x1A0);
        ov92_02260428((UnkStruct_ov116_0226501C *)var_r6, 0, 0, 5, 5, 0.8f, 0);
        ov92_02260428((UnkStruct_ov116_0226501C *)var_r6, 0, 0, -5, -5, 0.8f, 0);
        *(u32 *)var_r4 = (u32)(*(u32 *)var_r4 + 1);
        return;
    case 2:
        ov92_0225D9A8((s32)var_r5, *(s32 *)temp_r2, *(s32 *)(&ov92_02263CD4 + var_r3 * 2));
        ov92_0225D9B4(var_r5, *((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_40));
        *(u32 *)var_r4 = (u32)(*(u32 *)var_r4 + 1);
        return;
    case 3:
        sub_020180BC(((void *)var_r5) + 0x88, ((void *)var_r5) + 0x78, *(void **)temp_r2, 0x29, 0x71, temp_r2 + 0x24);
        temp_r2_3 = ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C;
        sub_020180BC(((void *)var_r5) + 0x9C, ((void *)var_r5) + 0x78, *(void **)temp_r2_3, 0x2A, 0x71, temp_r2_3 + 0x24);
        sub_020181D4((NNSG3dRenderObj *)var_r5, ((void *)var_r5) + 0x88);
        sub_020181D4((NNSG3dRenderObj *)var_r5, ((void *)var_r5) + 0x9C);
        sub_02018198(((void *)var_r5) + 0x88, 0);
        sub_02018198(((void *)var_r5) + 0x9C, 0);
        sub_020182A0(var_r5, 1);
        *(s32 *)((u8 *)var_r5 + 0x1EC) = 1;
        ov92_022630E8(((void *)var_r5) + 0x190);
        ov92_022630E8(((void *)var_r5) + 0x1A0);
        ov92_02260428((UnkStruct_ov116_0226501C *)var_r5, 0, 0, 5, 5, 0.8f, 0);
        ov92_02260428((UnkStruct_ov116_0226501C *)var_r5, 0, 0, -5, -5, 0.8f, 0);
        if (*((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_40) != 0) {
            sub_020182A8(var_r6, 0, -0x8000, 0);
            sub_020182A8(var_r5, 0, -0x8000, 0);
            *(s32 *)((u8 *)var_r6 + 0x1E4) = 0xFFFF8000;
            *(s32 *)((u8 *)var_r5 + 0x1E4) = 0xFFFF8000;
        }
        *(u32 *)var_r4 = (u32)(*(u32 *)var_r4 + 1);
        return;
    case 4:
        if (*(s32 *)((u8 *)var_r5 + 0x1EC) == 0) {
            sub_020182A0(var_r6, 1);
            sub_020182A0(var_r5, 0);
            *(s32 *)((u8 *)var_r6 + 0x1EC) = 1;
            *(u32 *)var_r4 = (u32)(*(u32 *)var_r4 + 1);
            return;
        }
        return;
    case 5:
        if (temp_r1 == 0) {
            *(u32 *)var_r4 = 0xFFU;
        } else if (*((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_28) != temp_r1) {
            ov92_0225DA2C(var_r6);
            ov92_0225DA2C(var_r5);
            sub_020180F8(((void *)var_r6) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
            var_r6 = (void *)((u8 *)var_r6 + (0x9C));
            sub_020180F8(var_r6, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
            sub_020180F8(((void *)var_r5) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
            var_r5 = (void *)((u8 *)var_r5 + (0x9C));
            sub_020180F8(var_r5, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
            *(u32 *)var_r4 = 0U;
        }
        *((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_28) = (s32) * ((s32 *)((UnkStruct_ov116_02262D08 *)var_r4)->unk_2C);
        return;
    default:
        ov92_0225DA2C(var_r6);
        ov92_0225DA2C(var_r5);
        sub_020180F8(((void *)var_r6) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        var_r6 = (void *)((u8 *)var_r6 + (0x9C));
        sub_020180F8(var_r6, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        sub_020180F8(((void *)var_r5) + 0x88, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        var_r5 = (void *)((u8 *)var_r5 + (0x9C));
        sub_020180F8(var_r5, ((UnkStruct_ov116_02262D08 *)var_r4)->unk_3C + 0x24);
        *((s32 *)&((UnkStruct_ov116_02262D08 *)var_r4)->unk_30) = 0;
        SysTask_Destroy(var_r7);
        break;
    }
}
