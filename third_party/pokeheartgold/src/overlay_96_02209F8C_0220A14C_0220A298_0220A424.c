#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_ov96_0220_data.h"

u16 ov96_0220AD4C(void *arg0);

u32 ov96_0220B730(void *arg0);

void *ov96_0220B758(void *arg0);

u32 ov96_0220B774(void *arg0);

u8 ov96_0220B7B4(void *arg0);

s32 ov96_0220B79C(void *);

void ov96_0220A424(PokeathlonCourseData *arg0);

void ov96_0220A424(PokeathlonCourseData *arg0) {
    s32 *temp_r6;
    s32 temp_r1;
    s32 var_r2;
    u32 temp_r7;
    Ov96_0220Data *temp_r4;
    u8 *temp_r5;

    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r5 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0));
    temp_r6 = ov96_0220B758(temp_r4->unk_4C);
    temp_r7 = ov96_0220B730(temp_r4->unk_4C);
    temp_r1 = (*(u32 *)temp_r5 & ~0xFF) | (u8)ov96_0220AD4C(temp_r4->unk_40);
    *(u32 *)temp_r5 = temp_r1;
    *(u32 *)temp_r5 = (s32)((0xFFFF00FF & temp_r1) | (u16)(((u32)(*temp_r6 << 6) >> 0x18) << 8));
    *(u32 *)temp_r5 = (s32)(((u32)(ov96_0220B79C(temp_r4->unk_4C) << 0x18) >> 8) | (0xFF00FFFF & *(u32 *)temp_r5));
    *(u32 *)temp_r5 = (s32)(((u32)(ov96_0220B774(temp_r4->unk_4C) << 0x1F) >> 5) | (0xFBFFFFFF & *(u32 *)temp_r5));
    if (temp_r7 == 3) {
        var_r2 = 1;
    } else {
        var_r2 = 0;
    }
    *(u32 *)temp_r5 = (s32)((*(u32 *)temp_r5 & 0xEFFFFFFF) | ((u32)(var_r2 << 0x1F) >> 3));
    *(u32 *)temp_r5 = (s32)(((u32)(ov96_0220B7B4(temp_r4->unk_4C) << 0x1E) >> 6) | (0xFCFFFFFF & *(u32 *)temp_r5));
}
