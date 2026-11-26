#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_ov96_02203468work.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_021E8228.c (tools/pokehg_abi.py)
void ov96_021E8228(void *, u32, u32, s32, s32);
#else
void ov96_021E8228(void *, u8, u8, s32, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
extern VecFx32 ov96_0221C98C[];

void ov96_02203544(PokeathlonCourseData *arg0, Ov96_02203468Work *work);

void ov96_02203544(PokeathlonCourseData *arg0, Ov96_02203468Work *work) {
    VecFx32 diff;
    f32 scale;
    f32 dmg;
    s32 mag;
    s32 loss;
    u8 left;
    int i;
    int j;

    for (i = 0; i < 12; i++) {
        work->items[i].unk_1B = 0;
        for (j = 0; j < 12; j++) {
            if (work->balls[j].state != 1) {
                continue;
            }
            if (j / 3 == i / 3) {
                continue;
            }
            VEC_Subtract(&ov96_0221C98C[i], &work->balls[j].pos, &diff);
            if (VEC_Mag(&diff) >= 0x3000) {
                continue;
            }
            work->balls[j].state = 3;
            ov96_021E8228(arg0, (u8)(j / 3), (u8)(j % 3), 3, 1);
            work->items[j].unk_14 += 1;
            if (work->items[j].unk_14 > 0x3E7U) {
                work->items[j].unk_14 = 0x3E7U;
            }
            work->balls[j].hit = 1;
            work->items[i].unk_1B = 1;
            switch (work->balls[j].unk_46) {
            case 0:
                scale = 1.0f;
                break;
            case 1:
                scale = 20.0f;
                break;
            case 2:
                scale = 50.0f;
                break;
            case 3:
                scale = 100.0f;
                break;
            default:
                GF_AssertFail();
                scale = 1.0f;
                break;
            }
            work->balls[j].unk_46 = 0U;
            work->balls[j].unk_44 = 0;
            mag = VEC_Mag(&work->balls[j].vel);
            dmg = (f32)(mag / 0x1000);
            dmg = dmg * scale;
            dmg = dmg * work->items[j].unk_8;
            loss = (s32)(work->items[i].unk_C * dmg);
            if (loss < 0) {
                GF_AssertFail();
            }
            work->items[i].unk_16 = (s16)((work->items[i].unk_16 - loss) - 0x1E);
            if (work->items[i].unk_16 < 0) {
                work->items[i].unk_16 = 0;
            }
            left = 0x78 - work->items[i].unk_16;
            if (work->items[i].unk_18 == 0) {
                if ((u32)left > 0x28U) {
                    work->items[i].unk_18 = left;
                    ov96_021E8228(arg0, (u8)(i / 3), (u8)(i % 3), 1, 1);
                }
            } else {
                work->items[i].unk_18 += left;
                ov96_021E8228(arg0, (u8)(i / 3), (u8)(i % 3), 1, 1);
            }
        }
    }
}
#else
extern VecFx32 ov96_0221C98C;

void ov96_02203544(void *arg0, VecFx32 *arg1);

void ov96_02203544(void *arg0, VecFx32 *arg1) {
    VecFx32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    VecFx32 *sp18;
    VecFx32 *sp14;
    VecFx32 *sp10;
    s32 spC;
    VecFx32 *sp8;
    void *sp4;
    VecFx32 *var_r4;
    VecFx32 *var_r5;
    VecFx32 *var_r6;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_7;
    s32 temp_r0_8;
    f32 var_r7;
    f32 temp_f0;
    u16 temp_r0_3;
    u8 temp_r0_6;

    sp4 = arg0;
    sp20 = 0;
    sp8 = arg1;
    var_r4 = arg1;
    sp18 = &ov96_0221C98C;
loop_1:
    var_r5 = sp8;
    sp1C = 0;
    *(s8 *)((u8 *)var_r4 + 0x433) = 0;
    sp10 = var_r5;
    sp14 = var_r5;
    sp14 = ((void *)var_r5) + 0xC8;
    sp10 = ((void *)var_r5) + 0xD4;
    var_r6 = var_r5;
    temp_r0 = sp20 / 3;
    spC = temp_r0;
    sp24 = (s32)temp_r0;
    sp28 = (s32)(sp20 % 3);
loop_2:
    if (*(s32 *)((u8 *)var_r5 + 0xC4) == 1) {
        temp_r0_2 = sp1C / 3;
        if (temp_r0_2 != spC) {
            VEC_Subtract(sp18, sp14, &sp2C);
            if (VEC_Mag(&sp2C) < 0x3000) {
                *(s32 *)((u8 *)var_r5 + 0xC4) = 3;
                ov96_021E8228(sp4, (u8)temp_r0_2, (u8)(sp1C % 3), 3, 1);
                *(u16 *)((u8 *)var_r6 + 0x42C) = (u16)(*(u16 *)((u8 *)var_r6 + 0x42C) + 1);
                if ((u32) * (u16 *)((u8 *)var_r6 + 0x42C) > 0x3E7U) {
                    *(u16 *)((u8 *)var_r6 + 0x42C) = 0x3E7U;
                }
                *(s8 *)((u8 *)var_r5 + 0xFB) = 1;
                *(s8 *)((u8 *)var_r4 + 0x433) = 1;
                temp_r0_3 = *(u16 *)((u8 *)var_r5 + 0xFE);
                switch (temp_r0_3) {
                case 0:
                    var_r7 = 1.0f;
                    break;
                case 1:
                    var_r7 = 20.0f;
                    break;
                case 2:
                    var_r7 = 50.0f;
                    break;
                case 3:
                    var_r7 = 100.0f;
                    break;
                default:
                    GF_AssertFail();
                    var_r7 = 1.0f;
                    break;
                }
                *(u16 *)((u8 *)var_r5 + 0xFE) = 0U;
                *(s16 *)((u8 *)var_r5 + 0xFC) = 0;
                temp_r0_4 = VEC_Mag(sp10);
                temp_f0 = (f32)(temp_r0_4 / 0x1000);
                temp_f0 = temp_f0 * var_r7;
                temp_f0 = temp_f0 * *(f32 *)((u8 *)var_r6 + 0x420);
                temp_r0_5 = (s32)(*(f32 *)((u8 *)var_r4 + 0x424) * temp_f0);
                if (temp_r0_5 < 0) {
                    GF_AssertFail();
                }
                *(s16 *)((u8 *)var_r4 + 0x42E) = (s16)((*(s16 *)((u8 *)var_r4 + 0x42E) - temp_r0_5) - 0x1E);
                if ((s32) * (s16 *)((u8 *)var_r4 + 0x42E) < 0) {
                    *(s16 *)((u8 *)var_r4 + 0x42E) = 0;
                }
                temp_r0_6 = 0x78 - *(s16 *)((u8 *)var_r4 + 0x42E);
                if (*(u8 *)((u8 *)var_r4 + 0x430) == 0) {
                    if ((u32)temp_r0_6 > 0x28U) {
                        *(u8 *)((u8 *)var_r4 + 0x430) = temp_r0_6;
                        ov96_021E8228(sp4, (u8)sp24, (u8)sp28, 1, 1);
                    }
                } else {
                    *(u8 *)((u8 *)var_r4 + 0x430) += temp_r0_6;
                    ov96_021E8228(sp4, (u8)sp24, (u8)sp28, 1, 1);
                }
            }
        }
    }
    var_r5 = (void *)((u8 *)var_r5 + (0x48));
    sp14 = (void *)((u8 *)sp14 + (0x48));
    var_r6 = (void *)((u8 *)var_r6 + (0x20));
    sp10 = (void *)((u8 *)sp10 + (0x48));
    temp_r0_7 = sp1C + 1;
    sp1C = temp_r0_7;
    if (temp_r0_7 < 0xC) {
        goto loop_2;
    }
    var_r4 = (void *)((u8 *)var_r4 + (0x20));
    sp18 = (void *)((u8 *)sp18 + (0xC));
    temp_r0_8 = sp20 + 1;
    sp20 = temp_r0_8;
    if (temp_r0_8 < 0xC) {
        goto loop_1;
    }
}
#endif
