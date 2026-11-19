#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

#include "system.h"
#include "touchscreen.h"

void ov96_0220EAC4(UnkStruct_ov96_0220E3F0 *arg0);

s32 ov96_021E65A4(PokeathlonCourseData *);

s32 ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

s32 ov96_0220E2A8(PokeathlonCourseData *);

s32 ov96_0221013C(UnkStruct_ov96_0220E3F0_Hud *, PokeathlonCourseData *);

s32 ov96_0220F1CC(PokeathlonCourseData *arg0);

s32 ov96_0220F1CC(PokeathlonCourseData *arg0) {
    s32 temp_r1;
    s32 held;
    s32 temp_r2;
    s32 var_r0;
    u8 *temp_r0;
    u8 *temp_r4;
    UnkStruct_ov96_0220E3F0 *temp_r6;
    void *temp_r7;
    temp_r6 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r7 = ov96_021E8A20(temp_r0 + 0xF0);
    temp_r4 = ov96_021E8A20(temp_r0);
    if ((((u32)(*((u32 *)(((u8 *)temp_r7) + 0x20)))) >> 0x1F) == 1) {
        ov96_0220EAC4(temp_r6);
        ov96_021E65A4(arg0);
        ov96_0220E2A8(arg0);
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(arg0, (u8)ov96_021E5F24(arg0), 3, 0, 1);
    }
    held = System_GetTouchHeld();
    if (held != 0) {
        temp_r1 = *((u32 *)temp_r4);
        temp_r2 = temp_r1 & (~0xFF);
        temp_r2 = ((u8)gSystem.touchX) | temp_r2;
        *((u32 *)temp_r4) = temp_r2;
        var_r0 = 0x10000 | ((0xFFFF00FF & temp_r2) | ((u16)(gSystem.touchY << 8)));
    } else {
        temp_r1 = *((u32 *)temp_r4);
        var_r0 = 0xFFFEFFFF & temp_r1;
    }
    *((u32 *)temp_r4) = var_r0;
    ov96_0221013C(&temp_r6->hud, arg0);
    return 0;
}
