#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "filesystem.h"

u32 ov96_021E679C(s32, s32);

s32 ov96_021ED954(PokeathlonCourseData *arg0, s32 arg1);

s32 ov96_021ED954(PokeathlonCourseData *arg0, s32 arg1) {
    s32 sp4[5];
    NARC *sp0;
    s32 var_r6;
    s32 var_r7;
    u8 *var_r5;

    var_r6 = 0x4B;
    sp0 = NARC_New(NARC_poketool_personal_performance, PokeathlonCourse_GetHeapID(arg0));
    var_r5 = PokeathlonCourse_GetParticipantUnk04(arg0, arg1);
    var_r7 = 0;
    do {
        NARC_ReadWholeMember(sp0, ov96_021E679C(*(u16 *)var_r5, *(u16 *)((u8 *)var_r5 + 2)), sp4);
        var_r5 += 0x28;
        var_r6 = ((((var_r6 - (*(u8 *)(sp4) + 1)) - (*(u8 *)((u8 *)(sp4) + 1) + 1)) - (*(u8 *)((u8 *)(sp4) + 2) + 1)) - (*(u8 *)((u8 *)(sp4) + 3) + 1)) - (*(u8 *)((u8 *)(sp4) + 4) + 1);
        var_r7 += 1;
    } while (var_r7 < 3);
    if (var_r6 > 0x4B) {
        var_r6 = 0x4B;
    } else if (var_r6 < 0) {
        var_r6 = 0;
    }
    NARC_Delete(sp0);
    return var_r6;
}
