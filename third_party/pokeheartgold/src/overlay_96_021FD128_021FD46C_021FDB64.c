#include "global.h"

#include "overlay_96_ov96_rower_work.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"
#include "system.h"
#include "touchscreen.h"

void ov96_021E8228(PokeathlonCourseData *, u32, u32, s32, s32);

s32 ov96_021E8324(PokeathlonCourseData *, s32 *);

s32 ov96_021EB144(void *, s32);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB63C(void *, s32);

s32 ov96_021FDC7C(PokeathlonCourseData *);

extern s32 ov96_021FFEE8;

s32 ov96_021FDB64(PokeathlonCourseData *arg0);

s32 ov96_021FDB64(PokeathlonCourseData *data) {
    Ov96RowerWork *v0;
    s32 v2;
    s32 v3;
    u8 *ptr;
    PokeathlonTouchRecord *v1;
    PokeathlonTally *tally;
    u8 v4;
    u8 v5;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ptr = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(ptr);
    v1->touching = 0;
    tally = ov96_021E8A20(ptr + 0xF0);
    if ((u8)((tally->pairs[2].word1 >> 0x18) & 1) != 0) {
        ov96_021EB52C(v0->score.unk08, 1, 0);
        Sprite_SetDrawFlag(v0->score.digit0, 0);
        Sprite_SetDrawFlag(v0->score.digit1, 0);
        ov96_021EB63C(v0->unk18, 1);
        ov96_021EB144(v0->unk3C8, 1);
        ov96_021E8324(data, &ov96_021FFEE8);
        if (ov96_021E5F24(data) == 0) {
            v5 = 0;
            do {
                v4 = 0;
loop_4:
                v2 = v0->rowers[v5].unk0C[v4];
                v3 = (s32)(v2 + ((u32)(v2 >> 9) >> 0x16)) >> 0xA;
                ov96_021E8228(data, v5, v4, 3, (s32)(u8)((s32)(v3 + ((u32)(v3 >> 0xB) >> 0x14)) >> 0xC));
                v4 += 1;
                if ((u32)v4 < 3U) {
                    goto loop_4;
                }
                v5 += 1;
            } while ((u32)v5 < 4U);
        }
        return 1;
    }
    if (System_GetTouchNew() != 0) {
        ov96_021E8228(data, (u8)ov96_021E5F24(data), 3U, 0, 1);
    }
    if (System_GetTouchHeld() != 0) {
        v1->touchX = (s8)gSystem.touchX;
        v1->touchY = (s8)gSystem.touchY;
        v1->touching = 1;
    }
    ov96_021FDC7C(data);
    return 0;
}
