#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "system.h"
#include "touchscreen.h"

#include "overlay_96_ov96_rd_work.h"

void ov96_02216200(PokeathlonCourseData *arg0);

void ov96_02217820(Ov96RdSprites *sprites);

void ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

s32 ov96_021E8324(PokeathlonCourseData *, void (*)(PokeathlonCourseData *));

s32 ov96_021EB144(void *, s32);

void ov96_0221964C(struct Ov96DigitWork *);

s32 ov96_02216234(PokeathlonCourseData *arg0);

s32 ov96_02216234(PokeathlonCourseData *arg0) {
    s32 temp_r2;
    s32 var_r0;
    u8 *temp_r0;
    u8 *temp_r5;
    Ov96RdWork *temp_r4;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r5 = ov96_021E8A20(temp_r0);
    if (((u32) * (u32 *)((u8 *)(ov96_021E8A20(temp_r0 + 0xF0)) + 0x20) >> 0x1F) == 1) {
        ov96_02217820(&temp_r4->sprites);
        ov96_021EB144(temp_r4->spriteSet, 1);
        ov96_0221964C(temp_r4->unk_180);
        ov96_021E8324(arg0, ov96_02216200);
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(arg0, (u8)ov96_021E5F24(arg0), 3, 0, 1);
    }
    if (System_GetTouchHeld() != 0) {
        temp_r2 = (*(u32 *)temp_r5 & ~0xFF) | (u8)gSystem.touchX;
        *(u32 *)temp_r5 = temp_r2;
        var_r0 = 0x10000 | ((temp_r2 & 0xFFFF00FF) | (u16)(gSystem.touchY << 8));
    } else {
        var_r0 = 0xFFFEFFFF & *(u32 *)temp_r5;
    }
    *(u32 *)temp_r5 = var_r0;
    return 0;
}
