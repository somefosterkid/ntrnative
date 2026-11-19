#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "overlay_96_ov96_02203468work.h"

void ov96_022033FC(PokeathlonCourseData *arg0);

void ov96_022033FC(PokeathlonCourseData *arg0) {
    s32 var_r5;
    Ov96_02203468Work *work;
    work = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    var_r5 = 0;
    do {
        if (work->items[var_r5].unk_18 != 0) {
            work->items[var_r5].unk_18--;
        }
        if (work->items[var_r5].unk_16 < 0x78) {
            work->items[var_r5].unk_16 += 1;
        }
        if (work->items[var_r5].unk_18 != 0) {
            work->items[var_r5].unk_19 = 1;
        } else {
            if (work->items[var_r5].unk_16 <= 0x64) {
                work->items[var_r5].unk_19 = 2;
            } else {
                work->items[var_r5].unk_19 = 0;
            }
        }
        var_r5 += 1;
    } while (var_r5 < 0xC);
}
