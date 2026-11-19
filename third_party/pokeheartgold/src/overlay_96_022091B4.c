#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "touchscreen.h"
#include "screen_fade.h"
#include "overlay_96_ov96_0220_data.h"

s32 ov96_021E6454(PokeathlonCourseData *, s32);

s32 ov96_021E8228(PokeathlonCourseData *, u32, s32, s32, s32);

void ov96_021EB144(void *, s32);

s32 ov96_02209BB0(void *);

s32 ov96_02209BF8(void *);

s32 ov96_02209E70(void *);

s32 ov96_0220A298(void *, s32);

s32 ov96_0220A5DC(PokeathlonCourseData *, s32);

s32 ov96_0220A704(void *, s8, s8);

s32 ov96_0220A840(void *);

s32 ov96_0220A87C(void *);

s32 ov96_0220A8CC(void *, u32);

s32 ov96_0220A910(void *);

s32 ov96_0220AAEC(void *, s32, s16, s32, s32, s32 *);

s32 ov96_0220AD34(void *);

u16 ov96_0220AD4C(void *);

s32 ov96_0220B528(void *, s32);

s32 ov96_0220B634(void *, s32, s32);

void ov96_0220B708(u8 *, s32);

u32 *ov96_0220B758(void *);

s32 ov96_0220B774(void *);

u8 ov96_0220B7B4(void *);

s32 ov96_0220B7CC(void *);

s32 ov96_0220B8D8(u8 *);

s32 ov96_0220B8F0(void *);

s32 ov96_0220C9A0(void *);

s32 ov96_022091B4(PokeathlonCourseData *arg0);

s32 ov96_021E667C(PokeathlonCourseData *);
s32 ov96_022091B4(PokeathlonCourseData *);
s32 ov96_0220A14C(PokeathlonCourseData *);
s32 ov96_0220A4DC(PokeathlonCourseData *);
s32 ov96_0220935C(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_022091B4(PokeathlonCourseData *data) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;
    s32 v6;
    s32 v9;
    s32 v10;
    s32 *v8;
    PokeathlonRecord1 *v5;
    PokeathlonTally *tally;
    Ov96_0220Data *v7;

    v7 = PokeathlonCourse_GetHeapAllocPtr4(data);
    tally = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    if (((u32)(tally->pairs[2].word0 << 0xF) >> 0x1F) == 1) {
        ov96_0220A5DC(data, ov96_0220AD4C(v7->unk_40));
        ov96_02209BF8(v7);
        ov96_0220B708(v7->unk_4C, 0);
        ov96_0220B8F0(v7->unk_44);
        ov96_0220A87C(v7->unk_40);
        ov96_021EB144(v7->spriteSet, 1);
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(data, (u8)ov96_021E5F24(data), 3, 0, 1);
    }
    ov96_02209E70(v7);
    ov96_0220A840(v7->unk_40);
    v2 = ov96_0220AD34(v7->unk_40);
    if (v2 == 1) {
        v10 = 1;
    } else {
        v10 = 0;
    }
    ov96_0220B528(v7->unk_4C, v10);
    if (ov96_021E5F24(data) == 0) {
        ov96_0220C9A0(v7->unk_48);
    }
    ov96_0220B8D8(v7->unk_44);
    v3 = ov96_0220A910(v7->unk_40);
    ov96_0220A298(v7, v3);
    if ((ov96_0220B7CC(v7->unk_4C) != 0) && (v2 == 2) && (v3 != 0)) {
        v8 = ov96_0220B758(v7->unk_4C);
        v4 = ov96_0220B774(v7->unk_4C);
        v0 = v4;
        v9 = ov96_0220A8CC(v7->unk_40, (u32)*v8 >> 0x1A);
        ov96_0220B634(v7->unk_4C, v9, ov96_0220AAEC(v7->unk_40, ov96_0220B7B4(v7->unk_4C), (s16)(u16)((u32)(*v8 << 0x17) >> 0x17), v9, v4, &v1));
        ov96_02209BB0(v7);
        if (v9 != 0) {
            ov96_0220A704(v7, 0x10, 2);
        } else if (v1 != 0) {
            ov96_0220A704(v7, 4, 1);
        } else if (ov96_0220B774(v7->unk_4C) != 0) {
            ov96_0220A704(v7, 4, 1);
        }
    }
    v6 = v7->unk_260;
    if (v6 > 0) {
        v7->unk_260 = (s32)(v6 - 1);
    }
    ov96_021E6454(data, v7->unk_260);
    if ((v7->unk_260 == 0) && (ov96_021E5F24(data) == 0)) {
        v5 = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0x28);
        v5->words[4] = (s32)(0x10000 | v5->words[4]);
    }
    return 0;
}

s32 ov96_0220935C(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    Ov96_0220Data *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v0 = *arg1;
    switch (v0) {
    case 0:
        ov96_0220A4DC(data);
        ov96_0220A14C(data);
        if (ov96_022091B4(data) != 0) {
            *arg1 += 1;
        }
        break;
    case 1:
        if ((ov96_021E667C(data) != 0) && (v1->unk_268 == NULL)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, v1->heapId);
            PokeathlonCourse_SetStateField07(data, 2U);
        }
        break;
    }
    return 0;
}
