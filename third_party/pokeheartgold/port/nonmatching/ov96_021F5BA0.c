
#include "global.h"
#include "system.h"
#include "pokeathlon/pokeathlon.h"
#include "touchscreen.h"

typedef struct {
    u8 filler0[0x138];
    s32 unk138;
    u8 filler13C[4];
    u8 selected;
    u8 holdTimer;
    u8 filler142[0x16C - 0x142];
    VecFx32 touchPos;
} Ov96Work_021F5BA0;

typedef struct {
    u32 data[9];
} Ov96Blk36; /* invented: 36-byte block copy */

s32 ov96_021E8228(PokeathlonCourseData *, u8, s32, s32, s32);
void ov96_021F5F34(s32 x, s32 y, VecFx32 *from, VecFx32 *out);
u32 ov96_021F5F68(Ov96Work_021F5BA0 *, u16, u16);
s32 ov96_021F6088(Ov96Work_021F5BA0 *, u8, VecFx32 *);
s32 ov96_021F7740(PokeathlonCourseData *, s32);
void ov96_021F5BA0(PokeathlonCourseData *course) {
    u8 buf[4];
    u8 *area;
    Ov96Work_021F5BA0 *work;
    u8 *out;
    u8 *p;
    s32 i;
    u32 idx;

    area = PokeathlonCourse_GetDataCopyArea(course);
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (System_GetTouchNew()) {
        ov96_021E8228(course, ov96_021E5F24(course), 3, 0, 1);
        idx = ov96_021F5F68(work, gSystem.touchX, gSystem.touchY);
        if (idx < 3) {
            work->selected = idx;
            work->touchPos.x = gSystem.touchX << 12;
            work->touchPos.y = (gSystem.touchY << 12) + 0xC0000;
            work->touchPos.z = 0;
        }
    } else if (System_GetTouchHeld()) {
        if (work->selected < 3) {
            if (work->holdTimer < 7) {
                work->holdTimer++;
            } else {
                VecFx32 vec = { 0 };
                ov96_021F5F34((u8)gSystem.touchX, (u8)gSystem.touchY, &work->touchPos, &vec);
                ov96_021F6088(work, work->selected, &vec);
                work->selected = 3;
                work->holdTimer = 0;
            }
        }
    } else {
        if (work->selected < 3 && work->holdTimer < 7) {
            VecFx32 vec2 = { 0 };
            ov96_021F5F34((u8)gSystem.touchX, (u8)gSystem.touchY, &work->touchPos, &vec2);
            ov96_021F6088(work, work->selected, &vec2);
        }
        work->selected = 3;
        work->holdTimer = 0;
    }
    if (ov96_021E5F24(course) == 0) {
        u8 *dst;
        out = ov96_021E8A20(area + 0x28);
        dst = ov96_021E8A20(area + 0x50);
        *(Ov96Blk36 *)dst = *(Ov96Blk36 *)ov96_021E8A20(area);
        ov96_021F7740(course, work->unk138);

        for (i = 0, p = area + 0x50; i < 4; i++) {
            buf[i] = *(u32 *)ov96_021E8A20(p);
            p += 0x28;
        }
        for (i = 0; i < 4; i++) {
            out[i] = buf[i];
        }
    }
}