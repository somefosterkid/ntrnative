#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "unk_02005D10.h"
#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_ov96_rower_work.h"

s32 ov96_021E6454(PokeathlonCourseData *, s32);

s32 ov96_021EB570(void *, s32);

s32 ov96_021FE550(PokeathlonCourseData *, s32, void *, u8 *);

s32 ov96_021FEAEC(void *, s32);

s32 ov96_021FFBD8(BgConfig *, s32);

s32 ov96_021FFC34(void *, s32);

void ov96_021FDC7C(PokeathlonCourseData *arg0);

void ov96_021FDC7C(PokeathlonCourseData *arg0) {
    u8 *temp_r0;
    s32 sp4;
    u16 *var_r6;
    s32 temp_r5;
    u16 var_r2;
    PokeathlonTally *temp_r7;
    u8 temp_r0_2;
    u8 temp_r6;
    u32 var_r5;
    Ov96RowerWork *temp_r4;
    temp_r4 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    temp_r0 = PokeathlonCourse_GetDataCopyArea(arg0);
    temp_r7 = ov96_021E8A20(temp_r0 + 0xF0);
    sp4 = (s32)((u8)ov96_021E5F24(arg0));
    var_r5 = 0;
    var_r6 = temp_r7->u16s;
    do {
        ov96_0220050C(temp_r4->unk3E0, var_r5, *var_r6);
        var_r6 += 1;
        var_r5 += 1;
    } while (((s32)var_r5) < 4);
    temp_r5 = sp4 * 2;
    ScheduleSetBgPosText(temp_r4->bgConfig, 0U, BG_POS_OP_SET_ROT, (s32)(temp_r7->u16s[sp4]));
    ScheduleSetBgPosText(temp_r4->bgConfig, 1U, BG_POS_OP_SET_ROT, (s32)(((u32)(temp_r7->u16s[sp4])) >> 1));
    ScheduleSetBgPosText(temp_r4->bgConfig, 2U, BG_POS_OP_SET_ROT, (temp_r7->u16s[sp4]) * 2);
    ov96_021FE550(arg0, sp4, temp_r4->rowers, temp_r0);
    ov96_021FEAEC(temp_r4, temp_r7->u16s[sp4]);
    temp_r6 = 0x7F & ((&temp_r7->u8s[sp4])[0x10]);
    if (((u8)((((s32)((u8)(0xF & (((s32)(temp_r7->pairs[2].word1)) >> 0x1A)))) >> sp4) & 1)) != 0) {
        var_r2 = 0;
    } else {
        var_r2 = temp_r7->u16s[sp4];
    }
    ov96_022005B4(temp_r4->unk3E0, temp_r6, var_r2);
    if (((u8)(1 & (((s32)(temp_r7->pairs[2].word1)) >> 0x18))) == 0) {
        ov96_021FFC34(&temp_r4->score, temp_r6);
    }
    ov96_02200A64(temp_r4->unk3E0, temp_r7->u16s[12]);
    temp_r0_2 = temp_r7->u8s[sp4 + 0x1E];
    switch (temp_r0_2) {
    case 0:
        temp_r4->unk642 = 0U;
        ov96_021EB570(temp_r4->unk1C, 0xB);
        break;

    case 3:
        temp_r4->unk642 = 0U;
        ov96_021EB570(temp_r4->unk1C, 0xD);
        break;

    case 2:
        if ((temp_r4->unk642) == 0) {
            temp_r4->unk642 = 1U;
            PlaySE(0x89BU);
        }
        ov96_021EB570(temp_r4->unk1C, 0xC);
        break;

    case 4:
        if ((temp_r4->unk642) == 0) {
            temp_r4->unk642 = 1U;
            PlaySE(0x89CU);
        }
        ov96_021EB570(temp_r4->unk1C, 0xE);
        break;

    case 1:
        temp_r4->unk642 = 0U;
        ov96_021EB570(temp_r4->unk1C, 0xF);
        break;
    }

    ov96_021FFBD8(temp_r4->bgConfig, temp_r7->u16s[sp4]);
    ov96_021E6454(arg0, temp_r7->u16s[12]);
}
