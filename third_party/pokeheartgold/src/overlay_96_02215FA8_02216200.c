#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_rd_work.h"

s32 ov96_021E8318(PokeathlonCourseData *, s16);

s32 ov96_022193F8(PokeathlonCourseData *, s32);

u16 ov96_0221978C(struct Ov96DigitWork *param_1);

void ov96_02216200(PokeathlonCourseData *arg0);

void ov96_02216200(PokeathlonCourseData *data) {
    u8 v0;
    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    v0 = ov96_0221978C(((Ov96RdWork *)PokeathlonCourse_GetHeapAllocPtr4(data))->unk_180);
    ov96_021E8318(data, v0);
    ov96_022193F8(data, v0);
}
