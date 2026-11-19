#include "global.h"

#include "pokeathlon/pokeathlon.h"

struct PokeathlonCourseData;

extern void ov96_021E811C(void *a0, u32 a1, void *a2);

extern void *ov96_021E99F4(void);

BOOL ov96_021E67C4(PokeathlonCourseData *data);

BOOL ov96_021E67C4(PokeathlonCourseData *data) {
    u8 *d = (u8 *)data;
    int ret = 1;
    if (!ov96_021E5F24(data)) {
        ov96_021E811C(((void *)((PokeathlonCourseData *)d)->args), d[0x72a], d + 0x1e8);
        ret = ov96_021E87B4(0x16, d + 0x1e8, ov96_021E99F4(), *((int *)&((PokeathlonCourseData *)d)->system));
    }
    if (ret != 0) {
        PokeathlonCourse_SetStateField07_IfDifferent(data, 0x26, 1);
    }
    return FALSE;
}
