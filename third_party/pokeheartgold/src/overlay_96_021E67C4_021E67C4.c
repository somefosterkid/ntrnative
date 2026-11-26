#include "global.h"

#include "pokeathlon/pokeathlon.h"

struct PokeathlonCourseData;

extern void ov96_021E811C(PokeathlonCourseArgs *a0, u32 a1, void *a2);

extern u32 ov96_021E99F4(void);


BOOL ov96_021E67C4(PokeathlonCourseData *data, u8 *unused);

BOOL ov96_021E67C4(PokeathlonCourseData *data, u8 *unused) {
    int ret = 1;
    if (!ov96_021E5F24(data)) {
        ov96_021E811C(data->args, data->maxParticipants, data->filler_1E8);
        ret = ov96_021E87B4(0x16, data->filler_1E8, ov96_021E99F4(), data->system);
    }
    if (ret != 0) {
        PokeathlonCourse_SetStateField07_IfDifferent(data, 0x26, 1);
    }
    return FALSE;
}
