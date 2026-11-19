#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

typedef struct PokeathlonCopyBlock {
    u32 words[9];
} PokeathlonCopyBlock;

s32 ov96_0220F8C8(UnkStruct_ov96_0220E3F0_Entry *, PokeathlonCourseData *);

s32 ov96_0220FA18(u8 (*)[12], PokeathlonCourseData *);

s32 ov96_02210030(UnkStruct_ov96_0220E3F0_Score *, u8 *, s32);

s32 ov96_02210858(void *);

void ov96_0220F03C(PokeathlonCourseData *arg0);

// Entry.unk_4 inside the entry loop. The DS code only matches when every access goes through a byte offset
// (new_var2 == 4 == offsetof(UnkStruct_ov96_0220E3F0_Entry, unk_4)); PC uses the field.
#if defined(SDK_PORT) || defined(NONMATCHING)
#define ENTRY_BITS(e, T, off) ((e)->unk_4.raw)
#else
#define ENTRY_BITS(e, T, off) (*(T *)((u8 *)(e) + (off)))
#endif

void ov96_0220F03C(PokeathlonCourseData *arg0) {
    u8 *spC;
    s32 sp8;
    u8 *sp4;
    PokeathlonCourseData *sp0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r1_3;
    s32 temp_r1_5;
    s32 var_r6;
    u8 *temp_r0;
    int new_var;
    u32 new_var3;
    u8 *temp_r0_4;
    u8 *var_r3;
    unsigned char new_var2;
    u8 *var_r4;
    u8 *var_r4_2;
    UnkStruct_ov96_0220E3F0 *temp_r7;
    UnkStruct_ov96_0220E3F0_Entry *var_r5;
    int new_var4;
    sp0 = arg0;
    temp_r7 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    spC = PokeathlonCourse_GetDataCopyArea(arg0);
    if (ov96_021E5F24(arg0) != 0) {
        return;
    }
    new_var = 1;
    sp8 = 0;
    temp_r0 = ov96_021E8A20(spC + 0x28);
    new_var2 = 4;
    sp4 = temp_r0;
    if ((((u32)(*((u32 *)(((u8 *)temp_r0) + 0x20)))) >> 0x1F) != 0) {
        ov96_0220FA18(temp_r7->hitFlags, sp0);
        return;
    }
    temp_r1 = temp_r7->timer;
    if (temp_r1 > 0) {
        temp_r7->timer = (s32)(temp_r1 - new_var);
        temp_r0_2 = (temp_r7->timer + 0x1E) / 30;
        sp8 = temp_r0_2;
        *((s32 *)(((u8 *)sp4) + 0x1C)) = (s32)(((*((s32 *)(((u8 *)sp4) + 0x1C))) & 0xC0FFFFFF) | (((u32)(temp_r0_2 << 0x1A)) >> 2));
    }
    var_r4 = ov96_021E8A20(spC + 0x50);
    var_r3 = ov96_021E8A20(spC);
    *((PokeathlonCopyBlock *)var_r4) = *((PokeathlonCopyBlock *)var_r3);
    var_r6 = 0;
    var_r4_2 = spC + 0x50;
    var_r5 = temp_r7->entries;
    do {
        temp_r0_4 = ov96_021E8A20(var_r4_2);
        temp_r1_3 = ENTRY_BITS(var_r5, s32, 4);
        if ((((u32)((*((u32 *)temp_r0_4)) << 0xF)) >> 0x1F) != 0) {
            u32 temp_r3;
            temp_r3 = ((u32)(temp_r1_3 << 0x11)) >> 0x1F;
            if ((temp_r3 != 0) && ((((u32)(temp_r1_3 << 0x10)) >> 0x1F) != 0)) {
                ENTRY_BITS(var_r5, s32, new_var2) = (s32)(0xFFFFBFFF & temp_r1_3);
            } else if ((new_var3 = temp_r3) == 0) {
                if ((((u32)((ENTRY_BITS(var_r5, u32, 4)) << 0x10)) >> 0x1F) == 0) {
                    new_var4 = (ENTRY_BITS(var_r5, u32, new_var2)) | 0x4000;
                    ENTRY_BITS(var_r5, s32, 4) = (s32)(0x8000 | new_var4);
                }
            }
            var_r5->unk_0 = (s16)((u8)(*((u32 *)temp_r0_4)));
            var_r5->unk_2 = (s16)(((u32)((*((u32 *)temp_r0_4)) << 0x10)) >> 0x18);
            temp_r1_5 = ENTRY_BITS(var_r5, s32, new_var2);
            ENTRY_BITS(var_r5, s32, new_var2) = (s32)((0xFFFFC000 & temp_r1_5) | (0x3FFF & ((((u32)(temp_r1_5 << 0x12)) >> 0x12) + new_var)));
        } else {
            temp_r1_5 = (0xE000FFFF & temp_r1_3) | (((u32)((((u32)(temp_r1_3 << 0x12)) >> 0x12) << 0x13)) >> 3);
            temp_r1_5 = temp_r1_5 & 0xFFFFC000;
            temp_r1_5 &= ~0x4000;
            ENTRY_BITS(var_r5, s32, new_var2) = (s32)(0xFFFF7FFF & temp_r1_5);
        }
        var_r6 += new_var;
        var_r4_2 += 0x28;
        var_r5++;
    } while (var_r6 < new_var2);
    ov96_02210858(temp_r7->unk_C8);
    ov96_02210030(&temp_r7->score, spC, sp8);
    ov96_0220F8C8(temp_r7->entries, sp0);
    if (temp_r7->timer <= 0) {
        *((s32 *)(((u8 *)sp4) + 0x20)) = (s32)(((*((s32 *)(((u8 *)sp4) + 0x20))) & 0x7FFFFFFF) | 0x80000000);
    }
    ov96_0220FA18(temp_r7->hitFlags, sp0);
}
