#include "global.h"
#include "overlay_99_pokeathlon_course_record.h"
#include "overlay_98_ov98text_sys.h"

u16 ov99_021E5C4C(s32 *, u8, u8);

void ov99_021E6144(void *arg0, s32 arg1);

void ov99_021E6144(void *arg0, s32 arg1) {
    u32 v1;
    Ov98TextSys *v0;

    v0 = ((PokeathlonCourseRecord *)arg0)->textContext;
    v1 = 0;
    do {
        if (arg1 != 0) {
            ov98_0221EE28(v0, v1 + 0xE, ov99_021E5C4C(*(void **)arg0, (u8) *((s32 *)&((PokeathlonCourseRecord *)arg0)->selection.page), v1));
        } else {
            ov98_0221EE84(v0, v1 + 0xE);
        }
        v1 += 1;
    } while ((s32)v1 < 3);
}
