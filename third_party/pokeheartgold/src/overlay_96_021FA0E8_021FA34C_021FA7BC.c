#include "global.h"

#include "pokeathlon/pokeathlon.h"

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);

void ov96_021EAE9C(u32 *param_1, s32 *param_2, s32 *param_3);

void sub_0200606C(u32, s32, u32, u32 *);

void sub_020061D0(s32, s32);

extern u8 ov96_0221DC2C[];

extern u8 ov96_0221DC28[];

void ov96_021FA7BC(PokeathlonCourseData *param_1, u32 param_2, u8 param_3, u32 *param_4);

void ov96_021FA7BC(PokeathlonCourseData *param_1, u32 param_2, u8 param_3, u32 *param_4) {
    u32 uVar1;

    if (param_4[1] == 0) {
        if (*(u8 *)(param_4 + 0xd) != 0) {
            sub_0200606C(0x60a, *(u8 *)(((u8 *)ov96_0221DC2C) + param_3), param_3, param_4);
            uVar1 = ov96_021E5F24(param_1);
            ov96_021E8228(param_1, uVar1 & 0xff, param_3, 2, 1);
        } else {
            sub_0200606C(0x8aa, *(u8 *)(((u8 *)ov96_0221DC2C) + param_3), param_3, param_4);
        }
        sub_020061D0(*(u8 *)(((u8 *)ov96_0221DC2C) + param_3), (s32) * (s8 *)(((u8 *)ov96_0221DC28) + param_3));
        param_4[1] = 1;
        *(u8 *)(param_4 + 3) = 0;
        param_4[2] = 1;
        ov96_021EAE9C((u32 *)*param_4, param_4 + 4, param_4 + 5);
    }
    return;
}
