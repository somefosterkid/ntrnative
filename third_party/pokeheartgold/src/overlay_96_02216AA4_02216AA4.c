#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"


void ov96_02216770(Ov96RdSlot *slot, s16 *pos, u32 *digits, u32 idx, void *flags, s32 param_6, s32 param_7);

void ov96_02216934(Ov96RdSlot *slot, s16 *pos, void *digits, u32 idx, void *flags, s32 param_6, s32 param_7);

void ov96_02216A54(Ov96RdSlot *slot, s16 *pos, void *flags, s32 param_4, s32 param_5);

void ov96_0221996C(void *digits, u32 idx, u32 param_3, s32 param_4);

typedef struct BF_ov96_02216AA4_0 {
    u8 pad0 : 5;
    u8 f5_2 : 2;
} BF_ov96_02216AA4_0;


void ov96_02216AA4(void *arg0, Ov96RdSlot *slot, void *arg2, PokeathlonCourseData *arg3, u32 arg4);

void ov96_02216AA4(void *arg0, Ov96RdSlot *slot, void *arg2, PokeathlonCourseData *arg3, u32 arg4) {
    s32 sp24;
    u8 *sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    PokeathlonCourseData *sp10;
    void *spC;
    s32 var_r6;
    s32 var_r0;
    s32 both;
    u32 temp_r0;
    u8 *temp_r6;

    sp18 = 0;
    spC = arg2;
    sp10 = arg3;
    temp_r6 = PokeathlonCourse_GetDataCopyArea(arg3);
    *(s16 *)(&sp24) = (s16) * (u8 *)arg0;
    *(s16 *)((u8 *)(&sp24) + 2) = (s16) * (u8 *)((u8 *)arg0 + 1);
    if ((u32)PokeathlonCourse_GetParticipantCount(sp10) <= arg4) {
        sp1C = 1;
        sp20 = ov96_021E8A20(&(temp_r6 + 0x50)[0x28 * arg4]);
    } else {
        sp1C = 0;
        sp20 = ov96_021E8A20(temp_r6);
    }
    if (ov96_021E5F24(sp10) == 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    if ((var_r0 != 0) && (sp1C != 0)) {
        sp14 = 1;
    } else {
        sp14 = 0;
    }
    if (arg4 == ov96_021E5F24(sp10)) {
        var_r6 = 1;
    } else {
        var_r6 = 0;
    }
    both = sp14 | var_r6;
    if ((((BF_ov96_02216AA4_0 *)((u8 *)arg0 + 5))->f5_2) != slot->mode) {
        slot->state = 0;
        slot->mode = ((BF_ov96_02216AA4_0 *)((u8 *)arg0 + 5))->f5_2;
        sp18 = 1;
    }
    temp_r0 = slot->mode;
    switch (temp_r0) {
    case 1:
        if (sp18 != 0) {
            ov96_0221996C(spC, arg4, (u32)(*(u8 *)((u8 *)arg0 + 4) << 0x18) >> 0x1E, 1);
        }
        ov96_02216770(slot, (s16 *)&sp24, spC, arg4, sp20, sp14, var_r6);
        return;
    case 2:
        ov96_02216934(slot, (s16 *)&sp24, spC, arg4, sp20, both, var_r6);
        return;
    case 0:
        if (sp18 != 0) {
            ov96_02216A54(slot, (s16 *)&sp24, sp20, both, var_r6);
            ov96_0221996C(spC, arg4, (u32)(*(u8 *)((u8 *)arg0 + 4) << 0x18) >> 0x1E, 0);
        }
        return;
    }
}
