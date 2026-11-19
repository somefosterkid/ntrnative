#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "overlay_96_app_021ede64.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ECC4C(Ov96PlayerSlot *, u8);

u32 ov96_021EE264(Ov96RankEntry *, s32);

void ov96_021EDE64(PokeathlonCourseData *arg0, Ov96PlayerSlot *arg1);

void ov96_021EDE64(PokeathlonCourseData *arg0, Ov96PlayerSlot *arg1) {
    s32 sp4;
    Ov96App_021ED1D0 *sp0;
    u32 var_r4;
    s32 temp_r0;
    s32 temp_r0_3;
    s32 var_r5;
    s32 temp_r0_5;
    PokeathlonFieldData *temp_r7;
    Ov96App_021ED1D0 *temp_r0_2;

    temp_r0 = ov96_021E5F24(arg0);
    sp4 = temp_r0;
    temp_r0_2 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    sp0 = temp_r0_2;
    temp_r7 = PokeathlonCourse_GetFieldData(arg0);
    temp_r0_3 = temp_r0_2->rank[0].idx;
    if (temp_r0_3 == temp_r0) {
        ((Ov96App_021EDE64 *)temp_r7)->unk_1D0 = (u16)(1 | (((Ov96App_021EDE64 *)temp_r7)->unk_1D0 & ~1));
    }
    var_r5 = 0x3E7;
    var_r4 = 0;
    do {
        temp_r0_3 = ov96_021ECC4C(arg1, var_r4);
        if (var_r5 >= temp_r0_3) {
            var_r5 = temp_r0_3;
        }
        var_r4 += 1;
    } while ((s32)var_r4 < 4);
    temp_r0_3 = ov96_021ECC4C(arg1, (u8)sp4);
    if (temp_r0_3 == var_r5) {
        ((Ov96App_021EDE64 *)temp_r7)->unk_1D0 = (u16)(2 | ((Ov96App_021EDE64 *)temp_r7)->unk_1D0);
    }
    ((Ov96App_021EDE64 *)temp_r7)->unk_180 = (s32)sp0->revealCount[sp4];
    ((Ov96App_021EDE64 *)temp_r7)->unk_1D2 = (s16)ov96_021ECC4C(sp0->playerSlots, (u8)sp4);
    temp_r0_5 = ov96_021EE264(sp0->rank, (u8)sp4);
    if (temp_r0_5 == -1) {
        GF_AssertFail();
    }
    ((Ov96App_021EDE64 *)temp_r7)->unk_1D0 = (u16)((((Ov96App_021EDE64 *)temp_r7)->unk_1D0 & ~0xC) | ((u32)((u16)temp_r0_5 << 0x1E) >> 0x1C));
}
