#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "screen_fade.h"
#include "overlay_96_hurdle_cpu.h"
#include "overlay_96_ov96hurdle_work.h"

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_02205D30(PokeathlonCourseData *);

s32 ov96_0220764C(void *);

s32 ov96_02207870(void *);



BOOL ov96_022052B0(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_022052B0(PokeathlonCourseData *arg0, u8 *arg1) {
    s32 sp5C[10];
    s32 sp34[10];
    s32 sp20[5];
    s32 sp1C;
    Ov96HurdleWork *sp18;
    u8 *sp14;
    u8 *sp10;
    PokeathlonCourseData *spC;
    s32 *var_r6;
    s32 *var_r4;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    void *var_r2;
#endif
    s32 var_r3;
    s32 var_r7_2;
    u16 temp_r0_3;
    u16 temp_r0_4;
    u16 temp_r1;
    u16 temp_r1_2;
    u8 temp_r0;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    void *var_r4_2;
    void *var_r5;
    void *var_r5_2;
    void *var_r7;
#endif

    sp10 = arg1;
    spC = arg0;
    sp18 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = *sp10;
    switch (temp_r0) {
    case 0:
        ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(spC) + 0xF0);
        ReadWholeNarcMemberByIdPair(sp20, NARC_a_2_2_5, sp18->unk_378);
#if defined(SDK_PORT) || defined(NONMATCHING)
        sp14 = (u8 *)sp5C;
        var_r6 = sp20;
        sp1C = 0;
        var_r4 = sp34;
        do {
            temp_r1 = *(u8 *)((u8 *)var_r4 + 1) * 8;
            temp_r0_3 = *(u8 *)var_r4 * 8;
            sp18->points_61C[sp1C].x = temp_r0_3;
            sp18->points_61C[sp1C].y = (s16)(*(u8 *)((u8 *)var_r4 + 1) * 8);
            if ((temp_r0_3 != 0) && (temp_r1 != 0)) {
                sp18->cells[sp1C].active = 1;
                sp18->cells[sp1C].x = temp_r0_3;
                sp18->cells[sp1C].y = temp_r1;
                ov96_021EB52C(sp18->cells[sp1C].sprite, 1, 1);
            }
            var_r4 = (void *)((u8 *)var_r4 + (2));
            sp1C += 1;
        } while (sp1C < 0x14);
        var_r7_2 = 0;
        do {
            temp_r1_2 = *(u8 *)((u8 *)var_r6 + 1) * 8;
            temp_r0_4 = *(u8 *)var_r6 * 8;
            sp18->points_66C[var_r7_2].x = temp_r0_4;
            sp18->points_66C[var_r7_2].y = (s16)(*(u8 *)((u8 *)var_r6 + 1) * 8);
            if ((temp_r0_4 != 0) && (temp_r1_2 != 0)) {
                sp18->pairs[var_r7_2].active = 1;
                sp18->pairs[var_r7_2].x = temp_r0_4;
                sp18->pairs[var_r7_2].y = temp_r1_2;
                ov96_021EB52C(sp18->pairs[var_r7_2].sprites[0], 1, 1);
                ov96_021EB52C(sp18->pairs[var_r7_2].sprites[1], 1, 1);
            }
            var_r7_2 += 1;
            var_r6 = (void *)((u8 *)var_r6 + (2));
        } while (var_r7_2 < 0xA);
        var_r3 = 0;
        do {
            sp18->points_5C8[var_r3].x = (s16)(*(u8 *)sp14 * 8);
            sp18->points_5C8[var_r3].y = (s16)(*(u8 *)((u8 *)sp14 + 1) * 8);
            var_r3 += 1;
            sp14 = (void *)((u8 *)sp14 + (2));
        } while (var_r3 < 0x14);
#else
        sp14 = (u8 *)sp5C;
        var_r7 = sp18;
        var_r6 = sp20;
        sp1C = 0;
        var_r4 = sp34;
        var_r5 = var_r7;
        do {
            temp_r1 = *(u8 *)((u8 *)var_r4 + 1) * 8;
            temp_r0_3 = *(u8 *)var_r4 * 8;
            *(u16 *)((u8 *)var_r7 + 0x61C) = temp_r0_3;
            *(s16 *)((u8 *)var_r7 + 0x61E) = (s16)(*(u8 *)((u8 *)var_r4 + 1) * 8);
            if ((temp_r0_3 != 0) && (temp_r1 != 0)) {
                *(s32 *)((u8 *)var_r5 + 0x37C) = 1;
                *(u16 *)((u8 *)var_r5 + 0x384) = temp_r0_3;
                *(u16 *)((u8 *)var_r5 + 0x386) = temp_r1;
                ov96_021EB52C(*(void **)((u8 *)var_r5 + 0x380), 1, 1);
            }
            var_r4 = (void *)((u8 *)var_r4 + (2));
            var_r7 += 4;
            var_r5 += 0xC;
            sp1C += 1;
        } while (sp1C < 0x14);
        var_r5_2 = sp18;
        var_r7_2 = 0;
        var_r4_2 = var_r5_2;
        do {
            temp_r1_2 = *(u8 *)((u8 *)var_r6 + 1) * 8;
            temp_r0_4 = *(u8 *)var_r6 * 8;
            *(u16 *)((u8 *)var_r5_2 + 0x66C) = temp_r0_4;
            *(s16 *)((u8 *)var_r5_2 + 0x66E) = (s16)(*(u8 *)((u8 *)var_r6 + 1) * 8);
            if ((temp_r0_4 != 0) && (temp_r1_2 != 0)) {
                *(s32 *)((u8 *)var_r4_2 + 0x46C) = 1;
                *(u16 *)((u8 *)var_r4_2 + 0x478) = temp_r0_4;
                *(u16 *)((u8 *)var_r4_2 + 0x47A) = temp_r1_2;
                ov96_021EB52C(*(void **)((u8 *)var_r4_2 + 0x470), 1, 1);
                ov96_021EB52C(*(void **)((u8 *)var_r4_2 + 0x474), 1, 1);
            }
            var_r7_2 += 1;
            var_r6 = (void *)((u8 *)var_r6 + (2));
            var_r5_2 += 4;
            var_r4_2 += 0x10;
        } while (var_r7_2 < 0xA);
        var_r2 = sp18;
        var_r3 = 0;
        do {
            var_r3 += 1;
            *(s16 *)((u8 *)var_r2 + 0x5C8) = (s16)(*(u8 *)sp14 * 8);
            *(s16 *)((u8 *)var_r2 + 0x5CA) = (s16)(*(u8 *)((u8 *)sp14 + 1) * 8);
            var_r2 += 4;
            sp14 = (void *)((u8 *)sp14 + (2));
        } while (var_r3 < 0x14);
#endif
        if (ov96_021E5F24(spC) == 0) {
            ov96_02207870(sp18);
            ov96_02208AF8(sp18->cpu, sp34);
#if defined(SDK_PORT) || defined(NONMATCHING)
            {
                // the DS reads 20 byte pairs from sp20 (10 pairs): its own, then the first 10 of sp34 behind it on the stack
                u8 pairs[40];

                MI_CpuCopy8(sp20, pairs, sizeof(sp20));
                MI_CpuCopy8(sp34, pairs + sizeof(sp20), sizeof(pairs) - sizeof(sp20));
                ov96_02208AF8(sp18->cpu, pairs);
            }
#else
            ov96_02208AF8(sp18->cpu, sp20);
#endif
            ov96_02208AF0(sp18->cpu, sp18->slots);
        }
        ov96_0220764C(sp18);
        if (GF_heap_c_dummy_return_true(HEAP_ID_POKEATHLON) == 0) {
            GF_AssertFail();
        }
        *sp10 = *sp10 + 1;
        PokeathlonCourse_SetStateTransitionType(spC, 0x13U);
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, 0U, 6, 1, sp18->heapId);
        *sp10 = *sp10 + 1;
        break;
    case 2:
        if (IsPaletteFadeFinished() != 0) {
            PokeathlonCourse_SetStateField07(spC, 1U);
        }
        break;
    }
    ov96_02205D30(spC);
    return 0;
}
