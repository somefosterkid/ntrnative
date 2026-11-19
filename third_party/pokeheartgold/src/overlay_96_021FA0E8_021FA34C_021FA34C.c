#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_f94a8.h"

s32 ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

s32 ov96_021EAC0C(s32 *, s32);

s32 ov96_021EAF8C(void *);

s32 ov96_021EAF90(void *);

Sprite *ov96_021EB5B8(const SaveData *);

u8 *ov96_021FA340(void *, s32);

s32 ov96_021FA7BC(PokeathlonCourseData *, s32, u8, void *);

s32 ov96_021FA83C(s32, void *, void *);

s32 ov96_021FAAE0(s32, int, s32 *);

s32 ov96_021FAB04(s32, s32 *);

s32 ov96_021FAB24(BgConfig *, UnkStruct_ov96_021FAB24 *);

s32 ov96_021FB6B4(UnkStruct_ov96_021F94A8 *, u32, s32);

s32 ov96_021FB6C8(UnkStruct_ov96_021F94A8 *, UnkStruct_ov96_021FAB24 *);

s32 ov96_021FBCB8(PokeathlonCourseData *, UnkStruct_ov96_021FAB24 *);

s32 ov96_021FBEA0(u8 *param_1);

s32 ov96_021FBEA4(u8 *);

s32 ov96_021FBEFC(void *, s32);

s32 ov96_021FBF5C(u8 *, s32 *);

s32 ov96_021FC6EC(void *, u32, s32);

s32 sub_0200606C(s32, s32);

s32 sub_020061D0(s32, s32);

extern s8 ov96_0221DC28[];

extern u8 ov96_0221DC2C[];

void ov96_021FA34C(PokeathlonCourseData *arg0, UnkStruct_ov96_021F94A8 *arg1, UnkStruct_ov96_021FAB24 *arg2, s32 *arg3);

void ov96_021FA34C(PokeathlonCourseData *arg0, UnkStruct_ov96_021F94A8 *arg1, UnkStruct_ov96_021FAB24 *arg2, s32 *arg3) {
    s32 sp1C[2];
    s32 sp14[2];
    s32 sp10;
    u8 *spC;
    s32 sp8;
    UnkStruct_ov96_021F94A8 *sp4;
    Sprite *temp_r0_2;
    f32 temp_r0;
    f32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    u8 *temp_r7;
    void *temp_r7_2;
    s32 var_r0;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r6_3;
    u16 temp_r1;
    u8 temp_r2;

    sp4 = arg1;
    if ((arg2->unk_8 != 1) && (arg2->unk_28 != 2) && (arg2->unk_9 == 0) && (arg2->unk_C < arg2->unk_50)) {
        temp_r0 = arg2->unk_C + arg2->unk_54;
        arg2->unk_C = temp_r0;
        if (temp_r0 > arg2->unk_50) {
            arg2->unk_C = (f32) arg2->unk_50;
        }
    }
    if ((arg3 != NULL) && (arg2->unk_28 == 0)) {
        ov96_021FA7BC(arg0, arg3[1], arg2->unk_18, &arg2->unk_24);
        sp8 = 0;
        if (arg2->unk_58 != 0) {
            for (var_r6 = 0; var_r6 < 4; var_r6 += 1) {
                temp_r7 = ov96_021FA340(arg2, var_r6);
                if (ov96_021FBEA0(temp_r7) != 0) {
                    ov96_021FBF5C(temp_r7, sp1C);
                    if (ov96_021FAB04((arg2->unk_3C + 0x110) - ov96_021EAF8C(arg2->unk_24), sp1C) != 0) {
                        sp8 = 1;
                        ov96_021FBCB8(arg0, arg2);
                        break;
                    }
                }
            }
        }
        temp_r0_2 = ov96_021EB5B8(arg2->unk_44);
        Sprite_SetDrawFlag(temp_r0_2, 1);
        if (sp8 != 0) {
            Sprite_SetAnimCtrlSeq(temp_r0_2, 0);
        } else {
            Sprite_SetAnimCtrlSeq(temp_r0_2, 1);
        }
    } else if ((arg3 != NULL) && (arg2->unk_28 == 1) && (arg2->unk_2C == 3) && (arg2->unk_58 != 0)) {
        arg2->unk_60 = 1U;
        arg2->unk_62 = (u16)(arg2->unk_62 + 1);
    }
    temp_r0_3 = arg2->unk_C + arg2->unk_10;
    temp_r0_4 = (s32)temp_r0_3;
    arg2->unk_10 = (f32)(temp_r0_3 - (f32)temp_r0_4);
    arg2->timer = (u16)(arg2->timer + temp_r0_4);
    ov96_021FB6B4(sp4, arg2->timer, arg2->unk_18);
    ov96_021FB6C8(sp4, arg2);
    arg2->unk_4 = (u16)(arg2->unk_4 - temp_r0_4);
    temp_r0_5 = arg2->unk_4 + 0x200;
    if (temp_r0_5 <= 0) {
        arg2->unk_4 = (u16)temp_r0_5;
    }
    arg2->unk_6 = (u16)(arg2->unk_4 + 0x110);
    temp_r0_6 = arg2->unk_6 + 0x200;
    if (temp_r0_6 <= 0) {
        arg2->unk_6 = (u16)temp_r0_6;
    }
    var_r6_2 = 0;
    do {
        ov96_021FBEFC(ov96_021FA340(arg2, var_r6_2), temp_r0_4);
        var_r6_2 += 1;
    } while (var_r6_2 < 4);
    ov96_021FC6EC(sp4->unk_DC, arg2->unk_18, temp_r0_4);
    ov96_021FA83C(arg2->unk_18, &arg2->unk_24, &arg2->unk_C);
    if (arg2->unk_8 == 1) {
        temp_r0_7 = arg2->unk_28;
        if (temp_r0_7 == 0) {
            arg2->unk_28 = 3;
        } else if (temp_r0_7 == 3 || temp_r0_7 == 1) {
            sp10 = arg2->unk_3C;
            temp_r1 = arg2->timer;
            if ((u32)temp_r1 >= 0x1080U) {
                if (sp10 == 0x78) {
                    arg2->unk_28 = 4;
                    arg2->unk_C = 0.0f;
                    arg2->unk_10 = 0.0f;
                }
            } else if ((u32)temp_r1 >= 0x1000U) {
                arg2->unk_C = 1.0f;
                arg2->unk_10 = 0.0f;
                ov96_021EAC0C(arg2->unk_24, 1);
                Sprite_SetAnimCtrlSeq(ov96_021EB5B8(arg2->unk_4C), 0);
            }
        }
        ov96_021FAB24(*(void **)((u8 *)sp4 + 4), arg2);
        return;
    }
    var_r0 = 1;
    if (arg2->unk_28 == 1) {
        var_r0 = 0;
    }
    if ((arg2->unk_60 == 1) && (arg2->unk_62 == 1)) {
        var_r0 = 1;
    }
    if (var_r0 != 0) {
        for (var_r6_3 = 0; var_r6_3 < 4; var_r6_3 += 1) {
            spC = ov96_021FA340(arg2, var_r6_3);
            if (ov96_021FBEA0(spC) != 0) {
                temp_r7_2 = arg2->unk_24;
                ov96_021FBF5C(spC, sp14);
                temp_r0_8 = ov96_021EAF90(temp_r7_2);
                sp10 = temp_r0_8;
                if (ov96_021FAAE0((arg2->unk_3C + 0x110) - temp_r0_8, (u8)ov96_021EAF8C(temp_r7_2), sp14) != 0) {
                    ov96_021FBEA4(spC);
                    sub_0200606C(0x8A5, ov96_0221DC2C[arg2->unk_18]);
                    temp_r2 = arg2->unk_18;
                    sub_020061D0(ov96_0221DC2C[temp_r2], ov96_0221DC28[temp_r2]);
                    if ((arg2->unk_60 == 1) && (arg2->unk_62 == 1)) {
                        ov96_021E8228(arg0, (u8)ov96_021E5F24(arg0), arg2->unk_18, 2, 1);
                        arg2->unk_30 = 3;
                        arg2->unk_2C = 1;
                        ov96_021FBCB8(arg0, arg2);
                    } else {
                        arg2->unk_C = 0.5f;
                        arg2->unk_9 = 0U;
                        arg2->unk_28 = 2;
                        arg2->unk_31 = 0;
                        arg2->unk_32 = 1;
                        arg2->unk_2C = 4;
                        Sprite_SetAnimCtrlSeq(ov96_021EB5B8(arg2->unk_4C), 0);
                        ov96_021E8228(arg0, (u8)ov96_021E5F24(arg0), arg2->unk_18, 1, 1);
                    }
                    arg2->unk_60 = 0U;
                    arg2->unk_62 = 0U;
                }
            }
        }
    }
    if ((arg2->unk_8 != 1) && ((u32) * (u16 *)arg2 >= 0x1000U)) {
        arg2->unk_8 = 1U;
        arg2->unk_2 = (s16) sp4->frameCount;
        sub_0200606C(0x8AB, ov96_0221DC2C[arg2->unk_18]);
        sp4->unk_3C4 = 0x1E;
    }
    ov96_021FAB24(*(void **)((u8 *)sp4 + 4), arg2);
}
