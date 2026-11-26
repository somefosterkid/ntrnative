#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "system.h"
#include "touchscreen.h"

#include "overlay_96_app_02210C98.h"


void ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

s32 ov96_021E8324(PokeathlonCourseData *, s32 *);

s32 ov96_021EB144(void *, s32);

s32 ov96_021EB63C(void *arg0, s32 arg1);

s32 ov96_02211F38(PokeathlonCourseData *);

extern s32 ov96_02214618;

s32 ov96_02211AF0(PokeathlonCourseData *arg0);

s32 ov96_02211AF0(PokeathlonCourseData *arg0) {
    u8 *var_r7;
    Ov96App_02210C98 *var_r6;
    PokeathlonCourseData *var_r5;
    PokeathlonTouchRecord *var_r4;
    PokeathlonTally *tally;
    u8 *temp_r0;
    s32 temp_r1;
    u8 var_flag;

    var_r5 = arg0;
    var_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(var_r5);
    var_r7 = temp_r0;
    var_r4 = ov96_021E8A20(temp_r0);
    var_r7 += 0xF0;
    var_r4->touching = 0;
    tally = ov96_021E8A20(var_r7);
    temp_r1 = tally->pairs[4].word0;
    var_flag = 1;
    var_flag &= temp_r1 >> 0x18;
    if (var_flag) {

        ov96_021EB63C(var_r6->unk_0, 1);
        ov96_021EB144(var_r6->unk_748, 1);
        ov96_021E65A4(var_r5);
        ov96_021E8324(var_r5, &ov96_02214618);
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(var_r5, (u8)ov96_021E5F24(var_r5), 3, 0, 1);
    }
    if (System_GetTouchHeld() != 0) {
        var_r4->touchX = (s8)gSystem.touchX;
        var_r4->touchY = (s8)gSystem.touchY;
        var_r4->touching = 1;
    }
    ov96_02211F38(var_r5);
    return 0;
}

s32 ov96_02211DE4(void *, u8 *);

s32 ov96_022124F8(PokeathlonCourseData *);

s32 ov96_022127F4(PokeathlonCourseData *);

s32 ov96_02212B94(PokeathlonCourseData *);

s32 ov96_022130EC(PokeathlonCourseData *);

s32 ov96_022132FC(PokeathlonCourseData *);

void ov96_02213354(s32 x, s32 y, VecFx32 *out);

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov96_02213364(void *, s16, s16, s16);
#else
s8 ov96_02213364(void *, u8, u8, u8);
#endif

void ov96_02214B84(PokeathlonCourseData *data, Ov96App_02210C98_Cpu *cpu);

typedef struct Words9 {
    u32 w[9];
} Words9;

void ov96_02211B94(PokeathlonCourseData *arg0);

void ov96_02211B94(PokeathlonCourseData *arg0) {
    u8 *sp10;
    u8 *spC;
    Ov96App_02210C98_Unit *sp8;
    PokeathlonCourseData *sp4;
    s32 temp_r0_6;
    s32 temp_r1;
    s32 temp_r1_3;
    s32 temp_r1_5;
    s8 temp_r0_5;
    u8 *temp_r0_2;
    u8 *temp_r0_4;
    u8 *var_r3;
    u8 *var_r5;
    u8 temp_r0_8;
    s32 var_r6;
    Ov96App_02210C98 *temp_r0;
    Ov96App_02210C98_Unit *temp_r0_9;
    Ov96App_02210C98_Unit *temp_r1_4;

    sp4 = arg0;
    spC = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r1 = temp_r0->unk_738;
    if (temp_r1 > 0) {
        temp_r0->unk_738 = (s32)(temp_r1 - 1);
    }
    if (ov96_021E5F24(sp4) != 0) {
        return;
    }
    if (temp_r0->unk_73E != 0) {
        temp_r0_2 = spC + 0x28;
        spC = temp_r0_2;
        ov96_02211DE4(temp_r0, ov96_021E8A20(temp_r0_2));
        return;
    }
    sp10 = ov96_021E8A20(spC + 0x28);
    var_r5 = ov96_021E8A20(spC + 0x50);
    var_r3 = ov96_021E8A20(spC);
    *(Words9 *)var_r5 = *(Words9 *)var_r3;
    var_r6 = 0;
    spC += 0x50;
    sp8 = temp_r0->unk_5C[0];
loop_9:
    temp_r0_4 = ov96_021E8A20(spC);
    if (*(s32 *)temp_r0_4 != 0) {
        temp_r1_3 = temp_r0->unk_704[var_r6].unk_0;
        if ((temp_r1_3 != 0) && (temp_r0->unk_704[var_r6].unk_4 != 0)) {
            temp_r0->unk_704[var_r6].unk_0 = 0;
        } else if ((temp_r1_3 == 0) && (temp_r0->unk_704[var_r6].unk_4 == 0)) {
            temp_r0->unk_704[var_r6].unk_0 = 1;
            temp_r0->unk_704[var_r6].unk_4 = 1;
        }
    } else {
        temp_r0->unk_704[var_r6].unk_0 = 0;
        temp_r0->unk_704[var_r6].unk_4 = 0;
    }
    if (temp_r0->unk_704[var_r6].unk_0 != 0) {
        temp_r0_5 = ov96_02213364(temp_r0, (u8)var_r6, *(u8 *)((u8 *)temp_r0_4 + 4), *(u8 *)((u8 *)temp_r0_4 + 5));
        if (temp_r0_5 != 0xC) {
            temp_r1_4 = &sp8[temp_r0_5 % 3];
            temp_r0_6 = temp_r1_4->unk_78;
            if ((temp_r0_6 != 2) && (temp_r0_6 != 1) && (temp_r1_4->unk_48 == 0)) {
                temp_r0->unk_734[var_r6] = temp_r0_5;
                temp_r0->unk_742 = 0U;
            }
        }
    } else if (temp_r0->unk_704[var_r6].unk_4 != 0) {
        if (temp_r0->unk_734[var_r6] != 0xC) {
            if ((u32)temp_r0->unk_742 < 0x14U) {
                temp_r0->unk_742 += 1;
            }
            ov96_02213354(*(u8 *)((u8 *)temp_r0_4 + 4), *(u8 *)((u8 *)temp_r0_4 + 5), &sp8[(u8)((s32)temp_r0->unk_734[var_r6] % 3)].unk_24);
        }
    } else {
        temp_r0_8 = temp_r0->unk_734[var_r6];
        if (temp_r0_8 != 0xC) {
            temp_r0_9 = &sp8[(u8)((s32)temp_r0_8 % 3)];
            temp_r1_5 = temp_r0_9->unk_78;
            if ((temp_r1_5 != 3) && (temp_r1_5 != 2)) {
                if ((u32)temp_r0->unk_742 <= 8U) {
                    if ((temp_r0_9->unk_24.x != temp_r0_9->unk_30.x) || (temp_r0_9->unk_24.y != temp_r0_9->unk_30.y)) {
                        temp_r0_9->unk_78 = 1;
                        ov96_021E8228(sp4, (u8)var_r6, (u8)((s32)temp_r0->unk_734[var_r6] % 3), 6, 1);
                    }
                } else {
                    temp_r0_9->unk_78 = 0;
                }
            }
            temp_r0->unk_742 = 0U;
            temp_r0->unk_734[var_r6] = 0xCU;
        }
    }
    spC += 0x28;
    var_r6 += 1;
    sp8 += 3;
    if ((s32)var_r6 < 4) {
        goto loop_9;
    }
    ov96_02214B84(sp4, temp_r0->unk_74C);
    ov96_022124F8(sp4);
    ov96_02212B94(sp4);
    ov96_022127F4(sp4);
    ov96_022130EC(sp4);
    ov96_022132FC(sp4);
    if (temp_r0->unk_738 <= 0) {
        temp_r0->unk_73E = 1;
    } else {
        temp_r0->unk_73E = 0;
    }
    ov96_02211DE4(temp_r0, sp10);
}
