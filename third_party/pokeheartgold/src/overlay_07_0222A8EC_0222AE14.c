#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_0222AE14_arg1 {
    u8 unk_0;
    u8 unk_1[0x3];
    void *unk_4;
    u8 unk_8[0x4];
    SpriteManager *unk_C;
    u8 unk_10[0x18];
    ManagedSprite *unk_28;
    u8 unk_2C[0x10];
    ManagedSprite *unk_3C;
    u8 unk_40[0x10];
    ManagedSprite *unk_50;
    u8 unk_54[0x4];
    s16 unk_58;
    s16 unk_5A;
    u8 unk_5C[0x4];
    u8 unk_60;
    u8 unk_61[0x13];
    s32 unk_74;
} UnkStruct_ov07_0222AE14_arg1;

s32 ov07_0222260C(void *);

s32 ov07_02222644(void *, f32 *, f32 *);

s32 ov07_022226FC(ManagedSprite *, s16, s16, s32, s32);

s32 ov07_02232508(void *);

void ov07_0222AE14(SysTask *arg0, void *param);

void ov07_0222AE14(SysTask *arg0, void *param) {
    UnkStruct_ov07_0222AE14_arg1 *arg1 = param;
    f32 x;
    f32 y;

    if (arg1->unk_0 == 0) {
        if (ov07_0222260C((void *)&arg1->unk_60) == 0) {
            ManagedSprite_SetDrawFlag(arg1->unk_28, 0);
            ManagedSprite_SetDrawFlag(arg1->unk_3C, 0);
            ManagedSprite_SetDrawFlag(arg1->unk_3C, 0);
            ov07_0221C448(arg1->unk_4, arg0);
            ov07_02232508(arg1);
            return;
        }
        ov07_02222644((void *)&arg1->unk_60, &x, &y);
        ManagedSprite_SetAffineScale(arg1->unk_28, x, y);
        ov07_022226FC(arg1->unk_28, arg1->unk_58, arg1->unk_5A, arg1->unk_74, 0);
        ManagedSprite_TickFrame(arg1->unk_28);
        ManagedSprite_TickFrame(arg1->unk_3C);
        ManagedSprite_TickFrame(arg1->unk_50);
        SpriteSystem_DrawSprites(arg1->unk_C);
    }
}
