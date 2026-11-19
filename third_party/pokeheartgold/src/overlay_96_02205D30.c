#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_96_ov96hurdle_work.h"
#include "overlay_96_sprite_res_man.h"

s32 ov96_021E6454(PokeathlonCourseData *, s32);

s32 ov96_02206380(PokeathlonCourseData *, s32, void *, u8 *);

s32 ov96_02206A24(void *, s32, s32);

s32 ov96_02206AC0(void *, s32, s32);

s32 ov96_02206B80(void *, u8 *, s32, s32);

s32 ov96_02207BE4(Ov96HurdleWork *);

s32 ov96_02208740(struct Ov96HurdleHud *, s32);

void ov96_02205D30(PokeathlonCourseData *arg0);

void ov96_021EB0A4(void *param_1, s32 param_2, s32 param_3, s32 *param_4, s32 *param_5);
u32 ov96_021EB0CC(Ov96SpriteSetElem *param_1, fx32 param_2, fx32 param_3, s32 param_4, s32 param_5);
u32 ov96_02205DD4(Ov96HurdleWork *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov96_02205D30(PokeathlonCourseData *data) {
    u8 *v1;
    s32 v0;
    PokeathlonCourseData *pokeathlonCourseData;
    s32 v8;
    s32 v7;
    u32 v4;
    u8 *v2;
    u8 *v5;
    u8 v3;
    Ov96HurdleWork *v6;

    pokeathlonCourseData = data;
    v6 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v2 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = v2;
    v5 = ov96_021E8A20(v2 + 0xF0);
    v3 = (u8)ov96_021E5F24(data);
    v0 = (s32)v3;
    v4 = *(u32 *)(v5 + (v3 * 4));
    v8 = v4 & 0x1FF;
    v7 = (v4 >> 9) & 0x1FF;
    ScheduleSetBgPosText(v6->bgConfig, 0U, BG_POS_OP_SET_ROT, v8 - 0x80);
    ScheduleSetBgPosText(v6->bgConfig, 0U, BG_POS_OP_SET_XSCALE, v7 - 0x60);
    ov96_02206380(data, v0, v6->runners, v1);
    ov96_02206A24(v6, v8, v7);
    ov96_02206AC0(v6, v8, v7);
    ov96_02206B80(v6, v5, v8, v7);
    ov96_02207BE4(v6);
    ov96_02208740(v6->hud, *(u16 *)((u8 *)v5 + 0x22));
    ov96_021E6454(data, *(u16 *)((u8 *)v5 + 0x22));
}

u32 ov96_02205DD4(Ov96HurdleWork *param_1, s32 param_2, s32 param_3, s32 param_4) {
    s32 iVar1;
    Ov96SpriteSetElem *elem;
    Ov96HurdleRunner *iVar1_p;
    s32 iStack_18;
    s32 iStack_1c;
    iVar1_p = &param_1->runners[param_2];
    if ((2 + 1) <= (iVar1_p->unk_B1)) {
        GF_AssertFail();
    }
    elem = (Ov96SpriteSetElem *)iVar1_p->unk_00[iVar1_p->unk_B1];
    ov96_021EB0A4(elem, 0x80, 0x60, &iStack_18, &iStack_1c);
    iVar1 = ov96_021EB0CC(elem, iStack_18 << 0xc, iStack_1c << 0xc, param_3, param_4);
    if (iVar1 != 0) {
        return 1;
    }
    return 0;
}
